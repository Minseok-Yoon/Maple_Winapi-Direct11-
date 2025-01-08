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

    if (currentSceneName == L"TestScene")
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
