#include "CGraphicsDevice_DX11.h"
#include "../Core/CCore.h"
#include "../Component/CRenderer.h"
#include "../Resource/CShader.h"
#include "../Resource/CMesh.h"
#include "../Resource/CTexture.h"
#include "../Resource/CMaterial.h"
#include "../Manager/CResourceManager.h"

extern CCore core;

CGraphicDevice_DX11::CGraphicDevice_DX11()
{
	GetDevice() = this;

	if (!(CreateDevice()))
		assert(NULL && "Create Device Failed!");
}

CGraphicDevice_DX11::~CGraphicDevice_DX11()
{
}

bool CGraphicDevice_DX11::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// Direct3D 장치 생성 시 필요한 플래그 설정(BGRA 형식 지원을 추가)
#if defined(DEBUG) || defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	OutputDebugString(L"[INFO] Creating Direct3D Device...\n");

	// D3D11CreateDevice 함수를 호출하여 Direct3D 장치를 생성
	// 첫 번째 매개변수(0): 어댑터를 자동 선택(기본 그래픽 카드 사용)
	// 두 번째 매개변수(D3D_DRIVER_TYPE_HARDWARE): 하드웨어 드라이버 타입을 지정하여 GPU를 사용
	// 세 번째 매개변수(0): 소프트웨어 드라이버를 사용하지 않으므로 NULL 설정
	// 네 번째 매개변수(creationFlags): BGRA 및 디버그 플래그를 포함한 장치 생성 옵션 설정
	// 다섯 번째 매개변수(featureLevels): 지원할 기능 수준 배열
	// 여섯 번째 매개변수(ARRAYSIZE(featureLevels)): 기능 수준 배열 크기 전달
	// 일곱 번째 매개변수(D3D11_SDK_VERSION): SDK 버전을 지정 (D3D11에서 사용)
	// 여덟 번째 매개변수(mDevice.GetAddressOf()): 생성된 장치 포인터를 mDevice에 반환
	// 아홉 번째 매개변수(0): 생성된 기능 수준을 받을 포인터 (NULL 처리)
	// 열 번째 매개변수(mContext.GetAddressOf()): 장치 컨텍스트 포인터를 mContext에 반환
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, creationFlags,
		featureLevels, ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION, mDevice.GetAddressOf(),
		nullptr, mContext.GetAddressOf())))
	{
		OutputDebugString(L"[ERROR] Failed to create Direct3D Device.\n");
		return false;
	}

	OutputDebugString(L"[SUCCESS] Direct3D Device created successfully.\n");

	ConfigureInfoQueue();

	return true;
}

bool CGraphicDevice_DX11::CreateSwapchain(DXGI_SWAP_CHAIN_DESC desc)
{
	Microsoft::WRL::ComPtr<IDXGIDevice>     pDXGIDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter>    pAdapter = nullptr;
	Microsoft::WRL::ComPtr<IDXGIFactory>    pFactory = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Debug>		debugDevice;

	if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
		return false;

	if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
		return false;

	if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
		return false;

	if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &desc, mSwapChain.GetAddressOf())))
		return false;

	ReportLiveResources();

	OutputDebugString(L"[SUCCESS] Swapchain created successfully.\n");
	return true;
}

void CGraphicDevice_DX11::ReportLiveResources()
{
	// 사용 중인 객체 출력
	Microsoft::WRL::ComPtr<ID3D11Debug> debugDevice;
	if (SUCCEEDED(mDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(debugDevice.GetAddressOf()))))
	{
		// 모든 라이브 객체들에 대한 상세 리포트를 출력합니다.
		debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
}

void CGraphicDevice_DX11::ConfigureInfoQueue()
{
	Microsoft::WRL::ComPtr<ID3D11InfoQueue> infoQueue;
	if (SUCCEEDED(mDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(infoQueue.GetAddressOf()))))
	{
		OutputDebugString(L"[INFO] Configuring Direct3D InfoQueue...\n");

		// 심각한 오류와 경고 발생 시 중단
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

		// WARNING 수준 메시지 필터링 (경고도 포함)
		D3D11_MESSAGE_SEVERITY severities[] = {
			D3D11_MESSAGE_SEVERITY_INFO,
			D3D11_MESSAGE_SEVERITY_WARNING
		};

		D3D11_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		if (FAILED(infoQueue->AddStorageFilterEntries(&filter)))
		{
			OutputDebugString(L"[ERROR] Failed to add filter to InfoQueue.\n");
		}
		else
		{
			OutputDebugString(L"[SUCCESS] InfoQueue configured successfully.\n");
		}
	}
	else
	{
		OutputDebugString(L"[WARNING] ID3D11InfoQueue not available. Debug filtering skipped.\n");
	}
}

