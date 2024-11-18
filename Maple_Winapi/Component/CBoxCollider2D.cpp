#include "CBoxCollider2D.h"
#include "CTransform.h"
#include "../Object/CGameObject.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"
#include "../Module/SelectGDI.h"

CBoxCollider2D::CBoxCollider2D() :
	CCollider(COLLIDER_TYPE::CT_Rect2D),
	m_iCol(0)
{
}

CBoxCollider2D::~CBoxCollider2D()
{
}

void CBoxCollider2D::Init()
{
}

void CBoxCollider2D::Update()
{
}

void CBoxCollider2D::LateUpdate()
{
}

void CBoxCollider2D::Render()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	Vector3 vPos = tr->GetPosition();
}
