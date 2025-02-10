#pragma once
#include "CGraphics.h"

// Sample Example DX11
// https://github.com/kevinmoran/BeginnerDirect3D11

class CGraphicDevice_DX11
{
public:
	CGraphicDevice_DX11();
	~CGraphicDevice_DX11();

	/// <summary>
	/// gpu object make trought device 
	/// </summary>
	/// <returns></returns>
	bool CreateDevice();	// Direct3D 11 ����̽� ����

	// swap chain
	bool CreateSwapchain(DXGI_SWAP_CHAIN_DESC desc);	// ȭ�� ��ȯ ü�� ����(���� ���۸��� ���� ���� ü��)
	void ReportLiveResources();
	void ConfigureInfoQueue();
	bool GetBuffer(UINT Buffer, REFIID riid, void** ppSurface);	// ���� ü���� �� ���� ��������
	bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);	// ���� Ÿ�� �� ����
	bool CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView);	// ����/���ٽ� �� ����
	bool CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);	// 2D �ؽ�ó ����
	bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);	// ���÷� ���� ����(�ؽ�ó ���͸� ����)
	bool CreateVertexShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11VertexShader** ppVertexShader);	// ���ؽ� ���̴� ����
	bool CreatePixelShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11PixelShader** ppPixelShader);		// �ȼ� ���̴� ����
	bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements
		, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout);	// �Է� ���̾ƿ� ����
	bool CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);	// ���� ����(����, �ε���, ��� ���� ��)
	bool CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);	// ���̴� ���ҽ� ����
	bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);	// �����Ͷ����� ���� ����(������ �׸��� ��� ����)
	bool CreateBlendState(const D3D11_BLEND_DESC* pBlendState, ID3D11BlendState** ppBlendState);	// ���� ���� ����(���� �� ����)
	bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState);	// DepthStencil ���� ����
	/// <summary>
	/// context swtich
	/// </summary>
	void SetDataGpuBuffer(ID3D11Buffer* buffer, void* data, UINT size);
	void SetShaderResource(SHADER_STAGE stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV);

	void BindInputLayout(ID3D11InputLayout* pInputLayout);	// �Է� ���̾ƿ��� GPU�� ���ε�
	void BindPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY topology);	// �⺻ ���� ���� ���� (��: �ﰢ�� ���)
	void BindVS(ID3D11VertexShader* pVertexShader);	// ���ؽ� ���̴��� GPU�� ���ε�
	void BindPS(ID3D11PixelShader* pPixelShader);	// �ȼ� ���̴��� GPU�� ���ε�
	void BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);	// ���� ���۸� GPU�� ���ε�
	void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);	// �ε��� ���۸� GPU�� ���ε�
	void BindConstantBuffer(SHADER_STAGE stage, CB_TYPE type, ID3D11Buffer* buffer);	// ��� ���۸� ���̴��� ���ε�
	void BindSampler(SHADER_STAGE stage, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);	// ���÷� ���¸� ���̴��� ���ε�
	void BindSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);	// ���÷� ���� �迭�� ���̴��� ���ε�
	void BindRasterizerState(ID3D11RasterizerState* pRasterizerState);	// �����Ͷ����� ���¸� GPU�� ���ε�
	void BindBlendState(ID3D11BlendState* pBlendState, const FLOAT BlendFactor[4], UINT SampleMask);	// ���� ���¸� GPU�� ���ε�
	void BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef);			// ���� ���ٽ� ���¸� GPU�� ���ε�
	void BindViewPort();	// ����Ʈ�� GPU�� ���ε�
	void BindRenderTargets(UINT NumViews = 1, ID3D11RenderTargetView* const* ppRenderTargetViews = nullptr, ID3D11DepthStencilView* pDepthStencilView = nullptr);	// ���� Ÿ�ٰ� ���� ���ٽ��� ���ε�
	void BindDefaultRenderTarget();	// �⺻ ���� Ÿ���� ���ε�

	void ClearRenderTargetView();	// ���� Ÿ�� �並 �ʱ�ȭ
	void ClearDepthStencilView();	// ����/���ٽ� �並 �ʱ�ȭ

	void Initialize();
	void CreateDepthStencilView();
	void Draw(UINT VertexCount, UINT StartVertexLocation);	// ���� ���� ���� ��ġ�� ������� ��ο� ȣ��
	void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);	// �ε����� ����� ��ο� ȣ��
	void Present();	// ���� ü���� ���� ȭ�鿡 ����� ǥ��

public:
	Microsoft::WRL::ComPtr<ID3D11Device> GetID3D11Device() { return mDevice; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetID3D11DeviceContext() { return mContext; }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
	Microsoft::WRL::ComPtr<IDXGISwapChain>	mSwapChain;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplers;
};

inline CGraphicDevice_DX11*& GetDevice()
{
	static CGraphicDevice_DX11* device = nullptr;
	return device;
}

inline ID3D11DeviceContext* GetDeviceContext()
{
	static ID3D11DeviceContext* context = GetDevice()->GetID3D11DeviceContext().Get();
	return context;
}