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
	bool CreateDevice();	// Direct3D 11 디바이스 생성

	// swap chain
	bool CreateSwapchain(DXGI_SWAP_CHAIN_DESC desc);	// 화면 교환 체인 생성(더블 버퍼링을 위한 스왑 체인)
	void ReportLiveResources();
	void ConfigureInfoQueue();
	bool GetBuffer(UINT Buffer, REFIID riid, void** ppSurface);	// 스왑 체인의 백 버퍼 가져오기
	bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);	// 렌더 타겟 뷰 생성
	bool CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView);	// 깊이/스텐실 뷰 생성
	bool CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);	// 2D 텍스처 생성
	bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);	// 샘플러 상태 생성(텍스처 필터링 제어)
	bool CreateVertexShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11VertexShader** ppVertexShader);	// 버텍스 셰이더 생성
	bool CreatePixelShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11PixelShader** ppPixelShader);		// 픽셀 셰이더 생성
	bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements
		, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout);	// 입력 레이아웃 생성
	bool CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);	// 버퍼 생성(정점, 인덱스, 상수 버퍼 등)
	bool CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);	// 셰이더 리소스 생성
	bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);	// 레스터라이저 상태 생성(폴리곤 그리기 방법 설정)
	bool CreateBlendState(const D3D11_BLEND_DESC* pBlendState, ID3D11BlendState** ppBlendState);	// 블렌드 상태 생성(투명도 등 설정)
	bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState);	// DepthStencil 상태 생성
	/// <summary>
	/// context swtich
	/// </summary>
	void SetDataGpuBuffer(ID3D11Buffer* buffer, void* data, UINT size);
	void SetShaderResource(SHADER_STAGE stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV);

	void BindInputLayout(ID3D11InputLayout* pInputLayout);	// 입력 레이아웃을 GPU에 바인딩
	void BindPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY topology);	// 기본 도형 구조 설정 (예: 삼각형 목록)
	void BindVS(ID3D11VertexShader* pVertexShader);	// 버텍스 셰이더를 GPU에 바인딩
	void BindPS(ID3D11PixelShader* pPixelShader);	// 픽셀 셰이더를 GPU에 바인딩
	void BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);	// 정점 버퍼를 GPU에 바인딩
	void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);	// 인덱스 버퍼를 GPU에 바인딩
	void BindConstantBuffer(SHADER_STAGE stage, CB_TYPE type, ID3D11Buffer* buffer);	// 상수 버퍼를 셰이더에 바인딩
	void BindSampler(SHADER_STAGE stage, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);	// 샘플러 상태를 셰이더에 바인딩
	void BindSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);	// 샘플러 상태 배열을 셰이더에 바인딩
	void BindRasterizerState(ID3D11RasterizerState* pRasterizerState);	// 래스터라이저 상태를 GPU에 바인딩
	void BindBlendState(ID3D11BlendState* pBlendState, const FLOAT BlendFactor[4], UINT SampleMask);	// 블렌드 상태를 GPU에 바인딩
	void BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef);			// 깊이 스텐실 상태를 GPU에 바인딩
	void BindViewPort();	// 뷰포트를 GPU에 바인딩
	void BindRenderTargets(UINT NumViews = 1, ID3D11RenderTargetView* const* ppRenderTargetViews = nullptr, ID3D11DepthStencilView* pDepthStencilView = nullptr);	// 렌더 타겟과 깊이 스텐실을 바인딩
	void BindDefaultRenderTarget();	// 기본 렌더 타겟을 바인딩

	void ClearRenderTargetView();	// 렌더 타겟 뷰를 초기화
	void ClearDepthStencilView();	// 깊이/스텐실 뷰를 초기화

	void Initialize();
	void CreateDepthStencilView();
	void Draw(UINT VertexCount, UINT StartVertexLocation);	// 정점 수와 시작 위치를 기반으로 드로우 호출
	void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);	// 인덱스를 사용한 드로우 호출
	void Present();	// 스왑 체인을 통해 화면에 결과를 표시

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