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
		idle();  // Idle 애니메이션을 설정
		break;
	case PLAYER_STATE::PS_Walk:
		move();  // 걷기 애니메이션을 설정
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
		// 필요에 따라 다른 상태 추가
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

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Idle";
	wstring filePathPattern = L"../Resources/Texture/Player/Idle/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 3;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생

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

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring animationName = L"Walk";
	wstring filePathPattern = L"../Resources/Texture/Player/Walk/Right/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 4;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생
}

void CPlayerScript::prone()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Prone";
	wstring filePathPattern = L"../Resources/Texture/Player/Prone/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 1;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		80.0f,                 // 각 프레임의 너비
		37.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생

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

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Jump";
	wstring filePathPattern = L"../Resources/Texture/Player/Jump/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 1;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생

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

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Attack 애니메이션 생성
	wstring animationName = L"Attack";
	wstring filePathPattern = L"../Resources/Texture/Player/Attack/Right/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 3;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		55.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, false);  // 반복되지 않도록 false 설정

	// 애니메이션이 끝났는지 체크하고, 끝났으면 Idle 상태로 변경
	if (animator->End())  // 애니메이션이 끝났다면
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;  // Idle 상태로 변경
	}
}