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
    // ������ CRigidBody Updat�� ȣ���Ͽ� �⺻ ���� ó���� ����
    CRigidBody::Update();

    // ���� ��� �ִ� ��� ���� �ӵ��� 0���� �����Ͽ� �߷� ������ ���� �ʰ� �Ѵ�.
    if (m_bGround)
    {
        // ������ ���� ��
        Vector2 gravity = m_vGravity;
        gravity.Normalize();

        float dot = m_vVelocity.Dot(gravity);
        m_vVelocity -= gravity * dot;
    }
    else
    {
        // ������ ���� ��
        m_vVelocity += m_vGravity * fDeltaTime;
    }

    // �ִ� �ӵ� ����
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
    if (!m_bGround) return; // ���� �پ� ���� ������ ���� �Ұ�

    m_bGround = false;

    // Owner ��ü�� Transform, RigidBody ������Ʈ�� �����´�.
    CTransform* tr = GetOwner()->GetComponent<CTransform>();
    CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();

    // RigidBody�� �ִ� ��쿡�� ���� �ӵ� ����
    if (rb)
    {
        rb->SetVelocity(Vector2(rb->GetVelocity().x, -_fJumpForce));
    }
}