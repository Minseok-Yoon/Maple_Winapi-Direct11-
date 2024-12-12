#pragma once
#include "../Resource/CResource.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

class CShader : public CResource
{
public:
	CShader();
	virtual ~CShader();

	HRESULT Load(const std::wstring& path) override;

	bool Create(SHADER_STAGE _eShaderStage, const wstring& _strFileName);
	bool CreateVertexShader(const wstring& _strFileName);
	bool CreatePixelShader(const wstring& _strFileName);

	void Bind();

	Microsoft::WRL::ComPtr<ID3DBlob> GetVSBlob() const { return m_VSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetHSBlob() const { return m_HSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetDSBlob() const { return m_DSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetGSBlob() const { return m_GSBlob; }
	Microsoft::WRL::ComPtr<ID3DBlob> GetPSBlob() const { return m_PSBlob; }

	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVS() const { return m_VS; }
	Microsoft::WRL::ComPtr<ID3D11HullShader> GetHS() const { return m_HS; }
	Microsoft::WRL::ComPtr<ID3D11DomainShader> GetDS() const { return m_DS; }
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetGS() const { return m_GS; }
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPS() const { return m_PS; }

	void SetRasterizerState(const RASTERIZER_STATE state) { m_RasterizerState = state; }
	void SetBlendState(const BLEND_STATE state) { m_BlendState = state; }
	void SetDepthStencilState(const DEPTHSTENCIL_STATE state) { m_DepthStencilState = state; }

private:
	static bool bWireframe;

	Microsoft::WRL::ComPtr<ID3DBlob> m_VSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_HSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_DSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_GSBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_PSBlob;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_HS;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_DS;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	RASTERIZER_STATE	m_RasterizerState;
	BLEND_STATE			m_BlendState;
	DEPTHSTENCIL_STATE	m_DepthStencilState;
};