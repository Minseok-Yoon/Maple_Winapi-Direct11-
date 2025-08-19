#include "../pch.h"
#include "CPixelCollider.h"
#include "CTransform.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"
#include "../Object/CBackGround.h"
/*#include "../Manager/CPathManager.h"
#include "../Core/CCore.h"*/

CPixelCollider::CPixelCollider() :
	CComponent(COMPONENT_TYPE::CT_PixelCollider),
	m_fCheckWidth(6.0f),
	m_fCheckDepth(3.0f),
	m_pTransform(nullptr),
	m_pBackGround(nullptr)
{
}

CPixelCollider::~CPixelCollider()
{
}

void CPixelCollider::Init()
{
	m_pTransform = GetOwner()->GetComponent<CTransform>();

	CScene* pScene = CSceneManager::GetCurScene();
	if (pScene)
	{
		m_pBackGround = pScene->GetBackGround();
	}
}

void CPixelCollider::Update()
{
	// 2025-07-07 �ʿ�� �ڵ� ����(?) or ���� ���� ����(?)
}

void CPixelCollider::LateUpdate()
{
}

void CPixelCollider::Render(const Matrix& view, const Matrix& projection)
{
}

bool CPixelCollider::CheckGround(const Vector3& worldPos)
{
	CScene* pScene = CSceneManager::GetCurScene();
	if (!pScene) return false;
	
	CBackGround* pBg = pScene->GetBackGround();
	if (!pBg) return false;

	Vector3 footPos = worldPos;
	footPos.y -= m_pTransform->GetLocalScale().y * 0.5f;

	for (float dx = -m_fCheckWidth; dx <= m_fCheckWidth; dx += 2.0f)
	{
		for (float dy = -m_fCheckDepth; dy <= 1.0f; dy += 1.0f)
		{
			Vector3 checkPos(footPos.x + dx, footPos.y + dy, 0.0f);
			if (pBg->CheckGround(checkPos))
				return true;
		}
	}

	return false;
}

float CPixelCollider::FindGroundY(const Vector3& currentPos, const Vector3& prevPos)
{
	if (!m_pBackGround)
		return currentPos.y;

	float bestY = currentPos.y;
	float minDistance = FLT_MAX;
	bool bFound = false;

	const float searchRange = 10.0f;
	const float step = 1.0f;

	for (float dy = searchRange; dy >= -searchRange; dy -= step)
	{
		Vector3 checkPos(currentPos.x, currentPos.y + dy, 0.0f);
		if (m_pBackGround->CheckGround(checkPos))
		{
			float dist = fabsf(dy);
			if (dist < minDistance)
			{
				minDistance = dist;
				bestY = checkPos.y;
				bFound = true;

				if (dist < 0.5f)
					break;
			}
		}
	}

	if (bFound)
	{
		float diffY = fabsf(currentPos.y - bestY);
		if (diffY > 1.0f)
		{
			return roundf((currentPos.y + bestY) * 0.5f);  // ������ + �ȼ�����
		}
		return roundf(bestY);
	}

	return currentPos.y;
}

bool CPixelCollider::IsColliding(int worldX, int worldY)
{
	// ���� ��ǥ -> ���� ��ǥ ��ȯ
	int localX = worldX - m_iOriginX;
	int localY = worldY - m_iOriginY;

	// �ؽ�ó ��ǥ ���� Ȯ��
	if (localX < 0 || localX >= m_iWidth || localY < 0 || localY >= m_iHeight) {
		return false; // �浹 ����
	}

	// �ȼ� ������ Ȯ��
	PIXEL pixel = m_vecPixel[localY * m_iWidth + localX];
	if (pixel.a > 0) { // ������ 0���� ũ�� �浹
		return true;
	}
	return false;
}

