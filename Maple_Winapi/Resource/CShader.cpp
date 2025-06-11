#include "CShader.h"
#include "../Component/CRenderer.h"
#include "../Manager/CResourceManager.h"

bool CShader::bWireframe = false;

CShader::CShader() :
	CResource(RESOURCE_TYPE::RT_Shader),
	m_RasterizerState(RASTERIZER_STATE::RS_SolidBack),
	m_BlendState(BLEND_STATE::BS_Transparent),
	m_DepthStencilState(DEPTHSTENCIL_STATE::DS_LessEqual)
{
}

CShader::~CShader()
{
}

HRESULT CShader::Load(const std::wstring& path)
{
	size_t fineNameBeginOffset = path.rfind(L"\\") + 1;
	size_t fineNameEndOffset = path.length() - fineNameBeginOffset;
	const std::wstring fileName(path.substr(fineNameBeginOffset, fineNameEndOffset));

	if (!Create(SHADER_STAGE::SS_VS, fileName))
		return S_FALSE;
	if (!Create(SHADER_STAGE::SS_PS, fileName))
		return S_FALSE;

	return S_OK;
}

bool CShader::Create(SHADER_STAGE _eShaderStage, const wstring& _strFileName)
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
	if (bWireframe)
	{
		// Wireframe 렌더링 상태 설정
		CShader* wireframeShader = CResourceManager::Find<CShader>(L"WireframeShader");
		Microsoft::WRL::ComPtr<ID3D11VertexShader> wireframeShaderVS = wireframeShader->GetVS();
		Microsoft::WRL::ComPtr<ID3D11PixelShader> wireframeShaderPS = wireframeShader->GetPS();
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> wireframeRasterizerState
			= renderer::rasterizerStates[static_cast<UINT>(RASTERIZER_STATE::RS_WireFrame)];

		// Wireframe 전용 Rasterizer 설정
		GetDevice()->BindVS(wireframeShader->GetVS().Get());
		GetDevice()->BindPS(wireframeShader->GetPS().Get());
		GetDevice()->BindRasterizerState(renderer::rasterizerStates[static_cast<UINT>(RASTERIZER_STATE::RS_WireFrame)].Get());
		GetDevice()->BindBlendState(renderer::blendStates[static_cast<UINT>(m_BlendState)].Get(), nullptr, 0xffffff);
		GetDevice()->BindDepthStencilState(renderer::depthStencilStates[static_cast<UINT>(m_DepthStencilState)].Get(), 0);

		return;
	}

	// 일반 텍스처 렌더링
	if (m_VS) GetDevice()->BindVS(m_VS.Get());
	if (m_PS) GetDevice()->BindPS(m_PS.Get());

	GetDevice()->BindRasterizerState(renderer::rasterizerStates[static_cast<UINT>(m_RasterizerState)].Get());
	GetDevice()->BindBlendState(renderer::blendStates[static_cast<UINT>(m_BlendState)].Get(), nullptr, 0xffffff);
	GetDevice()->BindDepthStencilState(renderer::depthStencilStates[static_cast<UINT>(m_DepthStencilState)].Get(), 0);
}