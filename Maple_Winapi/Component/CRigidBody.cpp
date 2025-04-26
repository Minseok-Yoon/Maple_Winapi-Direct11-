#include "CRigidBody.h"
#include "../Object/CGameObject.h"
#include "../Manager/CTimeManager.h"
#include "../Component/CTransform.h"

CRigidBody::CRigidBody() :
	CComponent(COMPONENT_TYPE::CT_RigidBody),
	m_fMass(1.0f),
	m_fFriction(10.0f),
	m_vForce(Vector2(0.0f, 0.0f)),
	m_vVelocity(Vector2(0.0f, 0.0f)),
	m_vMaxVelocity(Vector2(800.0f, 800.0f)),
	m_vAccel(Vector2(0.0f, 0.0f))
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Init()
{
}

void CRigidBody::Update()
{
	// f(힘) = m(질량) * a(가속도)
	m_vAccel = m_vForce / m_fMass;

	// 속도에 가속도를 더한다.
	m_vVelocity += m_vAccel * min(CTimeManager::GetfDeltaTime(), 0.016f);

	// 마찰력에 의한 반대방향으로의 가속도
	if (!(m_vVelocity == Vector2(0.0f, 0.0f)))
	{
		Vector2 vFriction = -m_vVelocity;
		vFriction.Normalize();
		vFriction = vFriction * m_fFriction * m_fMass * CTimeManager::GetfDeltaTime();

		if (m_vVelocity.Length() <= vFriction.Length())
		{
			// 마찰 가속도가 본래 속도보다 더 큰 경우
			m_vVelocity = Vector2(0.0f, 0.0f);
		}

		else
		{
			m_vVelocity += vFriction;
		}
	}

	// 속도 제한 검사
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
	}

	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
	}

	// 속도에 따른 이동
	Move();
}

void CRigidBody::LateUpdate()
{
}

void CRigidBody::Render(const Matrix& view, const Matrix& projection)
{
}

void CRigidBody::Move()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	Vector3 vPos = tr->GetLocalPosition();
	vPos = vPos + m_vVelocity * CTimeManager::GetfDeltaTime();
	tr->SetLocalPosition(vPos);

	m_vForce = Vector2::One;
}

void CRigidBody::ClearForce()
{
	m_vForce = Vector2(0.0f, 0.0f);
}