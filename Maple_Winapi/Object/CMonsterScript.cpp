#include "CMonsterScript.h"
#include "../Component/CRigidBody.h"
#include "../Manager/CKeyManager.h"
#include "../Component/CAnimator.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CRenderer.h"
#include "../Manager/CTimeManager.h"
#include "CBackGround.h"
#include "../Test/CRandom.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"
#include "../Resource/CTexture.h"
#include "../Object/CMonster.h"

CMonsterScript::CMonsterScript() :
	CScript(SCRIPT_TYPE::ST_MonsterScript),
	m_iDir(1)
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::OnInit()
{
	m_pTransform = GetOwner()->GetTransform();

	if (m_pTransform == nullptr)
	{
		OutputDebugStringA("ERROR: CMonsterScript - Transform is null!\n");
	}

	CScene* pScene = CSceneManager::GetCurScene();
	if (!pScene)
	{
		OutputDebugStringA("ERROR: CMonsterScript - Current scene is null!\n");
		return;
	}

	m_pBackGround = pScene->GetBackGround();
	if (!m_pBackGround)
	{
		OutputDebugStringA("ERROR: CMonsterScript - BackGround is null!\n");
	}
}

void CMonsterScript::OnUpdate()
{
	if (!m_pAnimator)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	if (!m_pAnimator)
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();

	if (!m_pMonOwner) return;

	switch (m_pMonOwner->GetCurState())  // Monster에서 직접 가져오기
	{
	case MON_STATE::MS_Idle:
		PlayAnimation(MON_STATE::MS_Idle);
		break;
	case MON_STATE::MS_Move:
		PlayAnimation(MON_STATE::MS_Move);
		break;
	case MON_STATE::MS_Attack:
		PlayAnimation(MON_STATE::MS_Attack);
		break;
	case MON_STATE::MS_Dead:
		PlayAnimation(MON_STATE::MS_Dead);
		dead();
		break;
	}
}

void CMonsterScript::OnLateUpdate()
{
}

void CMonsterScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CMonsterScript::AttackEffect()
{
}

void CMonsterScript::OnCollisionEnter(CCollider* other)
{
}

void CMonsterScript::OnCollisionStay(CCollider* other)
{
}

void CMonsterScript::OnCollisionExit(CCollider* other)
{
}

void CMonsterScript::RandomChangeDirTime(float _fMinTime, float _fMaxTime)
{
	CRandom pRandom;
	pRandom.SetSeed(time(nullptr));
	m_fChangeDirTime = pRandom.RandomFloat(_fMinTime, _fMaxTime);
}

void CMonsterScript::RandomChangeDir()
{
	CRandom pRandom;
	pRandom.SetSeed(reinterpret_cast<long long>(this) + time(nullptr));
	int iRandom = pRandom.RandomInt(0, 1);
	if (iRandom == 0)
	{
		changeDir();
	}
	return;
}

void CMonsterScript::PlayAnimation(MON_STATE state)
{
	if (!m_pAnimator)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	if (!m_pAnimator)
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (!sr)
		sr = GetOwner()->AddComponent<CSpriteRenderer>();

	m_pAnimator->SetSpriteRenderer(sr);

	if (m_AnimMap.find(state) == m_AnimMap.end())
	{
		OutputDebugStringA("ERROR: No animation data found for state\n");
		return;
	}

	const MonsterAnimInfo& animData = m_AnimMap[state];
	std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	std::wstring animName = std::to_wstring(static_cast<int>(state)) + L"_" + direction;

	if (m_strCurAnimName == animName && m_pAnimator->IsPlaying())
		return;

	if (!m_pAnimator->HasAnimation(animName))
	{
		std::wstring path = animData.pathPattern;
		// path에서 %s → direction 치환
		size_t pos = path.find(L"%s");
		if (pos != std::wstring::npos)
			path.replace(pos, 2, direction);

		m_pAnimator->AddFrameAnimation(
			animName,
			path.c_str(),
			animData.frameCount,
			Vector2(0.f, 0.f),
			animData.frameSize.x,
			animData.frameSize.y,
			0.f, 0.f,
			animData.frameDuration
		);
	}

	m_pAnimator->Play(animName, true);
	m_strCurAnimName = animName;
}

TextureColor CMonsterScript::checkGroundColor(const Vector3& _vPos)
{
	if (!m_pBackGround)
	{
		OutputDebugStringA("ERROR: CheckGround failed - BackGround is null!\n");
		return TextureColor(255, 0, 255, 255); // 또는 기본값
	}

	return m_pBackGround->GetPixelColor(_vPos);
}

void CMonsterScript::changeDir()
{
}

