#include "CBaseRenderer.h"
#include "CTransform.h"
#include "../Object/CGameObject.h"

CBaseRenderer::CBaseRenderer(COMPONENT_TYPE type) :
	CComponent(type),
	m_pMaterial(nullptr)
{
}

CBaseRenderer::~CBaseRenderer()
{
}

void CBaseRenderer::Init()
{
}

void CBaseRenderer::Update()
{
}

void CBaseRenderer::LateUpdate()
{
}

void CBaseRenderer::Render(const Matrix& view, const Matrix& projection)
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	if (tr)
		tr->Bind(view, projection);

	if (m_pMesh)
		m_pMesh->Bind();

	if (m_pMaterial)
		m_pMaterial->BindShader();
}

void CBaseRenderer::Draw()
{
	if (m_pMesh)
		GetDevice()->DrawIndexed(m_pMesh->GetIndexCount(), 0, 0);
}