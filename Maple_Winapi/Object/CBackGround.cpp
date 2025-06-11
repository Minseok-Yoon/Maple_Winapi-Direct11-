#include "../pch.h"
#include "CBackGround.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CColliderManager.h"
#include "../Component/CCamera.h"
#include "../Resource/CResource.h"
#include "../Resource/CTexture.h"
#include "../Core/CCore.h"
#include "../Component/CPixelCollider.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CPixelCollider.h"
#include "../Component/CTransform.h"
#include "../Manager/CSceneManager.h"
#include "../Object/CObject.h"

extern CCore core;

CBackGround::CBackGround() :
    m_strMapName(L""),
    m_pMap(nullptr),
    m_pMapCollision(nullptr),
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
}

void CBackGround::Update()
{
    CGameObject::Update();
}

void CBackGround::LateUpdate()
{
    CGameObject::LateUpdate();
}

void CBackGround::Render(const Matrix& view, const Matrix& projection)
{
    CGameObject::Render(view, projection);
}

bool CBackGround::CheckGround(Vector3 _vPlusCheckPos)
{
    bool IsGround = false;

    CScene* pCurScene = CSceneManager::GetCurScene();
    if (!pCurScene) return false;

    CBackGround* pCurBackGround = pCurScene->GetBackGround();
    if (!pCurBackGround) return false;

    // 현재 위치의 색상 확인
    TextureColor CheckColor = pCurBackGround->GetColor(m_pTransform->GetWorldPosition() + _vPlusCheckPos, TextureColor(0, 0, 0, 255));

    // 바닥 여부 판별
    if (CheckColor == GROUND_COLOR)
    {
        IsGround = true;
    }

    if (CheckColor == FOOTHOLD_COLOR)
    {
        IsGround = true;
    }

    return IsGround;
}

TextureColor CBackGround::GetPixelColor(const Vector3& _vWorldPos)
{
    if (!m_pMapCollision)
    {
        OutputDebugStringA("ERROR: CBackGround::GetPixelColor() - m_pMapCollision is NULL!\n");
        return TextureColor(255, 0, 255, 255); // Magenta (에러용)
    }

    return GetColor(_vWorldPos, TextureColor(255, 0, 255, 255)); // 기본값은 Magenta
}

TextureColor CBackGround::GetColor(Vector3 _Pos, TextureColor _DefaultColor)
{
    if (!m_pMapCollision)
    {
        OutputDebugStringA("ERROR: CBackGround::GetColor() - m_pMapCollision is NULL!\n");
        return _DefaultColor;
    }

    // 텍스처 크기 가져오기
    CTexture::TextureSize textureSize = m_pMapCollision->GetTextureSize();
    UINT texWidth = textureSize.width;
    UINT texHeight = textureSize.height;

    // 🟢 올바른 좌표 변환 적용
    int pixelX = static_cast<int>(_Pos.x);
    int pixelY = static_cast<int>(_Pos.y); // Y축 반전

    return m_pMapCollision->GetColor(Vector3(pixelX, pixelY, 0.0f), _DefaultColor);
}

void CBackGround::CreateMap(wstring _MapName)
{
    SetName(_MapName);
    m_strMapName = _MapName;

    UINT iWidth = core.GetWidth();
    UINT iHeight = core.GetHeight();

    CTransform* bgTr = GetComponent<CTransform>();
    bgTr->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

    m_pMap = CResourceManager::Find<CTexture>(_MapName);

    // 스프라이트 렌더러 설정
    CSpriteRenderer* sr = AddComponent<CSpriteRenderer>();
    sr->SetTexture(m_pMap);

    if (nullptr != m_pMap)
    {
        // 텍스처 크기 설정하기
        CTexture::TextureSize textureSize = m_pMap->GetTextureSize();
        if (textureSize.width <= iWidth || textureSize.height <= iHeight)
        {
            bgTr->SetLocalScale(Vector3(iWidth, iHeight, 0.0f));
        }
        else
        {
            bgTr->SetLocalScale(Vector3(textureSize.width, textureSize.height, 0.0f));
        }
    }
}

void CBackGround::CreateCollisionMap(wstring _MapCollisionName)
{
    SetName(_MapCollisionName);
    m_strMapName = _MapCollisionName;

    UINT iWidth = core.GetWidth();
    UINT iHeight = core.GetHeight();

    CTransform* bgTr = GetComponent<CTransform>();
    bgTr->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

    m_pMapCollision = CResourceManager::Find<CTexture>(_MapCollisionName);

    // 스프라이트 렌더러 설정
    CSpriteRenderer* sr = AddComponent<CSpriteRenderer>();
    sr->SetTexture(m_pMapCollision);

    if (nullptr != m_pMapCollision)
    {
        // 텍스처 크기 설정하기
        CTexture::TextureSize textureSize = m_pMapCollision->GetTextureSize();
        if (textureSize.width <= iWidth || textureSize.height <= iHeight)
        {
            bgTr->SetLocalScale(Vector3(iWidth, iHeight, 0.0f));
        }
        else
        {
            bgTr->SetLocalScale(Vector3(textureSize.width, textureSize.height, 0.0f));
        }
    }
}


bool CBackGround::CheckCollision(int playerX, int playerY)
{
    if (!m_pPixBackGround) {
        return false; // PixelCollider가 초기화되지 않았으면 충돌 없음
    }

    return m_pPixBackGround->IsColliding(playerX, playerY);
}

