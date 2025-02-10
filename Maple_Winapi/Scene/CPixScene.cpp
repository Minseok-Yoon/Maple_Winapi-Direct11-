#include "../pch.h"
#include "CPixScene.h"
#include "../Object/CGround.h"
#include "../Object/CObject.h"
#include "../Component/CPixelCollider.h"
#include "../Graphics/CGraphicsDevice_DX11.h"
#include "../Component/CRenderer.h"
#include "../Component/CCameraScript.h"
#include "../Object/CPlayer.h"
#include "../Object/CPlayerScript.h"
#include "../Component/CAnimator.h"
#include "../Component/CRigidBody.h"
#include "../Manager/CCollisionManager.h"
#include "../Core/CCore.h"

extern CCore core;

CPixScene::CPixScene()
{
	CTexture* player = CResourceManager::Load<CTexture>(L"PLAYER", L"../Resources/Texture/Player/Player.bmp");
}

CPixScene::~CPixScene()
{
}

void CPixScene::Enter()
{
	CScene::Enter();
}

void CPixScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CPixScene::Exit()
{
	CScene::Exit();
}

void CPixScene::Init()
{
	CScene::Init();

    // 플레이어 생성
    CGameObject* pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
    CTransform* plTr = pPlayer->GetComponent<CTransform>();
    plTr->SetPosition(Vector3(0.0f, 0.0f, -1.0f));

    // 플레이어 텍스처 설정
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");
    CTransform* playerTransform = pPlayer->GetComponent<CTransform>();
    playerTransform->SetScale(Vector3(54.0f, 65.0f, 1.0f));
    CSpriteRenderer* sr = pPlayer->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);
	renderer::selectedObject = pPlayer;
}

void CPixScene::Update()
{
	CScene::Update();
}

void CPixScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CPixScene::Render()
{
	CScene::Render();
}