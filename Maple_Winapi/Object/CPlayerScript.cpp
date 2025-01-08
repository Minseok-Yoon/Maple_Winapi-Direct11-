#include "CPlayerScript.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"
#include "../Object/CPlayer.h"
#include "../Component/CAnimator.h"
#include "../Component/CTransform.h"
#include "../Component/CRigidBody.h"
#include "../Component/CRenderer.h"

CPlayerScript::CPlayerScript() :
	m_ePlayerState(PLAYER_STATE::PS_Idle),
	m_iDir(1),
	m_pAnimator(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Init()
{
}

void CPlayerScript::Update()
{
	CGameObject* pPlayer = renderer::selectedObject;
	if (pPlayer != nullptr)
	{
		CAnimator* animator = pPlayer->GetComponent<CAnimator>();

		CCollider* col = pPlayer->GetComponent<CCollider>();
		col->SetOffsetPos(Vector2(0.0f, 0.0f));
		col->SetScale(Vector2(54.0f, 65.0f));
	}

	switch (m_ePlayerState)
	{
	case PLAYER_STATE::PS_Idle:
		idle();  // Idle �ִϸ��̼��� ����
		break;
	case PLAYER_STATE::PS_Walk:
		move();  // �ȱ� �ִϸ��̼��� ����
		break;
	case PLAYER_STATE::PS_Attack:
		attack();
		break;
	case PLAYER_STATE::PS_Prone:
		prone();
		break;
	case PLAYER_STATE::PS_Jump:
		jump();
		break;
		// �ʿ信 ���� �ٸ� ���� �߰�
	}
}

void CPlayerScript::LateUpdate()
{
}

void CPlayerScript::Render()
{
}

void CPlayerScript::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayerScript::OnCollisionStay(CCollider* _pOther)
{
}

void CPlayerScript::OnCollisionExit(CCollider* _pOther)
{
}

void CPlayerScript::idle()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Idle";
	wstring filePathPattern = L"../Resources/Texture/Player/Idle/" + direction + L"/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 3;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		54.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���

	if (KEY_HOLD(KEY_CODE::LEFT) || KEY_TAP(KEY_CODE::LEFT) ||
		KEY_HOLD(KEY_CODE::RIGHT) || KEY_TAP(KEY_CODE::RIGHT))
	{
		m_ePlayerState = PLAYER_STATE::PS_Walk;
	}

	if (KEY_HOLD(KEY_CODE::DOWN) || KEY_TAP(KEY_CODE::DOWN))
	{
		m_ePlayerState = PLAYER_STATE::PS_Prone;
	}

	if (KEY_HOLD(KEY_CODE::UP) || KEY_TAP(KEY_CODE::UP))
	{
		m_ePlayerState = PLAYER_STATE::PS_Jump;
	}

	if (KEY_HOLD(KEY_CODE::X) || KEY_TAP(KEY_CODE::X))
	{
		m_ePlayerState = PLAYER_STATE::PS_Attack;
	}
}

void CPlayerScript::move()
{
	CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	rb->ClearForce();

	if (KEY_HOLD(KEY_CODE::RIGHT))
	{
		rb->AddForce(Vector2(100.0f, 0.0f));
	}
	if (KEY_HOLD(KEY_CODE::LEFT))
	{
		rb->AddForce(Vector2(-100.0f, 0.0f));
	}
	if (KEY_HOLD(KEY_CODE::UP))
	{
		rb->AddForce(Vector2(0.0f, -100.0f));
	}
	if (KEY_HOLD(KEY_CODE::DOWN))
	{
		rb->AddForce(Vector2(0.0f, 100.0f));
	}

	if (KEY_AWAY(KEY_CODE::RIGHT) || KEY_AWAY(KEY_CODE::LEFT) ||
		KEY_AWAY(KEY_CODE::UP) || KEY_AWAY(KEY_CODE::DOWN))
	{
		rb->ClearForce();
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}


	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring animationName = L"Walk";
	wstring filePathPattern = L"../Resources/Texture/Player/Walk/Right/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 4;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		54.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���
}

void CPlayerScript::prone()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Prone";
	wstring filePathPattern = L"../Resources/Texture/Player/Prone/" + direction + L"/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 1;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		80.0f,                 // �� �������� �ʺ�
		37.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���

	if(KEY_AWAY(KEY_CODE::DOWN))
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}
}

void CPlayerScript::jump()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Jump";
	wstring filePathPattern = L"../Resources/Texture/Player/Jump/" + direction + L"/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 1;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		54.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���

	if (KEY_AWAY(KEY_CODE::UP))
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}
}

void CPlayerScript::attack()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Attack �ִϸ��̼� ����
	wstring animationName = L"Attack";
	wstring filePathPattern = L"../Resources/Texture/Player/Attack/Right/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 3;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		55.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, false);  // �ݺ����� �ʵ��� false ����

	// �ִϸ��̼��� �������� üũ�ϰ�, �������� Idle ���·� ����
	if (animator->End())  // �ִϸ��̼��� �����ٸ�
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;  // Idle ���·� ����
	}
}