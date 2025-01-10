#include "CCollider.h"
#include "../Object/CGameObject.h"
#include "../Component/CScript.h"
#include "../Component/CLineRenderer.h"
#include "../Module/SelectGDI.h"

UINT CCollider::g_iNextID = 1;
bool CCollider::g_bRenderColliders = false;

CCollider::CCollider() :
	CComponent(COMPONENT_TYPE::CT_Collider),
	m_iID(g_iNextID++),
	m_vScale(Vector2(1.0f, 1.0f)),
	m_bEnable(true),
	m_vBottomLeft(Vector3(-0.5f, -0.5f, -1.0f)), // 기본 크기
	m_vTopRight(Vector3(0.5f, 0.5f, -1.0f)),    // 기본 크기
	m_vColor(Vector4(0.0f, 0.0f, 255.0f, 100.0f)) // 기본 색상 (파란색)
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
	RectCollider();
}

void CCollider::LateUpdate()
{
}

void CCollider::Render()
{
	// 충돌체가 비활성화된 경우, 또는 렌더링이 비활성화된 경우 렌더링 중단
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

void CCollider::RectCollider()
{
	CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
	if (lineRenderer == nullptr)
	{
		lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
	}
	lineRenderer->SetShape(m_vBottomLeft, m_vTopRight, m_vColor);
}