void CBackGround::CreateTextureFromPixel()
{
    if (!m_pPixBackGround) return;

    const std::vector<PIXEL>& pixelData = m_pPixBackGround->GetPixelData();

    if (pixelData.empty())
    {
        OutputDebugStringA("Pixel data is empty.\n");
    }
    else
    {
        for (size_t i = 1; i < 10 && i < pixelData.size(); ++i) {
            const PIXEL& p = pixelData[i];
            char buf[128];
            sprintf_s(buf, "Pixel[%zu]: R=%d, G=%d, B=%d, A=%d\n", i, p.r, p.g, p.b, p.a);
            OutputDebugStringA(buf);
        }
    }

    int width = m_pPixBackGround->GetWidth();
    int height = m_pPixBackGround->GetHeight();

    // DirectX 텍스처 생성
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
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

    // Shader Resource View 생성
    ID3D11ShaderResourceView* srv = nullptr;
    hr = GetDevice()->CreateShaderResourceView(texture, nullptr, &srv);
    if (FAILED(hr)) {
        OutputDebugStringA("Failed to create shader resource view for background texture.\n");
    }

    m_pBackGroundTexture = new CTexture(texture, srv);
    //texture->Release();

    // 생성된 텍스처를 CSpriteRenderer에 설정
    CSpriteRenderer* sr = GetComponent<CSpriteRenderer>();
    if (!sr) {
        sr = AddComponent<CSpriteRenderer>();
    }
    sr->SetTexture(m_pBackGroundTexture);

    // 배경을 화면의 중앙에 맞추기 위해 CTransform의 위치 수정
    CTransform* bgTr = GetComponent<CTransform>();
    bgTr->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f)); // 화면의 중앙에 배경 위치 설정

    // 텍스처의 크기를 스케일에 맞게 조정
    bgTr->SetLocalScale(Vector3(static_cast<float>(width), static_cast<float>(height), 1.0f));
}

/*void CBackGround::SetBackGroundTexture(CTexture* _pBackGroundTexture)
{
}

m_pPixBackGround = new CPixelCollider();

const wchar_t* currentSceneName = CSceneManager::GetCurrentSceneName();

if (wcscmp(currentSceneName, L"DrawRectScene") == 0)
{
    if (!m_pPixBackGround->SetPixelInfo(L"../Resources/Texture/MainPixel.png"))
    {
        OutputDebugStringA("Failed to set pixel collider for Stage 01.\n");
    }
}

// 배경 텍스처가 로드된 후에 위치 설정
CTexture* bgTexture = GetBackGroundTexture();  // 텍스처 객체를 가져온다고 가정
if (bgTexture != nullptr)
{
    CTexture::TextureSize textureSize = bgTexture->GetTextureSize();
    float textureWidth = static_cast<float>(textureSize.width);
    float textureHeight = static_cast<float>(textureSize.height);

    // 화면 해상도 구하기
    float resolutionWidth = core.GetWidth();
    float resolutionHeight = core.GetHeight();

    // 텍스처의 중심을 화면 중앙으로 이동
    CTransform* bgTransform = GetComponent<CTransform>();
    bgTransform->SetLocalScale(Vector3(0.0f, 0.0f, 0.0f));

    // 텍스처 크기 조정 (화면을 덮도록 크기 조정)
    float scaleX = resolutionWidth / textureWidth;
    float scaleY = resolutionHeight / textureHeight;
    float scale = scaleX > scaleY ? scaleX : scaleY;
    bgTransform->SetLocalScale(Vector3(textureWidth * scale, textureHeight * scale, 1.0f));
}*/

// 디버깅 출력
    /*std::string debugMsgs = "🔍 Checking color at (" + std::to_string(pixelX) + ", " + std::to_string(pixelY) + ")\n";
    OutputDebugStringA(debugMsgs.c_str());*/

    //if (pixelX < 0 || pixelX >= texWidth || pixelY < 0 || pixelY >= texHeight)
    //{
    //    std::string errorMsg = "⚠️ ERROR: GetColor() - Out of bounds: (" +
    //        std::to_string(pixelX) + ", " + std::to_string(pixelY) + ")\n";
    //    OutputDebugStringA(errorMsg.c_str());

    //    // 🟢 추가: 현재 텍스처 크기 출력
    //    std::string textureSizeMsg = "🟢 Texture Size: (" + std::to_string(texWidth) + ", " + std::to_string(texHeight) + ")\n";
    //    OutputDebugStringA(textureSizeMsg.c_str());

    //    return _DefaultColor;
    //}

/*string debugSize = "🟢 Texture Size: (" + std::to_string(texWidth) + ", " + std::to_string(texHeight) + ")\n";
    OutputDebugStringA(debugSize.c_str());

    string debugMsg = "🟢 Input World Pos: (" + std::to_string(_Pos.x) + ", " + std::to_string(_Pos.y) + ")\n";
    OutputDebugStringA(debugMsg.c_str());*/

    /*OutputDebugStringA(("Checking ground collision at: (" +
            std::to_string(_vPlusCheckPos.x) + ", " +
            std::to_string(_vPlusCheckPos.y) + ")\n").c_str());*/


            //// 마젠타 색상과 충돌 체크
            //if (CheckColor == TextureColor(255, 0, 255, 255))
            //{
            //    OutputDebugStringA("⚠️ WARNING: Player touched MAGENTA pixel! ⚠️\n");
            //}