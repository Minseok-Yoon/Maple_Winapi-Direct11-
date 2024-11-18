#include "CGravity.h"
#include "../Manager/CTimeManager.h"
#include "../Object/CGameObject.h"
#include "../Component/CRigidBody.h"
#include "../Component/CTransform.h"

CGravity::CGravity() :
    m_bGround(false),
    m_vGravity(Vector2(0.0f, 800.0f))
{
}

CGravity::~CGravity()
{
}

void CGravity::Init()
{
    CRigidBody::Init();
}

void CGravity::Update()
{
    // 기존의 CRigidBody Updat를 호출하여 기본 물리 처리를 수행
    CRigidBody::Update();

    // 땅에 닿아 있는 경우 수직 속도를 0으로 설정하여 중력 영향을 받지 않게 한다.
    if (m_bGround)
    {
        // 땅위에 있을 때
        Vector2 gravity = m_vGravity;
        gravity.Normalize();

        float dot = m_vVelocity.Dot(gravity);
        m_vVelocity -= gravity * dot;
    }
    else
    {
        // 공증에 있을 때
        m_vVelocity += m_vGravity * fDeltaTime;
    }

    // 최대 속도 제한
    Vector2 gravity = m_vGravity;
    gravity.Normalize();
    float dot = m_vVelocity.Dot(gravity);
    gravity = gravity * dot;

    Vector2 sideVelocity = m_vVelocity - gravity;
    if (m_vMaxVelocity.y < gravity.Length())
    {
        gravity.Normalize();
        gravity *= m_vMaxVelocity.y;
    }

    if (m_vMaxVelocity.x < sideVelocity.Length())
    {
        sideVelocity.Normalize();
        sideVelocity *= m_vMaxVelocity;
    }
}

void CGravity::LateUpdate()
{
    CRigidBody::LateUpdate();
}

void CGravity::Render()
{
    CRigidBody::Render();
}

void CGravity::Jump(float _fJumpForce)
{
    if (!m_bGround) return; // 땅에 붙어 있지 않으면 점프 불가

    m_bGround = false;

    // Owner 객체의 Transform, RigidBody 컴포넌트를 가져온다.
    CTransform* tr = GetOwner()->GetComponent<CTransform>();
    CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();

    // RigidBody가 있는 경우에만 점프 속도 설정
    if (rb)
    {
        rb->SetVelocity(Vector2(rb->GetVelocity().x, -_fJumpForce));
    }
}