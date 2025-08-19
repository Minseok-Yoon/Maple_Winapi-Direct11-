#include "../pch.h"
#include "CResourceManager.h"
#include "CPathManager.h"
#include "../Resource/CTexture.h"
#include "../Resource/CResource.h"

map<wstring, CResource*> CResourceManager::m_mapResources = {};

// 2025-06-12 Dummy Texture(1x1 RGBA(0, 0, 0, 0) 추가
void CResourceManager::Init()
{
    // 1x1 완전 투명한 텍스처 생성
    UINT32 transparentPixel = 0x80808080; // RGBA: (0,0,0,0)

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = 1;
    desc.Height = 1;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = &transparentPixel;
    initData.SysMemPitch = sizeof(UINT32);
    initData.SysMemSlicePitch = 0;

    ID3D11Texture2D* tex = nullptr;
    HRESULT hr = GetDevice()->CreateTexture2D(&desc, &initData, &tex);
    if (FAILED(hr))
    {
        OutputDebugStringW(L"DummyTexture 생성 실패!\n");
        return;
    }

    ID3D11ShaderResourceView* srv = nullptr;
    hr = GetDevice()->CreateShaderResourceView(tex, nullptr, &srv);
    if (FAILED(hr))
    {
        OutputDebugStringW(L"DummyTexture SRV 생성 실패!\n");
        tex->Release();
        return;
    }

    tex->Release();

    CTexture* dummyTex = new CTexture;
    dummyTex->SetSRV(srv);
    dummyTex->SetName(L"DummyTexture");

    Insert(L"DummyTexture", dummyTex);

    OutputDebugStringW(L"DummyTexture 생성 완료!\n");
}

void CResourceManager::Insert(const wstring& _strKey, CResource* _pResource)
{
    if (_strKey == L"")
        return;

    if (_pResource == nullptr)
        return;

    m_mapResources.insert(make_pair(_strKey, _pResource));
}

void CResourceManager::Release()
{
    for (auto& iter : m_mapResources)
    {
        delete iter.second;
        iter.second = nullptr;
    }
}