#include "../pch.h"
#include "CGround.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CCollisionManager.h"
#include "../Component/CCamera.h"
#include "../Resource/CResource.h"
#include "../Resource/CTexture.h"
#include "../Core/CCore.h"
#include "../Component/CPixelCollider.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CPixelCollider.h"
#include "../Component/CTransform.h"
#include "../Manager/CSceneManager.h"

CBackGround::CBackGround() :
    m_pBackGroundTexture(nullptr),
    m_pPixBackGround(nullptr)
{
}

CBackGround::~CBackGround()
{
}

void CBackGround::Init()
{
    CGameObject::Init();

    m_pPixBackGround = new CPixelCollider();

    const wchar_t* currentSceneName = CSceneManager::GetCurrentSceneName();

    if (wcscmp(currentSceneName, L"DrawRectScene") == 0)
    {
        if (!m_pPixBackGround->SetPixelInfo(L"../Resources/Texture/MainPixel.png"))
        {
            OutputDebugStringA("Failed to set pixel collider for Stage 01.\n");
        }
    }
}

void CBackGround::Update()
{
    CGameObject::Update();
}

void CBackGround::LateUpdate()
{
    CGameObject::LateUpdate();
}

void CBackGround::Render()
{
    CGameObject::Render();
}

void CBackGround::CreateTextureFromPixel()
{
    if (!m_pPixBackGround) return;

    const std::vector<PIXEL>& pixelData = m_pPixBackGround->GetPixel();

    if (pixelData.empty())
    {
        OutputDebugStringA("Pixel data is empty.\n");
    }
    else
    {
        for (size_t i = 0; i < 10 && i < pixelData.size(); ++i) {
            const PIXEL& p = pixelData[i];
            char buf[128];
            sprintf_s(buf, "Pixel[%zu]: R=%d, G=%d, B=%d, A=%d\n", i, p.r, p.g, p.b, p.a);
            OutputDebugStringA(buf);
        }
    }

    int width = m_pPixBackGround->GetWidth();
    int height = m_pPixBackGround->GetHeight();
    
    // DirectX 咆胶贸 积己
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    
    std::vector<uint32_t> rawData(width * height);
    for (int i = 0; i < pixelData.size(); ++i) {
        const PIXEL& p = pixelData[i];
        rawData[i] = (p.a << 24) | (p.r << 16) | (p.g << 8) | p.b;
        if (i < 10) {
            char buf[128];
            sprintf_s(buf, "RawData[%d]: 0x%08X (A=%d, R=%d, G=%d, B=%d)\n", i, rawData[i], p.a, p.r, p.g, p.b);
            OutputDebugStringA(buf);
        }
    }
    
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = rawData.data();
    initData.SysMemPitch = width * sizeof(uint32_t);
    
    char buffer[256];
    sprintf_s(buffer, "Width: %d, Height: %d, Pitch: %d\n", texDesc.Width, texDesc.Height, initData.SysMemPitch);
    OutputDebugStringA(buffer);

    for (size_t i = 0; i < 10 && i < rawData.size(); ++i) {
        sprintf_s(buffer, "rawData[%zu] = 0x%08X\n", i, rawData[i]);
        OutputDebugStringA(buffer);
    }

    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = GetDevice()->CreateTexture2D(&texDesc, &initData, &texture);
    if (FAILED(hr)) {
        OutputDebugStringA("Failed to create background texture.\n");
        return;
    }
    
    // Shader Resource View 积己
    ID3D11ShaderResourceView* srv = nullptr;
    hr = GetDevice()->CreateShaderResourceView(texture, nullptr, &srv);
    if (FAILED(hr)) {
        OutputDebugStringA("Failed to create shader resource view for background texture.\n");
    }
    
    m_pBackGroundTexture = new CTexture(texture, srv);
    texture->Release();

    // 积己等 咆胶贸甫 CSpriteRenderer俊 汲沥
    CSpriteRenderer* sr = GetComponent<CSpriteRenderer>();
    if (!sr) {
        sr = AddComponent<CSpriteRenderer>();
    }
    sr->SetTexture(m_pBackGroundTexture);
}

void CBackGround::SetBackGroundTexture(CTexture* _pBackGroundTexture)
{
    m_pBackGroundTexture = _pBackGroundTexture;
    if (m_pBackGroundTexture != nullptr)
    {
        CSpriteRenderer* sr = GetComponent<CSpriteRenderer>();
        if (sr == nullptr)
        {
            sr = AddComponent<CSpriteRenderer>();
        }
        sr->SetTexture(m_pBackGroundTexture);
    }
}
