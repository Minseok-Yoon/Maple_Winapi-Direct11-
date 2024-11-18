#include "CShader.h"
#include "../Component/CRenderer.h"

CShader::CShader() :
	CResource(RESOURCE_TYPE::RT_Shader),
	m_eRasterizerState(RASTERIZER_STATE::RS_SolidBack),
	m_eBlendState(BLEND_STATE::BS_AlphaBlend),
	m_eDepthStencilState(DEPTHSTENCIL_STATE::DS_LessEqual)
{
}

CShader::~CShader()
{
}

HRESULT CShader::Load(const wstring& _strPath)
{
	size_t fineNameBeginOffset = _strPath.rfind(L"\\") + 1;
	size_t fineNameEndOffset = _strPath.length() - fineNameBeginOffset;
	const wstring fileName(_strPath.substr(fineNameBeginOffset, fineNameEndOffset));

	if (!Create(SHADER_STAGE::SS_VS, fileName))
		return S_FALSE;
	if (!Create(SHADER_STAGE::SS_PS, fileName))
		return S_FALSE;

	return S_OK;
}

bool CShader::Create(const SHADER_STAGE _eShaderStage, const wstring& _strFileName)
{
	if (_eShaderStage == SHADER_STAGE::SS_VS)
		CreateVertexShader(_strFileName);
	if (_eShaderStage == SHADER_STAGE::SS_PS)
		CreatePixelShader(_strFileName);

	return true;
}

bool CShader::CreateVertexShader(const wstring& _strFileName)
{
	if (!GetDevice()->CreateVertexShader(_strFileName, m_VSBlob.GetAddressOf(), m_VS.GetAddressOf()))
		return false;

	return true;
}

bool CShader::CreatePixelShader(const wstring& _strFileName)
{
	if (!GetDevice()->CreatePixelShader(_strFileName, m_PSBlob.GetAddressOf(), m_PS.GetAddressOf()))
		return false;

	return true;
}

void CShader::Bind()
{
	// BindVS(CGraphicDevice_DX11.cpp)->VSSetShader(d3d11.h)
	if (m_VS)
		GetDevice()->BindVS(m_VS.Get());
	if (m_PS)
		GetDevice()->BindPS(m_PS.Get());

	GetDevice()->BindRasterizerState(renderer::rasterizerStates[(UINT)m_eRasterizerState].Get());
	GetDevice()->BindBlendState(renderer::blendStates[(UINT)m_eBlendState].Get(), nullptr, 0xffffff);
	GetDevice()->BindDepthStencilState(renderer::depthStencilStates[(UINT)m_eDepthStencilState].Get(), 0);
}
