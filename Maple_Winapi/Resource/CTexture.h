#pragma once
#include <DirectXTex.h>
#include <DirectXTex.inl>
#include <DirectXTexEXR.h>

#include "CResource.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

class CTexture : public CResource
{
public:
    struct TextureSize
    {
        UINT width;
        UINT height;
    };

    CTexture();
    CTexture(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
    virtual ~CTexture();

    virtual HRESULT Load(const wstring& _strPath) override;
    void Bind(SHADER_STAGE _eShaderStage, UINT startSlot);

    TextureSize GetTextureSize() const { return m_tTextureSize; }
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }

    const wstring& GetFilePath() const { return m_strFilePath; }
    void SetFilePath(const wstring& _filePath) { m_strFilePath = _filePath; }

    bool GetDesc(D3D11_TEXTURE2D_DESC& desc) const;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture() const { return m_Texture; }

    void SetResource(Microsoft::WRL::ComPtr<ID3D11Resource> resource) { m_Resource = resource;  }
    Microsoft::WRL::ComPtr<ID3D11Resource> GetResource() const { return m_Resource; }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const { return m_SRV; }

    UINT GetWidth() { return m_iWidth; }
    UINT GetHeight() { return m_iHeight; }
    vector<PIXEL> GetPixelData() const { return m_vecPixel; }

private:
    vector<PIXEL>   m_vecPixel;
    UINT            m_iWidth;
    UINT            m_iHeight;

    TextureSize     m_tTextureSize;
    ScratchImage    m_Image;

    D3D11_TEXTURE2D_DESC                                m_Desc;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_Texture;
    Microsoft::WRL::ComPtr<ID3D11Resource>              m_Resource;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_SRV;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_RTV;

    wstring m_strFilePath; // 텍스처 파일 경로
};