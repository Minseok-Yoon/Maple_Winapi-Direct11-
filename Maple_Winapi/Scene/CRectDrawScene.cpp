#include "CRectDrawScene.h"
#include "../Object/CGameObject.h"
#include "../Component/CLineRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CSpriteRenderer.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"
#include "../Component/CTransform.h"
#include "../Object/CGround.h"
#include "../Core/CCore.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"
#include "../Component/CCameraScript.h"
#include "../Object/CPlayer.h"
#include "../Object/CPlayerScript.h"
#include "../Component/CAnimator.h"
#include "../Component/CRigidBody.h"
#include "../Manager/CCollisionManager.h"

extern CCore core;

CRectDrawScene::CRectDrawScene() :
    m_pLineRenderer(nullptr)
{
    //CResourceManager::Load<CTexture>(L"MainCol", L"../Resources/Texture/MainPixel.png");
	CTexture* player = CResourceManager::Load<CTexture>(L"PLAYER", L"../Resources/Texture/Player/Player.bmp");
}

CRectDrawScene::~CRectDrawScene()
{
    delete m_pLineRenderer;
}

void CRectDrawScene::Enter()
{
	CScene::Enter();
}

void CRectDrawScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CRectDrawScene::Exit()
{
	CScene::Exit();
}

void CRectDrawScene::Init()
{
    CScene::Init();

    CBackGround* bg = Instantiate<CBackGround>(LAYER_TYPE::LT_PixelBackGround);
    bg->Init();
    // 배경 텍스처 생성
    bg->CreateTextureFromPixel();
    CTransform* tr = bg->GetComponent<CTransform>();
    tr->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

    // 카메라 설정
    CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
    CCamera* cameraComp = camera->AddComponent<CCamera>();
    cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

    CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
    renderer::mainCamera = cameraComp;

    // 플레이어 생성
    CGameObject* pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
    CTransform* plTr = pPlayer->GetComponent<CTransform>();
    plTr->SetPosition(Vector3(0.0f, 0.0f, -1.0f));
    DontDestroyOnLoad(pPlayer);

    // 플레이어 텍스처 설정
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");
    CTransform* playerTransform = pPlayer->GetComponent<CTransform>();
    playerTransform->SetScale(Vector3(54.0f, 65.0f, 1.0f));
    CSpriteRenderer* sr = pPlayer->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);
    CAnimator* animator = pPlayer->AddComponent<CAnimator>();
    animator->SetSpriteRenderer(sr);
    pPlayer->AddComponent<CPlayerScript>();

	CPlayerScript* plSr = pPlayer->GetComponent<CPlayerScript>();
	plSr->SetPixelCollider(bg->GetPixelCollider());

    pPlayer->AddComponent<CRigidBody>();

    pPlayer->AddComponent<CCollider>();

    renderer::selectedObject = pPlayer;

	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_BackGround, true);
}

void CRectDrawScene::Update()
{
	CScene::Update();
}

void CRectDrawScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CRectDrawScene::Render()
{
	CScene::Render();
    //// 색상 값 OutputDebugString으로 출력
    //char colorMessage[100];
    //sprintf_s(colorMessage, "Line Color: R=%.2f, G=%.2f, B=%.2f, A=%.2f\n", color.x, color.y, color.z, color.w);
    //OutputDebugStringA(colorMessage);  // 색상 값 출력
}