void CMonsterScript::moveUpdate()
{
	m_fChangeDirTime -= CTimeManager::GetfDeltaTime();
	if (m_fChangeDirTime <= 0.0f)
	{
		RandomChangeDirTime(3.0f, 6.0f);
		RandomChangeDir();
	}

	float vMoveDeltaX = vMoveVectorForce.x * CTimeManager::GetfDeltaTime();
	if (vMoveDeltaX == 0.0f) return;

	float fStep = (vMoveDeltaX > 0.0f) ? MOVE_CHECK_FLOAT : -MOVE_CHECK_FLOAT;
	float fMoved = 0.0f;

	while (abs(fMoved) < abs(vMoveDeltaX))
	{
		Vector3 vStepMove = Vector3(fStep, 0.0f, 0.0f);
		Vector3 vNextPos = m_pTransform->GetLocalPosition() + vStepMove;

		// ----- 벽 감지 -----
		TextureColor topColor = checkGroundColor(vNextPos + Vector3(0.0f, 1.0f, 0.0f));
		int upYPivot = 1;
		while (upYPivot <= UP_PIXEL_LIMIT &&
			(topColor == GROUND_COLOR || topColor == FOOTHOLD_COLOR))
		{
			++upYPivot;
			topColor = checkGroundColor(vNextPos + Vector3(0.0f, (float)upYPivot, 0.0f));
		}

		// 벽으로 간주되면 방향 전환
		if (upYPivot > UP_PIXEL_LIMIT && bWallCheck)
		{
			changeDir();
			RandomChangeDirTime(2.0f, 4.0f);
			return;
		}

		// ----- 절벽 감지 -----
		TextureColor groundColor = checkGroundColor(vNextPos);
		int downYPivot = 0;

		if (groundColor != GROUND_COLOR && groundColor != FOOTHOLD_COLOR)
		{
			TextureColor belowColor = groundColor;

			while (downYPivot > -DOWN_PIXEL_LIMIT &&
				(belowColor != GROUND_COLOR && belowColor != FOOTHOLD_COLOR))
			{
				--downYPivot;
				belowColor = checkGroundColor(vNextPos + Vector3(0.0f, (float)downYPivot, 0.0f));
			}

			if (downYPivot <= -DOWN_PIXEL_LIMIT)
			{
				changeDir();
				RandomChangeDirTime(2.0f, 4.0f);
				return;
			}
		}

		// 이동 수행
		m_pTransform->AddLocalPosition(vStepMove);
		fMoved += fStep;
	}
}

void CMonsterScript::idle()
{
	/*if (!m_pAnimator)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	if (!m_pAnimator)
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (!sr)
		sr = GetOwner()->AddComponent<CSpriteRenderer>();

	m_pAnimator->SetSpriteRenderer(sr);

	std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	std::wstring animationName = L"Monster_Idle_" + direction;

	if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
		return;

	if (!m_pAnimator->HasAnimation(animationName))
	{
		std::wstring filePathPattern = m_tMonsterAnimInfo.pathPattern;
		int frameCount = m_tMonsterAnimInfo.frameCount;

		m_pAnimator->AddFrameAnimation(
			animationName,
			filePathPattern.c_str(),
			frameCount,
			Vector2(0.f, 0.f),
			m_tMonsterAnimInfo.frameSize.x,
			m_tMonsterAnimInfo.frameSize.y,
			0.f,
			0.f,
			0.1f
		);
	}

	m_pAnimator->Play(animationName, true);
	m_strCurAnimName = animationName;*/

	//if (m_pAnimator == nullptr)
	//	m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	//// SpriteRenderer 설정
	//CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	//if (sr == nullptr)
	//	sr = GetOwner()->AddComponent<CSpriteRenderer>();

	//m_pAnimator->SetSpriteRenderer(sr);

	//// 애니메이션 이름 생성
	//wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	//wstring animationName = L"Monster_Idle_" + direction;

	//// 이미 이 애니메이션이 재생 중이면 아무것도 안 함
	//if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
	//	return;

	//// 애니메이션 중복 추가 방지
	//if (!m_pAnimator->HasAnimation(animationName))
	//{
	//	wstring filePathPattern = L"../Resources/Texture/Monster/Erdas/Gladness/Stand/" + direction + L"/Gladness_stand_%d.png";
	//	int frameCount = 8;
	//	float frameDuration = 0.1f;

	//	m_pAnimator->AddFrameAnimation(
	//		animationName,
	//		filePathPattern.c_str(),
	//		frameCount,
	//		Vector2(0.f, 0.f),
	//		87.f,
	//		108.f,
	//		0.f,
	//		0.f,
	//		frameDuration
	//	);
	//}


	//// 새 애니메이션 재생
	//m_pAnimator->Play(animationName, true);
	//m_strCurAnimName = animationName;
}

void CMonsterScript::move()
{
	CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	rb->ClearForce();
}

