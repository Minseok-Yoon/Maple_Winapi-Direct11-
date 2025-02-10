#include "../pch.h"
#include "CPixelCollider.h"
#include "../Manager/CPathManager.h"
#include "../Core/CCore.h"

extern CCore core;

CPixelCollider::CPixelCollider() :
	m_pTexture(nullptr),
	m_pStagingTexture(nullptr)
{
	m_iWidth = 0;
	m_iHeight = 0;
}

CPixelCollider::~CPixelCollider()
{
}

bool CPixelCollider::SetPixelInfo(const wstring& _pFileName)
{
	// 텍스처 객체 생성
	m_pTexture = std::make_shared<CTexture>();
	if (FAILED(m_pTexture->Load(_pFileName))) {
		OutputDebugStringA("Failed to load texture.\n");
		return false;
	}

	// 파일 확장자 확인
	bool isBmp = (_pFileName.substr(_pFileName.find_last_of(L".") + 1) == L"bmp");

	// 텍스처의 설명 가져오기
	D3D11_TEXTURE2D_DESC textureDesc = {};
	if (!m_pTexture->GetDesc(textureDesc)) {
		OutputDebugStringA("Failed to get texture description.\n");
		return false;
	}

	// 텍스처의 포맷 가져오기
	DXGI_FORMAT format = textureDesc.Format;
	if (format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB) {
		OutputDebugStringA("Texture format is SRGB. Processing as UNORM.\n");
		format = DXGI_FORMAT_B8G8R8A8_UNORM; // SRGB를 UNORM처럼 처리
	}

	m_iWidth = textureDesc.Width;
	m_iHeight = textureDesc.Height;

	// Staging Texture 생성
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.BindFlags = 0;

	HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_pStagingTexture);
	if (FAILED(hr)) {
		OutputDebugStringA("Failed to create staging texture.\n");
		return false;
	}

	// 원본 텍스처 데이터를 복사
	GetDeviceContext()->CopyResource(m_pStagingTexture.Get(), m_pTexture->GetTexture().Get());

	// 픽셀 데이터 읽기
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = GetDeviceContext()->Map(m_pStagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(hr)) {
		OutputDebugStringA("Failed to map staging texture.\n");
		return false;
	}

	if (!mappedResource.pData) {
		OutputDebugStringA("Mapped resource data is null/\n");
		return false;
	}

	// 매핑된 데이터를 32비트 배열로 변환
	uint32_t* pData = reinterpret_cast<uint32_t*>(mappedResource.pData);
	m_vecPixel.clear();
	m_vecPixel.resize(m_iWidth * m_iHeight);

	for (int y = 0; y < m_iHeight; ++y) {
		uint32_t* row = reinterpret_cast<uint32_t*>(reinterpret_cast<BYTE*>(pData) + y * mappedResource.RowPitch);
		for (int x = 0; x < m_iWidth; ++x) {
			uint32_t pixel = row[x];

			PIXEL pixelValue;
			pixelValue.r = (pixel & 0xFF);
			pixelValue.g = ((pixel >> 8) & 0xFF);
			pixelValue.b = ((pixel >> 16) & 0xFF);
			pixelValue.a = (pixel >> 24) & 0xFF;

			int index = isBmp ? ((m_iHeight - 1 - y) * m_iWidth + x) : (y * m_iWidth + x);
			m_vecPixel[index] = pixelValue;
		}
	}

	//for (size_t i = 0; i < m_vecPixel.size(); ++i) {
	//	PIXEL& p = m_vecPixel[i];

	//	// BGRA → RGBA 변환
	//	swap(m_vecPixel[i].r, m_vecPixel[i].b);  // 직접 swap 적용

	//	char buf[128];
	//	sprintf_s(buf, "Pixel[%zu]: R=%d, G=%d, B=%d, A=%d\n",
	//		i, m_vecPixel[i].r, m_vecPixel[i].g, m_vecPixel[i].b, m_vecPixel[i].a);
	//	OutputDebugStringA(buf);
	//}

	GetDeviceContext()->Unmap(m_pStagingTexture.Get(), 0);
	return true;
}

PIXEL CPixelCollider::GetPixelColor(int x, int y)
{
	// 중심 좌표 계산
	int centerX = m_iWidth / 2;
	int centerY = m_iHeight / 2;

	// 입력 좌표를 중심 기준 좌표로 변환
	int localX = centerX + x;       // 중심 기준 X 변환
	int localY = centerY - y;       // 중심 기준 Y 변환 (반전 포함)

	// 1. 좌표 유효성 검사
	if (localX < 0 || localX >= m_iWidth || localY < 0 || localY >= m_iHeight)
	{
		return { 0, 0, 0, 0 }; // 잘못된 좌표는 투명도 포함 검정색 반환
	}

	// 2. 텍스처 데이터를 Staging 텍스처로 복사
	ID3D11DeviceContext* pContext = GetDeviceContext();
	pContext->CopyResource(m_pStagingTexture.Get(), m_pTexture->GetTexture().Get());

	// 3. Staging 텍스처를 Map
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	HRESULT hr = pContext->Map(m_pStagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(hr))
	{
		OutputDebugStringA("Failed to map staging texture in GetPixelColor.\n");
		return { 0, 0, 0, 0 };
	}

	// 4. 픽셀 데이터 읽기
	BYTE* pData = reinterpret_cast<BYTE*>(mappedResource.pData);
	UINT rowPitch = mappedResource.RowPitch;

	BYTE* pixelData = pData + (localY * rowPitch) + (localX * 4); // RGBA (4바이트)
	PIXEL pixel = { pixelData[0], pixelData[1], pixelData[2], pixelData[3] }; // RGBA 순서

	pContext->Unmap(m_pStagingTexture.Get(), 0);
	return pixel;
}

bool CPixelCollider::IsColliding(int worldX, int worldY)
{
	// 월드 좌표 -> 로컬 좌표 변환
	int localX = worldX - m_iOriginX;
	int localY = worldY - m_iOriginY;

	// 텍스처 좌표 범위 확인
	if (localX < 0 || localX >= m_iWidth || localY < 0 || localY >= m_iHeight) {
		return false; // 충돌 없음
	}

	// 픽셀 데이터 확인
	PIXEL pixel = m_vecPixel[localY * m_iWidth + localX];
	if (pixel.a > 0) { // 투명도가 0보다 크면 충돌
		return true;
	}
	return false;
}

void CPixelCollider::Init()
{
}

void CPixelCollider::Update()
{
}

void CPixelCollider::LateUpdate()
{
}

void CPixelCollider::Render()
{
}

void CPixelCollider::OnCollisionEnter(CCollider* _pOther)
{
}

void CPixelCollider::OnCollisionStay(CCollider* _pOther)
{
}

void CPixelCollider::OnCollisionExit(CCollider* _pOther)
{
}