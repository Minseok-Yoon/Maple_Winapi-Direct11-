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
		m_Resource = m_Texture; // Texture�� Resource�� ����
	}
}

CTexture::~CTexture()
{
}

HRESULT CTexture::Load(const wstring& _strPath)
{
	m_strFilePath = _strPath;

	// ���� ��ΰ� ��ȿ���� Ȯ��
	std::filesystem::path filePath(_strPath);

	// ������ �����ϴ��� Ȯ��
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
			return hr; // ���� �� ��ȯ
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

bool CTexture::GetDesc(D3D11_TEXTURE2D_DESC& desc) const
{
	if (!m_Texture) {
		return false;
	}

	m_Texture->GetDesc(&desc);
	return true;
}