void CMonsterScript::dead()
{
	std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	std::wstring animName = std::to_wstring(static_cast<int>(MON_STATE::MS_Dead)) + L"_" + direction;

	if (!m_pAnimator || !m_pAnimator->HasAnimation(animName))
		return;

	function<void()>& completeEvent = m_pAnimator->GetCompleteEvent(animName);
	if (!completeEvent)  //  중복 등록 방지
	{
		completeEvent = [this]() {
			if (m_pMonOwner)
				m_pMonOwner->DropItem();

			GetOwner()->SetDead();

			if (CPlayer* pPlayer = CSceneManager::GetCurScene()->GetPlayer())
				pPlayer->OnMonsterKilled(L"Gladness_of_Erdas");
		};
	}
}
	//if (m_pAnimator == nullptr)
	//	m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	//if (m_pAnimator == nullptr)
	//	return;

	//// SpriteRenderer 설정
	//CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	//if (sr == nullptr)
	//	sr = GetOwner()->AddComponent<CSpriteRenderer>();

	//m_pAnimator->SetSpriteRenderer(sr);

	//// 애니메이션 이름 생성
	//wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	//wstring animationName = L"Monster_Dead_" + direction;

	//// 이미 이 애니메이션이 재생 중이면 아무것도 안 함
	//if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
	//	return;

	//// 애니메이션 중복 추가 방지
	//if (!m_pAnimator->HasAnimation(animationName))
	//{
	//	wstring filePathPattern;
	//	if (direction == L"Right")
	//	{
	//		filePathPattern = L"../Resources/Texture/Monster/Erdas/Gladness/Dead/" + direction + L"/Gladness_Dead_Right_%d.png";
	//	}
	//	else
	//	{
	//		filePathPattern = L"../Resources/Texture/Monster/Erdas/Gladness/Dead/" + direction + L"/Gladness_Dead_Left_%d.png";
	//	}
	//	int frameCount = 8;
	//	float frameDuration = 0.1f;

	//	m_pAnimator->AddFrameAnimation(
	//		animationName,
	//		filePathPattern.c_str(),
	//		frameCount,
	//		Vector2(0.f, 0.f),
	//		203.f,
	//		160.f,
	//		0.f,
	//		0.f,
	//		frameDuration
	//	);
	//}

	//if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
	//	return;

	//function<void()>& completeEvent = m_pAnimator->GetCompleteEvent(animationName);

	//if (!completeEvent)
	//{
	//	completeEvent = [this]() {
	//		if (m_pMonOwner)
	//			m_pMonOwner->DropItem();

	//		GetOwner()->SetDead();
	//	};
	//}

	//m_pAnimator->Play(animationName, false);
	//m_strCurAnimName = animationName;

	//// 몬스터 사망 시, 플레이어에게 알림
	//// pPlayer가 nullptr로 나옴 여기서 부터 수정하기
	//CPlayer* pPlayer = CSceneManager::GetCurScene()->GetPlayer();
	//if (pPlayer)
	//{
	//	pPlayer->OnMonsterKilled(L"Gladness_of_Erdas");
	//}

//// 몬스터의 객체를 어떻게 가져올까?
	//CGameObject* pMonster = GetOwner();
	//if (pMonster != nullptr)
	//{
	//	m_pAnimator = pMonster->GetComponent<CAnimator>();
	//}

	//if (m_pAnimator == nullptr)
	//{
	//	m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	//}

	//// SpriteRenderer 가져오기
	//CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	//if (sr == nullptr)
	//{
	//	sr = GetOwner()->AddComponent<CSpriteRenderer>();
	//}
	//m_pAnimator->SetSpriteRenderer(sr);

	//switch (m_eMonState)
	//{
	//case MON_STATE::MS_Idle:
	//	idle();
	//	break;
	///*case PLAYER_STATE::PS_Walk:
	//	move();
	//	break;
	//case PLAYER_STATE::PS_Attack:
	//	attack();
	//	break;
	//case PLAYER_STATE::PS_Prone:
	//	prone();
	//	break;
	//case PLAYER_STATE::PS_Jump:
	//	jump();
	//	break;*/
	//}

//m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	//if (m_pAnimator == nullptr)
	//{
	//	m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	//}

	//// SpriteRenderer 가져오기
	//CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	//if (sr == nullptr)
	//{
	//	sr = GetOwner()->AddComponent<CSpriteRenderer>();
	//}

	//// Idle 애니메이션 생성
	//wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	//wstring animationName = L"Monster_Idle";
	//wstring filePathPattern = L"../Resources/Texture/Monster/Erdas/Gladness/Stand/" + direction + L"/Gladness_stand_%d.png";  // %d로 프레임 번호 변경
	//int frameCount = 8;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	//float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	//// 애니메이션 추가
	//m_pAnimator->AddFrameAnimation(
	//	animationName,
	//	filePathPattern.c_str(),
	//	frameCount,
	//	Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
	//	87.0f,                 // 각 프레임의 너비
	//	108.0f,                 // 각 프레임의 높이
	//	0.0f,                  // 프레임 간 x 오프셋
	//	0.0f,                  // 프레임 간 y 오프셋
	//	frameDuration         // 프레임 지속 시간
	//);

	//// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	////animator->SetSpriteRenderer(sr);
	//m_pAnimator->Play(animationName, true);  // 반복 재생

//void CMonsterScript::OnDeath()
//{
//	if (!m_pMonOwner) return;
//
//	// 상태를 바꾸는 책임은 Monster 쪽에 있음. 이쪽에서는 애니메이션만 처리
//	if (m_pAnimator == nullptr)
//		m_pAnimator = GetOwner()->GetComponent<CAnimator>();
//
//	if (m_pAnimator)
//	{
//		m_pAnimator->Play(m_iDir == -1 ? L"DeadLeft" : L"DeadRight", false);
//		//m_bDeadAniFin = false;
//	}
//}