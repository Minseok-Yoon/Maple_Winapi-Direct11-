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

CBackGround::CBackGround()
{
}

CBackGround::~CBackGround()
{
}

void CBackGround::Init()
{
    CGameObject::Init();

    /*switch (eSceneType)
    {
    case SCENE_TYPE::ST_Stage_01:
        if (!m_pPixelCollider->SetPixelInfo(L"texture\\MainLoad.bmp"))
        {
            OutputDebugStringA("Failed to set pixel collider for Stage 01.\n");
        }
        break;

    case SCENE_TYPE::ST_Stage_02:
        if (!m_pPixelCollider->SetPixelInfo(L"texture\\Stage1Load.bmp"))
        {
            OutputDebugStringA("Failed to set pixel collider for Stage 02.\n");
        }
        break;

    default:
        OutputDebugStringA("Unknown scene type.\n");
        break;
    }*/
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