#include "CSpriteRenderer.h"
#include "CTransform.h"
#include "../Resource/CTexture.h"
#include "../Component/CRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"

CSpriteRenderer::CSpriteRenderer() :
	CComponent(COMPONENT_TYPE::CT_SpriteRenderer),
	m_pTexture(nullptr),
	m_pMaterial(nullptr),
	m_pMesh(nullptr)
{
}

CSpriteRenderer::~CSpriteRenderer()
{
}

void CSpriteRenderer::Init()
{
	m_pMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	m_pMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");
}

void CSpriteRenderer::Update()
{
}

void CSpriteRenderer::LateUpdate()
{
}

void CSpriteRenderer::Render()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	if (tr)
		tr->Bind();

	if (m_pMesh)
		m_pMesh->Bind();

	if (m_pMaterial)
		m_pMaterial->BindShader();

	if (m_pTexture)
		m_pTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Sprite);

	if (m_pMesh)
		GetDevice()->DrawIndexed(m_pMesh->GetIndexCount(), 0, 0);
}