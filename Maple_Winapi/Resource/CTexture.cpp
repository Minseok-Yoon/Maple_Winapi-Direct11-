#include "CTexture.h"
#include "../Core/CCore.h"
#include "../Manager/CResourceManager.h"

extern CCore core;

CTexture::CTexture() :
	CResource(RESOURCE_TYPE::RT_Texture),
	m_Desc{}
{
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Load(const wstring& _strPath)
{
	m_strFilePath = _strPath;

#pragma region �ؽ�ó
	//wstring ext = _strPath.substr(_strPath.find_last_of(L".") + 1);

	//if (ext == L"bmp")
	//{
	//	m_eTextureState = TEXTURE_STATE::TS_Bmp;

	//	// ���Ϸ� ���� �ε��� �����͸� ��Ʈ������ ����
	//	m_hBitmap = (HBITMAP)LoadImageW(nullptr, _strPath.c_str(), IMAGE_BITMAP,
	//		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//	if (m_hBitmap == nullptr)
	//	{
	//		DWORD error = GetLastError();
	//		// ���� �޽����� ���ڿ��� ��ȯ
	//		std::wstringstream ss;
	//		ss << L"LoadImage failed with error code: " << error << std::endl;

	//		// ��ȯ�� ���ڿ��� ���â�� ���
	//		OutputDebugString(ss.str().c_str());

	//		return S_FALSE;
	//	}

	//	// ��Ʈ�� ����
	//	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitInfo);

	//	m_iWidth = CTexture::GetWidth();
	//	m_iHeight = CTexture::GetHeight();

	//	// ��Ʈ�ʰ� ������ DC
	//	m_Hdc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	//	// ��Ʈ�ʰ� DC ����
	//	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_Hdc, m_hBitmap);
	//	DeleteObject(hPrevBit);
	//}
	//else if (ext == L"png")
	//{
	//	m_eTextureState = TEXTURE_STATE::TS_Png;

	//	m_pImage = Gdiplus::Image::FromFile(_strPath.c_str());
	//	if (m_pImage == nullptr)
	//		return S_FALSE;

	//	m_iWidth = m_pImage->GetWidth();
	//	m_iHeight = m_pImage->GetHeight();
	//}
#pragma endregion
	// ���� ��ΰ� ��ȿ���� Ȯ��
	std::filesystem::path filePath(_strPath);

	wstringstream debugStream;
	debugStream << L"Checking file path: " << _strPath << L"\n";
	OutputDebugStringW(debugStream.str().c_str());

	// ������ �����ϴ��� Ȯ��
	if (!std::filesystem::exists(filePath))
	{
		std::wstringstream ss;
		ss << L"File does not exist: " << _strPath << "\n";
		OutputDebugStringW(ss.str().c_str());
		return S_FALSE;
	}
	else
	{
		std::wstringstream ss;
		ss << L"File exist: " << _strPath << "\n";
		OutputDebugStringW(ss.str().c_str());
	}

	wstring ext
		= _strPath.substr(_strPath.find_last_of(L".") + 1);

	if (ext == L".dds" || ext == L".DDS")
	{
		if(FAILED(LoadFromDDSFile(_strPath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image)))
			return S_FALSE;
	}
	else if (ext == L".tga" || ext == L".TGA")
	{
		if(FAILED(LoadFromTGAFile(_strPath.c_str(), nullptr, m_Image)))
			return S_FALSE;
	}
	else // WIC (png, jpg, jpeg, bmp)
	{
		HRESULT hr = LoadFromWICFile(_strPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
		if(FAILED(hr))
		{
			std::wstringstream ss;
			ss << L"Failed to load WIC file: " << _strPath << L", HRESULT: " << std::hex << hr << L"\n";
			OutputDebugStringW(ss.str().c_str());
			return hr; // ���� �� ��ȯ
		}

		/*if (FAILED(LoadFromWICFile(_strPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image)))
			return S_FALSE;*/
	}

	HRESULT hr = CreateShaderResourceView
	(
		GetDevice()->GetID3D11Device().Get(),
		m_Image.GetImages(),
		m_Image.GetImageCount(),
		m_Image.GetMetadata(),
		m_SRV.GetAddressOf()
	);

	if (hr == S_FALSE)
		assert(false/*"Texture load Failed!*/);

	m_SRV->GetResource(reinterpret_cast<ID3D11Resource**>(m_Texture.GetAddressOf()));

	// ��Ÿ�����͸� ����Ͽ� �ؽ�ó ũ�� ����
	TexMetadata metadata = m_Image.GetMetadata();
	m_tTextureSize.width = static_cast<UINT>(metadata.width);
	m_tTextureSize.height = static_cast<UINT>(metadata.height);

	// Ŭ���̾�Ʈ�� ũ��
	return S_OK;
}

void CTexture::Bind(SHADER_STAGE _eShaderStage, UINT startSlot)
{
	GetDevice()->SetShaderResource(_eShaderStage, startSlot, m_SRV.GetAddressOf());
}

//CTexture* CTexture::Create(const wstring& _strName, UINT _iWidth, UINT _iHeight)
//{
//	CTexture* image = CResourceManager::Find<CTexture>(_strName);
//	if (image)
//		return image;
//
//	image = new CTexture();
//	image->SetName(_strName);
//	image->SetWidth(_iWidth);
//	image->SetHeight(_iWidth);
//
//	HDC mainhDC = CCore::GetInst()->GetMainDC();
//	HWND mainhWnd = CCore::GetInst()->GetMainHWnd();
//
//	image->m_hBitmap = CreateCompatibleBitmap(mainhDC, _iWidth, _iHeight);
//	image->m_Hdc = CreateCompatibleDC(mainhDC);
//
//	HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
//	HBRUSH oldBrush = (HBRUSH)SelectObject(mainhDC, transparentBrush);
//	math::Rectangle(image->m_Hdc, -1, -1, image->GetWidth() + 1, image->GetHeight() + 1);
//	SelectObject(mainhDC, oldBrush);
//
//	HBITMAP oldBitmap = (HBITMAP)SelectObject(image->m_Hdc, image->m_hBitmap);
//	DeleteObject(oldBitmap);
//
//	// ��Ʈ�� ����
//	CResourceManager::Insert(_strName + L"Image", image);
//
//	return image;
//}