//extern CCore core;
//
//CPixelCollider::CPixelCollider() :
//	m_pTexture(nullptr),
//	m_pStagingTexture(nullptr)
//{
//	m_iWidth = 0;
//	m_iHeight = 0;
//}
//
//CPixelCollider::~CPixelCollider()
//{
//}
//
//bool CPixelCollider::SetPixelInfo(const wstring& _pFileName)
//{
//	// �ؽ�ó ��ü ����
//	m_pTexture = std::make_shared<CTexture>();
//	if (FAILED(m_pTexture->Load(_pFileName))) {
//		OutputDebugStringA("Failed to load texture.\n");
//		return false;
//	}
//
//	// ���� Ȯ���� Ȯ��
//	bool isBmp = (_pFileName.substr(_pFileName.find_last_of(L".") + 1) == L"bmp");
//
//	// �ؽ�ó�� ���� ��������
//	D3D11_TEXTURE2D_DESC textureDesc = {};
//	if (!m_pTexture->GetDesc(textureDesc)) {
//		OutputDebugStringA("Failed to get texture description.\n");
//		return false;
//	}
//
//	// �ؽ�ó�� ���� ��������
//	DXGI_FORMAT format = textureDesc.Format;
//	if (format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB) {
//		OutputDebugStringA("Texture format is SRGB. Processing as UNORM.\n");
//		format = DXGI_FORMAT_B8G8R8A8_UNORM; // SRGB�� UNORMó�� ó��
//	}
//
//	m_iWidth = textureDesc.Width;
//	m_iHeight = textureDesc.Height;
//
//	// Staging Texture ����
//	textureDesc.Usage = D3D11_USAGE_STAGING;
//	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//	textureDesc.BindFlags = 0;
//
//	HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_pStagingTexture);
//	if (FAILED(hr)) {
//		OutputDebugStringA("Failed to create staging texture.\n");
//		return false;
//	}
//
//	// ���� �ؽ�ó �����͸� ����
//	GetDeviceContext()->CopyResource(m_pStagingTexture.Get(), m_pTexture->GetTexture().Get());
//
//	// �ȼ� ������ �б�
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	hr = GetDeviceContext()->Map(m_pStagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
//	if (FAILED(hr)) {
//		OutputDebugStringA("Failed to map staging texture.\n");
//		return false;
//	}
//
//	if (!mappedResource.pData) {
//		OutputDebugStringA("Mapped resource data is null/\n");
//		return false;
//	}
//
//	// ���ε� �����͸� 32��Ʈ �迭�� ��ȯ
//	uint32_t* pData = reinterpret_cast<uint32_t*>(mappedResource.pData);
//	m_vecPixel.clear();
//	m_vecPixel.resize(m_iWidth * m_iHeight);
//
//	for (int y = 0; y < m_iHeight; ++y) {
//		uint32_t* row = reinterpret_cast<uint32_t*>(reinterpret_cast<BYTE*>(pData) + y * mappedResource.RowPitch);
//		for (int x = 0; x < m_iWidth; ++x) {
//			uint32_t pixel = row[x];
//
//			PIXEL pixelValue;
//			pixelValue.r = (pixel & 0xFF);
//			pixelValue.g = ((pixel >> 8) & 0xFF);
//			pixelValue.b = ((pixel >> 16) & 0xFF);
//			pixelValue.a = (pixel >> 24) & 0xFF;
//
//			int index = isBmp ? ((m_iHeight - 1 - y) * m_iWidth + x) : (y * m_iWidth + x);
//			m_vecPixel[index] = pixelValue;
//		}
//	}
//
//	//for (size_t i = 0; i < m_vecPixel.size(); ++i) {
//	//	PIXEL& p = m_vecPixel[i];
//
//	//	// BGRA �� RGBA ��ȯ
//	//	swap(m_vecPixel[i].r, m_vecPixel[i].b);  // ���� swap ����
//
//	//	char buf[128];
//	//	sprintf_s(buf, "Pixel[%zu]: R=%d, G=%d, B=%d, A=%d\n",
//	//		i, m_vecPixel[i].r, m_vecPixel[i].g, m_vecPixel[i].b, m_vecPixel[i].a);
//	//	OutputDebugStringA(buf);
//	//}
//
//	GetDeviceContext()->Unmap(m_pStagingTexture.Get(), 0);
//	return true;
//}
//
//PIXEL CPixelCollider::GetPixelColor(int x, int y)
//{
//	// �߽� ��ǥ ���
//	int centerX = m_iWidth / 2;
//	int centerY = m_iHeight / 2;
//
//	// �Է� ��ǥ�� �߽� ���� ��ǥ�� ��ȯ
//	int localX = centerX + x;       // �߽� ���� X ��ȯ
//	int localY = centerY - y;       // �߽� ���� Y ��ȯ (���� ����)
//
//	// 1. ��ǥ ��ȿ�� �˻�
//	if (localX < 0 || localX >= m_iWidth || localY < 0 || localY >= m_iHeight)
//	{
//		return { 0, 0, 0, 0 }; // �߸��� ��ǥ�� ���� ���� ������ ��ȯ
//	}
//
//	/*// 2. �ؽ�ó �����͸� Staging �ؽ�ó�� ����
//	ID3D11DeviceContext* pContext = GetDeviceContext();
//	pContext->CopyResource(m_pStagingTexture.Get(), m_pTexture->GetTexture().Get());
//
//	// 3. Staging �ؽ�ó�� Map
//	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
//	HRESULT hr = pContext->Map(m_pStagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
//	if (FAILED(hr))
//	{
//		OutputDebugStringA("Failed to map staging texture in GetPixelColor.\n");
//		return { 0, 0, 0, 0 };
//	}
//
//	// 4. �ȼ� ������ �б�
//	BYTE* pData = reinterpret_cast<BYTE*>(mappedResource.pData);
//	UINT rowPitch = mappedResource.RowPitch;
//
//	BYTE* pixelData = pData + (localY * rowPitch) + (localX * 4); // RGBA (4����Ʈ)
//	PIXEL pixel = { pixelData[0], pixelData[1], pixelData[2], pixelData[3] }; // RGBA ����
//
//	pContext->Unmap(m_pStagingTexture.Get(), 0);
//	return pixel;*/
//	return m_vecPixel[localY * m_iWidth + localX];
//}
//
//bool CPixelCollider::IsColliding(int worldX, int worldY)
//{
//	// ���� ��ǥ -> ���� ��ǥ ��ȯ
//	int localX = worldX - m_iOriginX;
//	int localY = worldY - m_iOriginY;
//
//	// �ؽ�ó ��ǥ ���� Ȯ��
//	if (localX < 0 || localX >= m_iWidth || localY < 0 || localY >= m_iHeight) {
//		return false; // �浹 ����
//	}
//
//	// �ȼ� ������ Ȯ��
//	PIXEL pixel = m_vecPixel[localY * m_iWidth + localX];
//	if (pixel.a > 0) { // ������ 0���� ũ�� �浹
//		return true;
//	}
//	return false;
//}
//
//bool CPixelCollider::IsPixelColliding(const Vector3& worldPos)
//{
//	int localX = static_cast<int>(worldPos.x) + m_iWidth / 2;
//	int localY = m_iHeight / 2 - static_cast<int>(worldPos.y);
//
//	if (localX < 0 || localX >= m_iWidth || localY < 0 || localY >= m_iHeight)
//		return false;
//
//	PIXEL pixel = m_vecPixel[localY * m_iWidth + localX];
//
//	// ���⼭ �������� �ٴ� ���� �Ǵ�
//	if ((pixel.r == GROUND_COLOR.R && pixel.g == GROUND_COLOR.G && pixel.b == GROUND_COLOR.B) ||
//		(pixel.r == FOOTHOLD_COLOR.R && pixel.g == FOOTHOLD_COLOR.G && pixel.b == FOOTHOLD_COLOR.B)) {
//		return true;
//	}
//
//	return false;
//
//
//	//// ���� ���Ǵ� ������ ĳ��
//	//static int lastCenterX = -1, lastCenterY = -1;
//	//if (lastCenterX != m_iWidth / 2) {
//	//	lastCenterX = m_iWidth / 2;
//	//	lastCenterY = m_iHeight / 2;
//	//}
//
//	//int localX = static_cast<int>(worldPos.x) + lastCenterX;
//	//int localY = lastCenterY - static_cast<int>(worldPos.y);
//
//	//// ���� ���� �˻�
//	//if (static_cast<unsigned int>(localX) >= static_cast<unsigned int>(m_iWidth) ||
//	//	static_cast<unsigned int>(localY) >= static_cast<unsigned int>(m_iHeight)) {
//	//	return false;
//	//}
//
//	//// ���� �迭 ���� (�������� ������ �����)
//	//return m_vecPixel[localY * m_iWidth + localX].a > 0;
//}
//
//void CPixelCollider::Init()
//{
//}
//
//void CPixelCollider::Update()
//{
//}
//
//void CPixelCollider::LateUpdate()
//{
//}
//
//void CPixelCollider::Render(const Matrix& view, const Matrix& projection)
//{
//}
//
//void CPixelCollider::OnCollisionEnter(CCollider* _pOther)
//{
//}
//
//void CPixelCollider::OnCollisionStay(CCollider* _pOther)
//{
//}
//
//void CPixelCollider::OnCollisionExit(CCollider* _pOther)
//{
//}