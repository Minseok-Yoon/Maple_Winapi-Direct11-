#include "CCollider.h"
#include "../Object/CGameObject.h"
#include "../Component/CScript.h"
#include "../Module/SelectGDI.h"

UINT CCollider::g_iNextID = 1;
bool CCollider::g_bRenderColliders = false;

CCollider::CCollider(COLLIDER_TYPE _eColliderType) :
	CComponent(COMPONENT_TYPE::CT_Collider),
	m_eColliderType(_eColliderType),
	m_iID(g_iNextID++),
	m_vScale(Vector2(1.0f, 1.0f)),
	m_bEnable(true)
{
}

CCollider::~CCollider()
{
}

void CCollider::Init()
{
}

void CCollider::Update()
{
}

void CCollider::LateUpdate()
{
}

void CCollider::Render()
{
	// �浹ü�� ��Ȱ��ȭ�� ���, �Ǵ� �������� ��Ȱ��ȭ�� ��� ������ �ߴ�
	if (!m_bEnable || !g_bRenderColliders)
		return;
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	CScript* sr = GetOwner()->GetComponent<CScript>();
	sr->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionStay(CCollider* _pOther)
{
	CScript* sr = GetOwner()->GetComponent<CScript>();
	sr->OnCollisionStay(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	CScript* sr = GetOwner()->GetComponent<CScript>();
	sr->OnCollisionExit(_pOther);
}