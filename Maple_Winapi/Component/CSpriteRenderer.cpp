#include "CSpriteRenderer.h"
#include "CTransform.h"
#include "../Resource/CTexture.h"
#include "../Component/CRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"

CSpriteRenderer::CSpriteRenderer() :
	CBaseRenderer(COMPONENT_TYPE::CT_SpriteRenderer),
	m_pTexture(nullptr)
{
}

CSpriteRenderer::~CSpriteRenderer()
{
}

void CSpriteRenderer::Init()
{
	CBaseRenderer::Init();

	CMesh* m_pMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	CMaterial* m_pMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");

	SetMesh(m_pMesh);
	SetMaterial(m_pMaterial);
}

void CSpriteRenderer::Update()
{
	CBaseRenderer::Update();
}

void CSpriteRenderer::LateUpdate()
{
	CBaseRenderer::LateUpdate();
}

void CSpriteRenderer::Render(const Matrix& view, const Matrix& projection)
{
	CBaseRenderer::Render(view, projection);

	if (m_pTexture)
		m_pTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Sprite);

	CBaseRenderer::Draw();
}