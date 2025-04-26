#include "CMaterial.h"

CMaterial::CMaterial() :
	CResource(RESOURCE_TYPE::RT_Material),
	m_eRenderingMode(RENDERING_MODE::RM_Opaque),
	m_pAlbedoTexture(nullptr),
	m_pShader(nullptr)
{
}

CMaterial::~CMaterial()
{
}

HRESULT CMaterial::Load(const wstring& _strPath)
{
	return E_NOTIMPL;
}

void CMaterial::Bind()
{
	if (m_pShader)
		m_pShader->Bind();

	if (m_pAlbedoTexture)
		m_pAlbedoTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Albedo);
}

void CMaterial::BindShader()
{
	if (m_pShader)
		m_pShader->Bind();
}

void CMaterial::BindTextures()
{
	if (m_pAlbedoTexture)
		m_pAlbedoTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Albedo);
}

void CMaterial::SetRenderingMode(const RENDERING_MODE _eRenderingMode)
{
	m_eRenderingMode = _eRenderingMode;

	switch (_eRenderingMode)
	{
	case RENDERING_MODE::RM_Opaque:
		m_pShader->SetRasterizerState(RASTERIZER_STATE::RS_SolidNone);
		m_pShader->SetBlendState(BLEND_STATE::BS_Opaque);
		m_pShader->SetDepthStencilState(DEPTHSTENCIL_STATE::DS_LessEqual);
		break;

	case RENDERING_MODE::RM_CutOut:
		m_pShader->SetRasterizerState(RASTERIZER_STATE::RS_SolidNone);
		m_pShader->SetBlendState(BLEND_STATE::BS_Cutout);
		m_pShader->SetDepthStencilState(DEPTHSTENCIL_STATE::DS_LessEqual);
		break;

	case RENDERING_MODE::RM_Transparent:
		m_pShader->SetRasterizerState(RASTERIZER_STATE::RS_SolidNone);
		m_pShader->SetBlendState(BLEND_STATE::BS_Transparent);
		m_pShader->SetDepthStencilState(DEPTHSTENCIL_STATE::DS_Always);
		break;

	default:
		break;
	}
}
