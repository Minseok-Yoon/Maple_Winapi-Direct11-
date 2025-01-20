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
#pragma region SetPixelInfo
	//wstring filePath = CPathManager::GetInst()->GetContentPath();
	//filePath += _pFileName;

	//FILE* file = nullptr;

	//_wfopen_s(&file, filePath.c_str(), L"rb");
	//// ���� ���� ����
	//assert(file);

	//BITMAPFILEHEADER	fh;
	//BITMAPINFOHEADER	ih;

	//fread(&fh, sizeof(fh), 1, file);
	//fread(&ih, sizeof(ih), 1, file);

	//m_iWidth = ih.biWidth;
	//m_iHeight = ih.biHeight;

	//m_vecPixel.clear();

	//m_vecPixel.resize(m_iWidth * m_iHeight);

	//fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), file);

	//fclose(file);

	//PPIXEL pPixelArr = new PIXEL[m_iWidth];		// ����¥�� �ȼ� �迭

	//// �� �Ʒ��� ���������ش�.
	//for (int i = 0; i < m_iHeight / 2; ++i)
	//{
	//	// ���� �ε����� �ȼ� �� ���� �����صд�.
	//	memcpy(pPixelArr, &m_vecPixel[i * m_iWidth], sizeof(PIXEL) * m_iWidth);
	//	memcpy(&m_vecPixel[i * m_iWidth], &m_vecPixel[(m_iHeight - i - 1) * m_iWidth],
	//		sizeof(PIXEL) * m_iWidth);
	//	memcpy(&m_vecPixel[(m_iHeight - i - 1) * m_iWidth], pPixelArr,
	//		sizeof(PIXEL) * m_iWidth);
	//}

	//delete[] pPixelArr;

	//fclose(file);
	//OutputDebugStringA("Pixel data loaded successfully.\n");

	//return true;
#pragma endregion
	// 1. CTexture�� ���� �ؽ�ó �ε�
	m_pTexture = std::make_shared<CTexture>();
	if (FAILED(m_pTexture->Load(_pFileName))) {
		OutputDebugStringA("Failed to load texture in SetPixelInfo.\n");
		return false;
	}

	// 2. �ؽ�ó�� ������ ��������
	D3D11_TEXTURE2D_DESC textureDesc = {};
	if (!m_pTexture->GetDesc(textureDesc)) {
		OutputDebugStringA("Failed to get texture description in SetPixelInfo.\n");
		return false;
	}

	m_iWidth = textureDesc.Width;
	m_iHeight = textureDesc.Height;

	// 3. Staging Texture ����
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.BindFlags = 0;

	HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_pStagingTexture);
	if (FAILED(hr)) {
		OutputDebugStringA("Failed to create staging texture in SetPixelInfo.\n");
		return false;
	}

	// 4. ���� �ؽ�ó�� ������¡ �ؽ�ó�� ����
	GetDeviceContext()->CopyResource(m_pStagingTexture.Get(), m_pTexture->GetTexture().Get());

	// 5. ������¡ �ؽ�ó���� ������ �б�
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = GetDeviceContext()->Map(m_pStagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(hr)) {
		OutputDebugStringA("Failed to map staging texture in SetPixelInfo.\n");
		return false;
	}

	// 6. �ȼ� �����͸� �о�鿩 ����
	uint32_t* pData = (uint32_t*)mappedResource.pData;

	m_vecPixel.clear();
	m_vecPixel.resize(m_iWidth * m_iHeight);

	for (int y = 0; y < m_iHeight; ++y) {
		for (int x = 0; x < m_iWidth; ++x) {
			uint32_t pixel = pData[y * m_iWidth + x];
			// �ȼ� �����͸� �迭�� ���� (PIXEL ����ü�� �´� �������)
			PIXEL pixelValue;
			pixelValue.r = (pixel >> 16) & 0xFF; // Red
			pixelValue.g = (pixel >> 8) & 0xFF;  // Green
			pixelValue.b = pixel & 0xFF;         // Blue
			pixelValue.a = (pixel >> 24) & 0xFF; // Alpha
			m_vecPixel[y * m_iWidth + x] = pixelValue;
		}
	}

	// 7. ���
	GetDeviceContext()->Unmap(m_pStagingTexture.Get(), 0);

	OutputDebugStringA("Pixel data loaded and stored successfully.\n");

	return true;
}

PIXEL CPixelCollider::GetPixelColor(int x, int y)
{
	// 1. ��ǥ ��ȿ�� �˻�
	if (x < 0 || x >= m_iWidth || y < 0 || y >= m_iHeight)
	{
		return { 0, 0, 0, 0 }; // �߸��� ��ǥ�� ���� ���� ������ ��ȯ
	}

	// 2. �ؽ�ó �����͸� Staging �ؽ�ó�� ����
	ID3D11DeviceContext* pContext = GetDeviceContext();
	pContext->CopyResource(m_pStagingTexture.Get(), m_pTexture->GetTexture().Get());

	// 3. Staging �ؽ�ó�� Map
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	HRESULT hr = pContext->Map(m_pStagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(hr))
	{
		OutputDebugStringA("Failed to map staging texture in GetPixelColor.\n");
		return { 0, 0, 0, 0 };
	}

	// 4. �ȼ� ������ �б�
	BYTE* pData = reinterpret_cast<BYTE*>(mappedResource.pData);
	UINT rowPitch = mappedResource.RowPitch;

	BYTE* pixelData = pData + (y * rowPitch) + (x * 4); // RGBA (4����Ʈ)
	PIXEL pixel = { pixelData[0], pixelData[1], pixelData[2], pixelData[3] }; // RGBA ����

	pContext->Unmap(m_pStagingTexture.Get(), 0);
	return pixel;
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