bool CGraphicDevice_DX11::GetBuffer(UINT Buffer, REFIID riid, void** ppSurface)
{
	if (FAILED(mSwapChain->GetBuffer(Buffer, riid, ppSurface)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateRenderTargetView(ID3D11Resource* pResource, 
	const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
{
	if (FAILED(mDevice->CreateRenderTargetView(pResource, pDesc, ppRTView)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
{
	if (FAILED(mDevice->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
{
	if (FAILED(mDevice->CreateTexture2D(pDesc, pInitialData, ppTexture2D)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
{
	if (FAILED(mDevice->CreateSamplerState(pSamplerDesc, ppSamplerState)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateVertexShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11VertexShader** ppVertexShader)
{
	// 셰이더 컴파일 시 규칙 적용, 디버그 정보 포함, 최적화 건너뛰기를 위한 플래그
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

	// 컴파일 에러 메시지 출력을 위한 Blob 생성
	ID3DBlob* errorBlob = nullptr;
	const std::wstring shaderFilePath = L"..\\Shader\\";
	const std::wstring fullPath = shaderFilePath + fileName + L"VS.hlsl";

	// 경로 디버그 출력
	OutputDebugStringW(L"Shader file path: ");
	OutputDebugStringW(fullPath.c_str());
	OutputDebugStringW(L"\n");
	// 지정된 파일 이름과 기본 경로를 결합하여 HLSL 셰이더 파일을 컴파일
	D3DCompileFromFile((shaderFilePath + fileName + L"VS.hlsl").c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "main", "vs_5_0", shaderFlags, 0, ppCode, &errorBlob);

	// 컴파일 에러가 발생한 경우 에러 메시지를 디버그 출력 창에 표시하고, 메모리 해제 후 false 반환.
	if (errorBlob)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
		assert(NULL && "hlsl file have problem check message!");
		return false;
	}

	// 정점 셰이더 생성. 컴파일된 셰이더 코드의 버퍼 포인터와 버퍼 크기를 사용하여 셰이더 생성.
	if (FAILED(mDevice->CreateVertexShader((*ppCode)->GetBufferPointer(), (*ppCode)->GetBufferSize(), nullptr, ppVertexShader)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreatePixelShader(const std::wstring& fileName, ID3DBlob** ppCode, ID3D11PixelShader** ppPixelShader)
{
	// 셰이더 컴파일 시 규칙 적용, 디버그 정보 포함, 최적화 건너뛰기를 위한 플래그
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

	// 컴파일 에러 메시지 출력을 위한 Blob 생성.
	ID3DBlob* errorBlob = nullptr;
	const std::wstring shaderFilePath = L"..\\Shader\\";
	const std::wstring fullPath = shaderFilePath + fileName + L"PS.hlsl";

	// 경로 디버그 출력
	OutputDebugStringW(L"Shader file path: ");
	OutputDebugStringW(fullPath.c_str());
	OutputDebugStringW(L"\n");

	// 지정된 파일 이름과 기본 경로를 결합하여 HLSL 셰이더 파일을 컴파일.
	D3DCompileFromFile((shaderFilePath + fileName + L"PS.hlsl").c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "main", "ps_5_0", shaderFlags, 0, ppCode, &errorBlob);

	if (errorBlob)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
		assert(NULL && "hlsl file have problem check message!");
		return false;
	}

	if (FAILED(mDevice->CreatePixelShader((*ppCode)->GetBufferPointer(), (*ppCode)->GetBufferSize(), nullptr, ppPixelShader)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements
	, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout)
{
	if (FAILED(mDevice->CreateInputLayout(pInputElementDescs, NumElements
		, pShaderBytecodeWithInputSignature
		, BytecodeLength
		, ppInputLayout)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
{
	if (FAILED(mDevice->CreateBuffer(pDesc, pInitialData, ppBuffer)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
{
	if (FAILED(mDevice->CreateShaderResourceView(pResource, pDesc, ppSRView)))
		return false;

	return true;
}

bool CGraphicDevice_DX11::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
{
	if (FAILED(mDevice->CreateRasterizerState(pRasterizerDesc, ppRasterizerState)))
		return false;

	return true;
}
bool CGraphicDevice_DX11::CreateBlendState(const D3D11_BLEND_DESC* pBlendState, ID3D11BlendState** ppBlendState)
{
	if (FAILED(mDevice->CreateBlendState(pBlendState, ppBlendState)))
		return false;

	return true;
}
bool CGraphicDevice_DX11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
{
	if (FAILED(mDevice->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState)))
		return false;

	return true;
}

void CGraphicDevice_DX11::SetDataGpuBuffer(ID3D11Buffer* buffer, void* data, UINT size)
{
	D3D11_MAPPED_SUBRESOURCE sub = {};
	mContext->Map(buffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &sub);
	memcpy(sub.pData, data, size);
	mContext->Unmap(buffer, 0);
}

void CGraphicDevice_DX11::SetShaderResource(SHADER_STAGE stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV)
{
	if ((UINT)SHADER_STAGE::SS_VS & (UINT)stage)
		mContext->VSSetShaderResources(startSlot, 1, ppSRV);

	if ((UINT)SHADER_STAGE::SS_HS & (UINT)stage)
		mContext->HSSetShaderResources(startSlot, 1, ppSRV);

	if ((UINT)SHADER_STAGE::SS_DS & (UINT)stage)
		mContext->DSSetShaderResources(startSlot, 1, ppSRV);

	if ((UINT)SHADER_STAGE::SS_GS & (UINT)stage)
		mContext->GSSetShaderResources(startSlot, 1, ppSRV);

	if ((UINT)SHADER_STAGE::SS_PS & (UINT)stage)
		mContext->PSSetShaderResources(startSlot, 1, ppSRV);

	if ((UINT)SHADER_STAGE::SS_CS & (UINT)stage)
		mContext->CSSetShaderResources(startSlot, 1, ppSRV);
}

void CGraphicDevice_DX11::BindInputLayout(ID3D11InputLayout* pInputLayout)
{
	mContext->IASetInputLayout(pInputLayout);
}

void CGraphicDevice_DX11::BindPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY topology)
{
	mContext->IASetPrimitiveTopology(topology);
}

void CGraphicDevice_DX11::BindVS(ID3D11VertexShader* pVertexShader)
{
	mContext->VSSetShader(pVertexShader, nullptr, 0);
}

void CGraphicDevice_DX11::BindPS(ID3D11PixelShader* pPixelShader)
{
	mContext->PSSetShader(pPixelShader, nullptr, 0);
}

void CGraphicDevice_DX11::BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{
	mContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void CGraphicDevice_DX11::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	mContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

void CGraphicDevice_DX11::BindConstantBuffer(SHADER_STAGE stage, CB_TYPE type, ID3D11Buffer* buffer)
{
	UINT slot = (UINT)type;
	switch (stage)
	{
	case SHADER_STAGE::SS_VS:
		mContext->VSSetConstantBuffers(slot, 1, &buffer);
		break;
	case SHADER_STAGE::SS_HS:
		mContext->HSSetConstantBuffers(slot, 1, &buffer);
		break;
	case SHADER_STAGE::SS_DS:
		mContext->DSSetConstantBuffers(slot, 1, &buffer);
		break;
	case SHADER_STAGE::SS_GS:
		mContext->GSSetConstantBuffers(slot, 1, &buffer);
		break;
	case SHADER_STAGE::SS_PS:
		mContext->PSSetConstantBuffers(slot, 1, &buffer);
		break;
	case SHADER_STAGE::SS_CS:
		mContext->CSSetConstantBuffers(slot, 1, &buffer);
		break;
	case SHADER_STAGE::SS_ALL:
		mContext->VSSetConstantBuffers(slot, 1, &buffer);
		mContext->HSSetConstantBuffers(slot, 1, &buffer);
		mContext->DSSetConstantBuffers(slot, 1, &buffer);
		mContext->GSSetConstantBuffers(slot, 1, &buffer);
		mContext->PSSetConstantBuffers(slot, 1, &buffer);
		mContext->CSSetConstantBuffers(slot, 1, &buffer);
		break;
	default:
		break;
	}
}

void CGraphicDevice_DX11::BindSampler(SHADER_STAGE stage, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
{
	if (SHADER_STAGE::SS_VS == stage)
		mContext->VSSetSamplers(StartSlot, NumSamplers, ppSamplers);

	if (SHADER_STAGE::SS_HS == stage)
		mContext->HSSetSamplers(StartSlot, NumSamplers, ppSamplers);

	if (SHADER_STAGE::SS_DS == stage)
		mContext->DSSetSamplers(StartSlot, NumSamplers, ppSamplers);

	if (SHADER_STAGE::SS_GS == stage)
		mContext->GSSetSamplers(StartSlot, NumSamplers, ppSamplers);

	if (SHADER_STAGE::SS_PS == stage)
		mContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

void CGraphicDevice_DX11::BindSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
{
	BindSampler(SHADER_STAGE::SS_VS, StartSlot, NumSamplers, ppSamplers);
	BindSampler(SHADER_STAGE::SS_HS, StartSlot, NumSamplers, ppSamplers);
	BindSampler(SHADER_STAGE::SS_DS, StartSlot, NumSamplers, ppSamplers);
	BindSampler(SHADER_STAGE::SS_GS, StartSlot, NumSamplers, ppSamplers);
	BindSampler(SHADER_STAGE::SS_PS, StartSlot, NumSamplers, ppSamplers);
}

void CGraphicDevice_DX11::BindRasterizerState(ID3D11RasterizerState* pRasterizerState)
{
	mContext->RSSetState(pRasterizerState);
}
void CGraphicDevice_DX11::BindBlendState(ID3D11BlendState* pBlendState, const FLOAT BlendFactor[4], UINT SampleMask)
{
	mContext->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
}
void CGraphicDevice_DX11::BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef)
{
	mContext->OMSetDepthStencilState(pDepthStencilState, StencilRef);
}

void CGraphicDevice_DX11::BindViewPort()
{
	D3D11_VIEWPORT viewPort = {};
	viewPort.Width = static_cast<float>(core.GetWidth());
	viewPort.Height = static_cast<float>(core.GetHeight());
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	mContext->RSSetViewports(1, &viewPort);
}

void CGraphicDevice_DX11::BindRenderTargets(UINT NumViews
	, ID3D11RenderTargetView* const* ppRenderTargetViews
	, ID3D11DepthStencilView* pDepthStencilView)
{
	mContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void CGraphicDevice_DX11::BindDefaultRenderTarget()
{
	mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
}

void CGraphicDevice_DX11::ClearRenderTargetView()
{
	FLOAT backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
}

void CGraphicDevice_DX11::ClearDepthStencilView()
{
	mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CGraphicDevice_DX11::Initialize()
{
#pragma region swapchain desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.OutputWindow = core.GetMainHWnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Width = core.GetWidth();
	swapChainDesc.BufferDesc.Height = core.GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

#pragma region 4X MSAA surported check
	// 4X MSAA surported check
	UINT quility = 0;
	mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quility);
	//if (quility > 0)
	//{
	//	swapChainDesc.SampleDesc.Count = 4; // how many multisamples
	//	swapChainDesc.SampleDesc.Quality = quility - 1;
	//}
#pragma endregion

	swapChainDesc.SampleDesc.Count = 1; // how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;
#pragma endregion
	if (!(CreateSwapchain(swapChainDesc)))
		assert(NULL && "Create Swapchain Failed!");

	if (!(GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mRenderTarget.GetAddressOf()))))
		assert(NULL && "Couldn't bring rendertarget!");

	if (!(CreateRenderTargetView(mRenderTarget.Get(), nullptr, mRenderTargetView.GetAddressOf())))
		assert(NULL && "Create RenderTargetView Failed!");

	CreateDepthStencilView();
}

void CGraphicDevice_DX11::CreateDepthStencilView()
{
#pragma region depthstencil desc
	// DepthStencil 텍스처를 위한 설명 구조체 초기화
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};	
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;	// DepthStencil 뷰로 바인딩 설정
	depthStencilDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;	// 24비트 깊이와 8비트 스텐실을 위한 포맷
	depthStencilDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;				// 텍스처 사용 방식
	depthStencilDesc.Width = core.GetWidth();			// 텍스처 너비 설정
	depthStencilDesc.Height = core.GetHeight();			// 텍스처 높이 설정
	depthStencilDesc.ArraySize = 1;											// 텍스처 배열의 크기(1로 단일 텍스처)
	depthStencilDesc.SampleDesc.Count = 1;									// 멀티샘플링 설정(1로 단일 샘플)
	depthStencilDesc.SampleDesc.Quality = 0;								// 샘플 퀄리티 설정
	//depthStencilDesc.MipLevels = 1;											// 미맵 레벨 수 (1로 설정하여 미맵 없음)
	//depthStencilDesc.CPUAccessFlags = 0;									// CPU 접근 방식 설정
	//depthStencilDesc.MiscFlags = 0;											// 기타 플래그 설정
#pragma endregion

	// DepthStencil 텍스처 생성 확인
	if (!(CreateTexture2D(&depthStencilDesc, nullptr, mDepthStencil.GetAddressOf())))
		assert(NULL && "Create depthstencil texture failed!");

	// DepthStencil 텍스처 뷰 생성
	if (!(CreateDepthStencilView(mDepthStencil.Get(), nullptr, mDepthStencilView.GetAddressOf())))
		assert(NULL && "Create depthstencilview failed!");
}

void CGraphicDevice_DX11::Draw(UINT VertexCount, UINT StartVertexLocation)
{
	mContext->Draw(VertexCount, StartVertexLocation);
}

void CGraphicDevice_DX11::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	mContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void CGraphicDevice_DX11::Present()
{
	mSwapChain->Present(1, 0);
}