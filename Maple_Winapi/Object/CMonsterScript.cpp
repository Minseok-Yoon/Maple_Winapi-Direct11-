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

	switch (m_pMonOwner->GetCurState())  // Monster���� ���� ��������
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
		// path���� %s �� direction ġȯ
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
		return TextureColor(255, 0, 255, 255); // �Ǵ� �⺻��
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

		// ----- �� ���� -----
		TextureColor topColor = checkGroundColor(vNextPos + Vector3(0.0f, 1.0f, 0.0f));
		int upYPivot = 1;
		while (upYPivot <= UP_PIXEL_LIMIT &&
			(topColor == GROUND_COLOR || topColor == FOOTHOLD_COLOR))
		{
			++upYPivot;
			topColor = checkGroundColor(vNextPos + Vector3(0.0f, (float)upYPivot, 0.0f));
		}

		// ������ ���ֵǸ� ���� ��ȯ
		if (upYPivot > UP_PIXEL_LIMIT && bWallCheck)
		{
			changeDir();
			RandomChangeDirTime(2.0f, 4.0f);
			return;
		}

		// ----- ���� ���� -----
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

		// �̵� ����
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

	//// SpriteRenderer ����
	//CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	//if (sr == nullptr)
	//	sr = GetOwner()->AddComponent<CSpriteRenderer>();

	//m_pAnimator->SetSpriteRenderer(sr);

	//// �ִϸ��̼� �̸� ����
	//wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	//wstring animationName = L"Monster_Idle_" + direction;

	//// �̹� �� �ִϸ��̼��� ��� ���̸� �ƹ��͵� �� ��
	//if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
	//	return;

	//// �ִϸ��̼� �ߺ� �߰� ����
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


	//// �� �ִϸ��̼� ���
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
	if (!completeEvent)  //  �ߺ� ��� ����
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

	//// SpriteRenderer ����
	//CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	//if (sr == nullptr)
	//	sr = GetOwner()->AddComponent<CSpriteRenderer>();

	//m_pAnimator->SetSpriteRenderer(sr);

	//// �ִϸ��̼� �̸� ����
	//wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	//wstring animationName = L"Monster_Dead_" + direction;

	//// �̹� �� �ִϸ��̼��� ��� ���̸� �ƹ��͵� �� ��
	//if (m_strCurAnimName == animationName && m_pAnimator->IsPlaying())
	//	return;

	//// �ִϸ��̼� �ߺ� �߰� ����
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

	//// ���� ��� ��, �÷��̾�� �˸�
	//// pPlayer�� nullptr�� ���� ���⼭ ���� �����ϱ�
	//CPlayer* pPlayer = CSceneManager::GetCurScene()->GetPlayer();
	//if (pPlayer)
	//{
	//	pPlayer->OnMonsterKilled(L"Gladness_of_Erdas");
	//}

//// ������ ��ü�� ��� �����ñ�?
	//CGameObject* pMonster = GetOwner();
	//if (pMonster != nullptr)
	//{
	//	m_pAnimator = pMonster->GetComponent<CAnimator>();
	//}

	//if (m_pAnimator == nullptr)
	//{
	//	m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	//}

	//// SpriteRenderer ��������
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

	//// SpriteRenderer ��������
	//CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	//if (sr == nullptr)
	//{
	//	sr = GetOwner()->AddComponent<CSpriteRenderer>();
	//}

	//// Idle �ִϸ��̼� ����
	//wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	//wstring animationName = L"Monster_Idle";
	//wstring filePathPattern = L"../Resources/Texture/Monster/Erdas/Gladness/Stand/" + direction + L"/Gladness_stand_%d.png";  // %d�� ������ ��ȣ ����
	//int frameCount = 8;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	//float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	//// �ִϸ��̼� �߰�
	//m_pAnimator->AddFrameAnimation(
	//	animationName,
	//	filePathPattern.c_str(),
	//	frameCount,
	//	Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
	//	87.0f,                 // �� �������� �ʺ�
	//	108.0f,                 // �� �������� ����
	//	0.0f,                  // ������ �� x ������
	//	0.0f,                  // ������ �� y ������
	//	frameDuration         // ������ ���� �ð�
	//);

	//// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	////animator->SetSpriteRenderer(sr);
	//m_pAnimator->Play(animationName, true);  // �ݺ� ���

//void CMonsterScript::OnDeath()
//{
//	if (!m_pMonOwner) return;
//
//	// ���¸� �ٲٴ� å���� Monster �ʿ� ����. ���ʿ����� �ִϸ��̼Ǹ� ó��
//	if (m_pAnimator == nullptr)
//		m_pAnimator = GetOwner()->GetComponent<CAnimator>();
//
//	if (m_pAnimator)
//	{
//		m_pAnimator->Play(m_iDir == -1 ? L"DeadLeft" : L"DeadRight", false);
//		//m_bDeadAniFin = false;
//	}
//}