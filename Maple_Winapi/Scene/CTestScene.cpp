#include "../pch.h"
#include "CTestScene.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Object/CObject.h"
#include "../Object/CPlayer.h"
#include "../Object/CPlayerScript.h"
#include "../Component/CCamera.h"
#include "../Component/CCameraScript.h"
#include "../Component/CRenderer.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CRigidBody.h"
#include "../Component/CAnimator.h"
#include "../Core/CCore.h"

extern CCore core;

CTestScene::CTestScene()
{
	CResourceManager::Load<CTexture>(L"Stage02_BG", L"../Resources/Texture/Main.bmp");
	CTexture* player = CResourceManager::Load<CTexture>(L"PLAYER", L"../Resources/Texture/Player/Player.bmp");
}

CTestScene::~CTestScene()
{
}

void CTestScene::Enter()
{
	CScene::Enter();

	// 배경 객체 생성
	CBackGround* bg = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
	CTransform* bgTr = bg->GetComponent<CTransform>();
	bgTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	// 배경 텍스처 로드
	CTexture* bgTexture = CResourceManager::Find<CTexture>(L"Stage02_BG");
	if (bgTexture != nullptr)
	{
		CTexture::TextureSize textureSize = bgTexture->GetTextureSize();
		float textureWidth = static_cast<float>(textureSize.width);
		float textureHeight = static_cast<float>(textureSize.height);

		// 화면 해상도를 가져오기
		float resolutionWidth = core.GetWidth();
		float resolutionHeight = core.GetHeight();

		// 텍스처가 화면을 완전히 덮도록 비율 계산
		float scaleX = resolutionWidth / textureWidth;
		float scaleY = resolutionHeight / textureHeight;

		// 더 큰 비율로 설정하여 텍스처가 화면을 덮도록 조정
		float scale = scaleX > scaleY ? scaleX : scaleY;

		// 배경 크기를 스케일링
		bgTr->SetScale(Vector3(textureWidth, textureHeight, 1.0f));
		bg->SetBackGroundTexture(bgTexture);
	}

	// 배경 렌더러 설정
	CSpriteRenderer* bgSr = bg->AddComponent<CSpriteRenderer>();
	bgSr->SetTexture(bgTexture);
}

void CTestScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CTestScene::Exit()
{
	CScene::Exit();
}

void CTestScene::Init()
{
    CScene::Init();

    // 카메라 설정
    CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
    CCamera* cameraComp = camera->AddComponent<CCamera>();
    cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

    CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
    renderer::mainCamera = cameraComp;

    // 플레이어 생성
    CGameObject* pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
    DontDestroyOnLoad(pPlayer);
    //cameraComp->SetTarget(pPlayer);

    // 플레이어 텍스처 로드
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");

    // 텍스처 크기 가져오기
    CTexture::TextureSize playerTextureSize = playerTexture->GetTextureSize();
    float textureWidth = static_cast<float>(playerTextureSize.width);
    float textureHeight = static_cast<float>(playerTextureSize.height);

    // 화면 해상도 가져오기
    float resolutionWidth = core.GetWidth();
    float resolutionHeight = core.GetHeight();

    // 텍스처가 화면을 완전히 덮도록 비율 계산
    float scaleX = resolutionWidth / textureWidth;
    float scaleY = resolutionHeight / textureHeight;

    // 더 큰 비율로 설정하여 텍스처가 화면을 덮도록 조정
    float scale = scaleX > scaleY ? scaleX : scaleY;

    // 플레이어의 크기를 원본 크기인 54x65로 설정
    CTransform* playerTransform = pPlayer->GetComponent<CTransform>();
    playerTransform->SetScale(Vector3(54.0f, 65.0f, 1.0f));

    // SpriteRenderer와 애니메이터 컴포넌트 추가
    CSpriteRenderer* sr = pPlayer->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);
    CAnimator* animator = pPlayer->AddComponent<CAnimator>();
    animator->SetSpriteRenderer(sr);

    // 플레이어에 CPlayerScript 컴포넌트 추가
    CPlayerScript* playerScript = pPlayer->AddComponent<CPlayerScript>();

    pPlayer->AddComponent<CRigidBody>();

    renderer::selectedObject = pPlayer;
}

void CTestScene::Update()
{
	CScene::Update();
}

void CTestScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CTestScene::Render()
{
	CScene::Render();
}