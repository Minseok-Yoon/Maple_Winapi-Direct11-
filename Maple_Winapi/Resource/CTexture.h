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
    virtual ~CTexture();

    virtual HRESULT Load(const wstring& _strPath) override;
    void Bind(SHADER_STAGE _eShaderStage, UINT startSlot);

    TextureSize GetTextureSize() const { return m_tTextureSize; }
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }

private:
    TextureSize     m_tTextureSize;
    ScratchImage    m_Image;

    D3D11_TEXTURE2D_DESC                                m_Desc;
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_Texture;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_SRV;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_RTV;
};