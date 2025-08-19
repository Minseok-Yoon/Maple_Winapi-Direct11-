#include "CBossScript.h"
#include "..\Scene\CScene.h"
#include "..\Manager\CSceneManager.h"
#include "..\Manager\CTimeManager.h"
#include "../Component/CAnimator.h"
#include "../Scene/Jinhillah_Scene.h"
#include "Manifestation_of_Desire.h"

CBossScript::CBossScript() :
	CScript(SCRIPT_TYPE::ST_BossMonsterScript),
	m_iDir(-1)
{
}

CBossScript::~CBossScript()
{
}

void CBossScript::OnInit()
{
	m_pTransform = GetOwner()->GetTransform();

	CScene* pScene = CSceneManager::GetCurScene();
	m_pBackGround = pScene->GetBackGround();
}

void CBossScript::OnUpdate()
{
	if (!m_pAnimator)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	if (!m_pAnimator)
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();

	if (!m_pBossOwner) return;

	switch (m_pBossOwner->GetBossCurState())  // Monster에서 직접 가져오기
	{
	case BOSS_MON_STATE::BMS_Idle:
		idle();
		break;
	case BOSS_MON_STATE::BMS_summon1:
		summon1();
		break;
	case BOSS_MON_STATE::BMS_Move:
		move();
		break;
	case BOSS_MON_STATE::BMS_Dead:
		dead();
		break;
	}

	CheckSummonCondition();
}

void CBossScript::OnLateUpdate()
{
}

void CBossScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CBossScript::AttackEffect()
{
}

void CBossScript::OnCollisionEnter(CCollider* other)
{
}

void CBossScript::OnCollisionStay(CCollider* other)
{
}

void CBossScript::OnCollisionExit(CCollider* other)
{
}

void CBossScript::CheckSummonCondition()
{
	int monsterCount = 0;

	if(monsterCount == 0)
	{
		if (!m_bSummonPending)
		{
			m_bSummonPending = true;
			m_fSummonTimer = 10.0f; // 10초 타이머 시작
		}
		else
		{
			m_fSummonTimer -= CTimeManager::GetfDeltaTime();

			if (m_fSummonTimer <= 0.0f)
			{
				m_bSummonPending = false;
				m_pBossOwner->SetBossCurState(BOSS_MON_STATE::BMS_summon1);
			}
		}
	}
	else
	{
		// 몬스터가 다시 생기면 타이머 초기화
		m_bSummonPending = false;
	}
}

void CBossScript::SummonMonster()
{
	CScene* pScene = CSceneManager::GetCurScene();
	if (!pScene) return;

	// 현재 씬이 The_Lake_of_Oblivion인지 확인 후 캐스팅
	Jinhillah_Scene* pLakeScene = dynamic_cast<Jinhillah_Scene*>(pScene);
	if (!pLakeScene) return;

	// 소환 위치: 보스 위치 기준
	float offsetX = (m_iDir == 1) ? 150.f : -150.f;
	Vector3 summonPos = m_pTransform->GetLocalPosition() + Vector3(offsetX, 0.f, 0.f);

	// 몬스터 생성 (템플릿으로 호출)
	pLakeScene->CreateMonster<Manifestation_of_Desire>(summonPos);
}

TextureColor CBossScript::checkGroundColor(const Vector3& _vPos)
{
	if (!m_pBackGround)
	{
		OutputDebugStringA("ERROR: CheckGround failed - BackGround is null!\n");
		return TextureColor(255, 0, 255, 255); // 또는 기본값
	}

	return m_pBackGround->GetPixelColor(_vPos);
}

void CBossScript::changeDir()
{
}

void CBossScript::moveUpdate()
{
	m_fChangeDirTime -= CTimeManager::GetfDeltaTime();
	if (m_fChangeDirTime <= 0.0f)
	{
		//RandomChangeDirTime(3.0f, 6.0f);
		//RandomChangeDir();
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
			//RandomChangeDirTime(2.0f, 4.0f);
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
				//RandomChangeDirTime(2.0f, 4.0f);
				return;
			}
		}

		// 이동 수행
		m_pTransform->AddLocalPosition(vStepMove);
		fMoved += fStep;
	}
}

void CBossScript::idle()
{
	if (m_pAnimator == nullptr)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	// SpriteRenderer 설정
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
		sr = GetOwner()->AddComponent<CSpriteRenderer>();

	m_pAnimator->SetSpriteRenderer(sr);

	// 애니메이션 이름 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Jinhilla_Idle_" + direction;

	// 이미 이 애니메이션이 재생 중이면 아무것도 안 함
	if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
		return;

	// 애니메이션 중복 추가 방지
	if (!m_pAnimator->HasAnimation(animationName))
	{
		wstring filePathPattern = L"../Resources/Texture/Boss/Jinhilla/Idle/" + direction + L"/Jinhilla_stand_%d.png";
		int frameCount = 7;
		float frameDuration = 1.0f;

		m_pAnimator->AddFrameAnimation(
			animationName,
			filePathPattern.c_str(),
			frameCount,
			Vector2(0.f, 0.f),
			240.f,
			305.f,
			0.f,
			0.f,
			frameDuration
		);
	}


	// 새 애니메이션 재생
	m_pAnimator->Play(animationName, true);
	m_strCurAnimName = animationName;
}

void CBossScript::summon1()
{
	if (m_pAnimator == nullptr)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	// SpriteRenderer 설정
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
		sr = GetOwner()->AddComponent<CSpriteRenderer>();

	m_pAnimator->SetSpriteRenderer(sr);

	// 애니메이션 이름 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Jinhilla_summon1_" + direction;

	// 이미 이 애니메이션이 재생 중이면 아무것도 안 함
	if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
		return;

	// 애니메이션 중복 추가 방지
	if (!m_pAnimator->HasAnimation(animationName))
	{
		wstring filePathPattern = L"../Resources/Texture/Boss/Jinhilla/summon1/" + direction + L"/Jinhilla_summon1_%d.png";
		int frameCount = 17;
		float frameDuration = 1.0f;

		m_pAnimator->AddFrameAnimation(
			animationName,
			filePathPattern.c_str(),
			frameCount,
			Vector2(0.f, 0.f),
			274.f,
			303.f,
			0.f,
			0.f,
			frameDuration
		);
	}

	function<void()>& completeEvent = m_pAnimator->GetCompleteEvent(animationName);
	if (!completeEvent)
	{
		completeEvent = [this]() {
			SummonMonster();
			m_pBossOwner->SetBossCurState(BOSS_MON_STATE::BMS_Idle);
		};
	}

	// 새 애니메이션 재생
	m_pAnimator->Play(animationName, true);
	m_strCurAnimName = animationName;
}

void CBossScript::move()
{
}

void CBossScript::dead()
{
}
