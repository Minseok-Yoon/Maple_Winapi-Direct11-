#include "CTexture.h"
#include "../Core/CCore.h"
#include "../Manager/CResourceManager.h"

extern CCore core;

CTexture::CTexture() :
	CResource(RESOURCE_TYPE::RT_Texture),
	m_iWidth(0),
	m_iHeight(0),
	m_Desc{}
{
}

CTexture::CTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv) : 
	m_Texture(texture),
	m_SRV(srv)
{
	if (texture)
	{
		texture->GetDesc(&m_Desc);
		m_iWidth = m_Desc.Width;
		m_iHeight = m_Desc.Height;
		m_tTextureSize = { m_Desc.Width, m_Desc.Height };
		m_Resource = m_Texture; // Texture를 Resource로 설정
	}
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Load(const wstring& _strPath)
{
	m_strFilePath = _strPath;

	// 파일 경로가 유효한지 확인
	std::filesystem::path filePath(_strPath);

	// 파일이 존재하는지 확인
	if (!std::filesystem::exists(filePath))
	{
		return S_FALSE;
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
			return hr; // 실패 시 반환
		}
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

	if (m_Texture) {
		m_Texture->GetDesc(&m_Desc);
	}

	// 메타데이터를 사용하여 텍스처 크기 저장
	TexMetadata metadata = m_Image.GetMetadata();
	m_tTextureSize.width = static_cast<UINT>(metadata.width);
	m_tTextureSize.height = static_cast<UINT>(metadata.height);

	m_iWidth = m_tTextureSize.width;
	m_iHeight = m_tTextureSize.height;

	// 클라이언트의 크기
	return S_OK;
}

void CTexture::Bind(SHADER_STAGE _eShaderStage, UINT startSlot)
{
	GetDevice()->SetShaderResource(_eShaderStage, startSlot, m_SRV.GetAddressOf());
}

void CTexture::CreateTexture(wstring _strName)
{
}

bool CTexture::GetDesc(D3D11_TEXTURE2D_DESC& desc) const
{
	if (!m_Texture) {
		return false;
	}

	m_Texture->GetDesc(&desc);
	return true;
}

TextureColor CTexture::GetColor(int _x, int _y, TextureColor _DefaultColor)
{
	// GetScale() 값을 한번만 저장 (불필요한 함수 호출 방지)
	Vector3 scale = GetScale();
	int texWidth = static_cast<int>(scale.x);
	int texHeight = static_cast<int>(scale.y);

	// 중앙 기준 좌표를 텍스처 좌표로 변환
	int texX = _x + (texWidth / 2);
	int texY = (_y * -1) + (texHeight / 2); // Y축 반전

	//// 디버깅 로그 추가
	//std::string debugMsg = "🔍 Texture Size: (" + std::to_string(texWidth) + ", " + std::to_string(texHeight) + ")\n";
	//debugMsg += "🎯 Converted Coordinates: (" + std::to_string(texX) + ", " + std::to_string(texY) + ")\n";
	//OutputDebugStringA(debugMsg.c_str());

	// 변환된 좌표의 범위 검사
	if (texX < 0 || texY < 0 || texX >= texWidth || texY >= texHeight)
	{
		//OutputDebugStringA(("⚠️ GetColor() - Converted Out of bounds: (" + std::to_string(texX) + ", " + std::to_string(texY) + ")\n").c_str());
		return _DefaultColor;
	}

	// 픽셀 데이터 포인터 가져오기
	unsigned char* Ptr = m_Image.GetPixels();
	if (!Ptr)
	{
		//OutputDebugStringA("❌ GetColor() - Pixel data is null!\n");
		return _DefaultColor;
	}

	// 텍스처 형식 가져오기
	DXGI_FORMAT Fmt = m_Image.GetMetadata().format;

	// 픽셀 데이터의 위치 계산 (Stride 고려)
	unsigned char* pixelPtr = Ptr + ((texY * texWidth) + texX) * 4;

	// 포맷에 따라 색상 변환
	switch (Fmt)
	{
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		return { pixelPtr[2], pixelPtr[1], pixelPtr[0], pixelPtr[3] }; // BGR → RGB 변환

	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return { pixelPtr[0], pixelPtr[1], pixelPtr[2], pixelPtr[3] }; // 그대로 사용

	default:
		OutputDebugStringA(("⚠️ Unsupported texture format in GetColor(): " + std::to_string(Fmt) + "\n").c_str());
		break;
	}

	return _DefaultColor;
}
