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

CPlayerScript::CPlayerScript() :
	m_ePlayerState(PLAYER_STATE::PS_Idle),
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
	if (m_pAnimator == nullptr)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	switch (m_ePlayerState)
	{
	case PLAYER_STATE::PS_Idle:
		idle();
		break;

	case PLAYER_STATE::PS_Walk:
		move();
		break;

	case PLAYER_STATE::PS_Jump:
		jump();
		break;

	case PLAYER_STATE::PS_Attack:
		attack();
		break;

	default:
		break;
	}

	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	//Vector2 pos = tr->GetPosition();
	
	CRigidBody* playerRB = GetOwner()->GetComponent<CRigidBody>();
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
	CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	rb->ClearForce();

	if (KEY_HOLD(KEY_CODE::RIGHT))
	{
		rb->AddForce(Vector2(1200.0f, 0.0f));
		m_ePlayerState = PLAYER_STATE::PS_Walk;
	}
	if (KEY_HOLD(KEY_CODE::LEFT))
	{
		rb->AddForce(Vector2(-1200.0f, 0.0f));
		m_ePlayerState = PLAYER_STATE::PS_Walk;
	}
	if (KEY_HOLD(KEY_CODE::UP))
	{
		rb->AddForce(Vector2(0.0f, -1200.0f));
	}
	if (KEY_HOLD(KEY_CODE::DOWN))
	{
		rb->AddForce(Vector2(0.0f, 1200.0f));
	}
}

void CPlayerScript::move()
{
	CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	rb->ClearForce();

	if (KEY_HOLD(KEY_CODE::RIGHT))
	{
		rb->AddForce(Vector2(1200.0f, 0.0f));
	}
	if (KEY_HOLD(KEY_CODE::LEFT))
	{
		rb->AddForce(Vector2(-1200.0f, 0.0f));
	}
	if (KEY_HOLD(KEY_CODE::UP))
	{
		rb->AddForce(Vector2(0.0f, -1200.0f));
	}
	if (KEY_HOLD(KEY_CODE::DOWN))
	{
		rb->AddForce(Vector2(0.0f, 1200.0f));
	}

	if (KEY_AWAY(KEY_CODE::RIGHT) || KEY_AWAY(KEY_CODE::LEFT) ||
		KEY_AWAY(KEY_CODE::UP) || KEY_AWAY(KEY_CODE::DOWN))
	{
		rb->ClearForce();
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}

	//m_pAnimator->Play(L"WalkRight", true);
}

void CPlayerScript::jump()
{
}

void CPlayerScript::attack()
{
	if (KEY_TAP(KEY_CODE::X))
	{
		if (m_ePlayerState != PLAYER_STATE::PS_Attack)
		{
			m_ePlayerState = PLAYER_STATE::PS_Attack;
		}
	}

	if (m_pAnimator->IsFinish())
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}
}
