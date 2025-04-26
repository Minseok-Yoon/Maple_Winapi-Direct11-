#include "CCircleCollider2D.h"
#include "CTransform.h"
#include "CCamera.h"
#include "CRenderer.h"
#include "../Object/CGameObject.h"
#include "../Module/SelectGDI.h"

CCircleCollider2D::CCircleCollider2D() :
	m_fRadius(0.0f)
{
}

CCircleCollider2D::~CCircleCollider2D()
{
}

void CCircleCollider2D::Init()
{
}

void CCircleCollider2D::Update()
{
}

void CCircleCollider2D::LateUpdate()
{
}

void CCircleCollider2D::Render(const Matrix& view, const Matrix& projection)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	Vector3 vPos = tr->GetLocalPosition();
}