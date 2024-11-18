#pragma once
#include "CResource.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

class CShader : public CResource
{
public:
	CShader();
	virtual ~CShader();

	virtual HRESULT Load(const wstring& _strPath) override;

	bool Create(const SHADER_STAGE _eShaderStage, const wstring& _strFileName);
	bool CreateVertexShader(const wstring& _strFileName);
	bool CreatePixelShader(const wstring& _strFileName);

	void Bind();

	Microsoft::WRL::ComPtr<ID3DBlob> GetVSBlob() { return m_VSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetHSBlob() { return m_HSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetDSBlob() { return m_DSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetGSBlob() { return m_GSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetPSBlob() { return m_PSBlob; }

	void SetRasterizerState(const RASTERIZER_STATE _eRasterizerState) { m_eRasterizerState = _eRasterizerState; }
	void SetBlendState(const BLEND_STATE _eBlendState) { m_eBlendState = _eBlendState; }
	void SetDepthStencilState(const DEPTHSTENCIL_STATE _eDepthStencilState) { m_eDepthStencilState = _eDepthStencilState; }

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_VSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_HSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_DSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_GSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_PSBlob;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_VS;
	Microsoft::WRL::ComPtr<ID3D11HullShader>		m_HS;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		m_DS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_GS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_PS;

	RASTERIZER_STATE		m_eRasterizerState;
	BLEND_STATE				m_eBlendState;
	DEPTHSTENCIL_STATE		m_eDepthStencilState;
};