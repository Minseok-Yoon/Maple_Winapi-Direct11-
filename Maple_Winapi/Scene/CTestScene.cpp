#include "../pch.h"
#include "CTestScene.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Object/CObject.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"
#include "../Object/CPlayerScript.h"
#include "../Component/CCamera.h"
#include "../Component/CCameraScript.h"
#include "../Component/CRenderer.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CRigidBody.h"
#include "../Component/CAnimator.h"
#include "../Core/CCore.h"
#include "../Component/CCollider.h"
#include "../Component/CAudioListener.h"
#include "../Object/CMiniMap.h"
#include "../Object/CPortal.h"
#include "../Object/CInventoryUI.h"

extern CCore core;

CTestScene::CTestScene()
{
	CResourceManager::Load<CTexture>(L"Lake of Oblivion_MiniMap", L"../Resources/Texture/MiniMap/Lake of Oblivion_MiniMap.png");
	CResourceManager::Load<CTexture>(L"Lake of Oblivion_1", L"../Resources/Texture/Lake_of_Oblivion_1.png");
	CResourceManager::Load<CTexture>(L"Lake of Oblivion_2", L"../Resources/Texture/Lake of Oblivion_2.png");
	CAudioClip* ac = CResourceManager::Load<CAudioClip>(L"Lake of Oblivion", L"../Resources/Sound/Lake Of Oblivion.mp3");
}

CTestScene::~CTestScene()
{
}

void CTestScene::Enter()
{
	CScene::Enter();

	if (m_pBackGround == nullptr)
	{
		m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
		m_pBackGround->CreateCollisionMap(L"Lake of Oblivion_2");
		m_pBackGround->CreateMap(L"Lake of Oblivion_1");
	}

	if (m_pAudioSource == nullptr)
	{
		m_pBackGround->AddComponent<CAudioListener>();
		m_pAudioSource = m_pBackGround->AddComponent<CAudioSource>();

		// 배경 음악 로드 및 재생
		CAudioClip* ac = CResourceManager::Find<CAudioClip>(L"Lake of Oblivion");
		m_pAudioSource->SetClip(ac);
		m_pAudioSource->Play();
	}

	
	/*if (CUIManager::GetUI(UI_TYPE::UI_Inventory) == nullptr)
	{
		CUIManager::Push(UI_TYPE::UI_Inventory);
	}*/

	////// ✅ 여기서 MiniMap 동적 등록
	////if (CUIManager::GetUI(UI_TYPE::UT_MiniMap) == nullptr)
	////{
	////	CMiniMap* uiMiniMap = Instantiate<CMiniMap>(LAYER_TYPE::LT_UI);
	////	CUIManager::RegisterUI(UI_TYPE::UT_MiniMap, uiMiniMap);
	////}

	////CUIManager::Push(UI_TYPE::UT_MiniMap);

	m_pPlayer = InstantiateFromPool<CPlayer>(LAYER_TYPE::LT_Player, L"Player");
	m_pPlayer->SetActive(true);

	// 포탈 생성
	CPortal* portal = InstantiateFromPool<CPortal>(LAYER_TYPE::LT_Portal, L"The_Land_of_Weathered_gladness");
	portal->SetActive(true);
	portal->m_pTransform->SetLocalPosition(Vector3(2000.0f, -430.0f, -1.0f));
	portal->SetMoveMap(L"The_Land_of_Weathered_gladness");

	// 카메라 설정
	CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
	camera->SetName(L"MainCamera");
	CCamera* cameraComp = camera->AddComponent<CCamera>();
	cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

	CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
	cameraScript->SetTarget(m_pPlayer); // 추적 카메라 거리

	renderer::mainCamera = cameraComp;
	renderer::activeCamera = renderer::mainCamera;
	cameraComp->SetCameraMask(~(1 << static_cast<UINT>(LAYER_TYPE::LT_UI)));
	m_vecCameras.push_back(cameraComp);

	// UI 카메라
	CGameObject* uiCamObj = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.f, 0.f, -5.0f));
	uiCamObj->SetName(L"UICamera");

	CCamera* uiCamera = uiCamObj->AddComponent<CCamera>();
	uiCamera->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);
	uiCamera->SetCameraMask(1 << static_cast<UINT>(LAYER_TYPE::LT_UI));
	m_vecCameras.push_back(uiCamera);

	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Portal, true);
}

void CTestScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CTestScene::Exit()
{
	CScene::Exit();

	if (m_pPlayer)
		m_pPlayer->SetActive(false);

	CPortal* portal = dynamic_cast<CPortal*>(FindObjectByName(L"The_Land_of_Weathered_gladness"));
	if (portal)
		portal->SetActive(false);

	//CUIManager::Pop(UI_TYPE::UT_MiniMap);
}

void CTestScene::Init()
{
	NextScene = L"The_Land_of_Weathered_Gladness";

    CScene::Init();

	// 인게임 시작 씬 캐릭터 생성(만들어 두고 씬에 진입할 때만 활성화)
	m_pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
	m_pPlayer->SetActive(false);

	CPortal* portal = Instantiate<CPortal>(LAYER_TYPE::LT_Portal);
	portal->SetName(L"The_Land_of_Weathered_gladness");
	portal->m_pTransform->SetLocalPosition(Vector3(2000.0f, -430.0f, -1.0f));
	portal->SetActive(false);
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

/*// 카메라 설정
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

CCollider* playerCol = pPlayer->AddComponent<CCollider>();
playerCol->SetScale(Vector2(54.0f, 65.0f));

renderer::selectedObject = pPlayer;*/

//CResourceManager::Load<CTexture>(L"Stage02_BG", L"../Resources/Texture/Main.bmp");
//CTexture* player = CResourceManager::Load<CTexture>(L"PLAYER", L"../Resources/Texture/Player/Player.bmp");

//2025-03-20
/*CMonster* pMonster = Instantiate<CMonster>(LAYER_TYPE::LT_Monster);
	CTransform* MonTr = pMonster->GetComponent<CTransform>();
	MonTr->SetParent(m_pPlayer->GetTransform());
	MonTr->SetLocalPosition(Vector3(20.0f, 0.0f, 0.0f));

	// 플레이어와 몬스터의 로컬 및 월드 좌표 가져오기
	Vector3 playerLocalPos = m_pPlayer->GetTransform()->GetLocalPosition();
	Vector3 playerWorldPos = m_pPlayer->GetTransform()->GetWorldPosition();
	Vector3 playerLocalScale = m_pPlayer->GetTransform()->GetLocalScale();
	Vector3 playerWorldScale = m_pPlayer->GetTransform()->GetWorldScale();

	// 플레이어와 몬스터의 로컬 및 월드 좌표 가져오기
	Vector3 MonsterrLocalPos = MonTr->GetLocalPosition();
	Vector3 MonsterrWorldPos = MonTr->GetWorldPosition();
	Vector3 MonsterrLocalScale = MonTr->GetLocalScale();
	Vector3 MonsterrWorldScale = MonTr->GetWorldScale();

	// 디버그 출력 (플레이어)
	char debugMsg[256];
	sprintf_s(debugMsg, "Player Local Pos: (%f, %f, %f)\n", playerLocalPos.x, playerLocalPos.y, playerLocalPos.z);
	OutputDebugStringA(debugMsg);

	sprintf_s(debugMsg, "Player World Pos: (%f, %f, %f)\n", playerWorldPos.x, playerWorldPos.y, playerWorldPos.z);
	OutputDebugStringA(debugMsg);

	sprintf_s(debugMsg, "Player Local Scale: (%f, %f, %f)\n", playerLocalScale.x, playerLocalScale.y, playerLocalScale.z);
	OutputDebugStringA(debugMsg);

	sprintf_s(debugMsg, "Player World Scale: (%f, %f, %f)\n", playerWorldScale.x, playerWorldScale.y, playerWorldScale.z);
	OutputDebugStringA(debugMsg);

	// 디버그 출력 (몬스터)
	sprintf_s(debugMsg, "Monster Local Pos: (%f, %f, %f)\n", MonsterrLocalPos.x, MonsterrLocalPos.y, MonsterrLocalPos.z);
	OutputDebugStringA(debugMsg);

	sprintf_s(debugMsg, "Monster World Pos: (%f, %f, %f)\n", MonsterrWorldPos.x, MonsterrWorldPos.y, MonsterrWorldPos.z);
	OutputDebugStringA(debugMsg);

	sprintf_s(debugMsg, "Monster Local Scale: (%f, %f, %f)\n", MonsterrLocalScale.x, MonsterrLocalScale.y, MonsterrLocalScale.z);
	OutputDebugStringA(debugMsg);

	sprintf_s(debugMsg, "Monster World Scale: (%f, %f, %f)\n", MonsterrWorldScale.x, MonsterrWorldScale.y, MonsterrWorldScale.z);
	OutputDebugStringA(debugMsg);*/ 

	//m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
		//CTransform* bgTransform = m_pBackGround->GetComponent<CTransform>();
		//bgTransform->SetLocalPosition(Vector3(0.0f, 300.0f, 2.0f));

		//// 배경 텍스처 로드
		//CTexture* bgTexture = CResourceManager::Find<CTexture>(L"Lake of Oblivion_1");
		//if (bgTexture != nullptr)
		//{
		//	// 텍스처 설정
		//	m_pBackGround->SetBackGroundTexture(bgTexture);

		//	// 텍스처 크기 설정하기
		//	CTexture::TextureSize textureSize = bgTexture->GetTextureSize();
		//	if (textureSize.width <= iWidth || textureSize.height <= iHeight)
		//	{
		//		bgTransform->SetLocalScale(Vector3(iWidth, iHeight, 0.0f));
		//	}
		//	else
		//	{
		//		bgTransform->SetLocalScale(Vector3(textureSize.width, textureSize.height, 0.0f));
		//	}
		//}

// 2025-04-16
// ✅ UI 전용 카메라 생성
	//if (renderer::uiCamera == nullptr)
	//{
	//	CGameObject* uiCameraObj = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.f, 0.f, -10.f));
	//	uiCameraObj->SetName(L"UICamera");
	//	CCamera* uiCamera = uiCameraObj->AddComponent<CCamera>();
	//	uiCamera->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);
	//	uiCamera->SetLayerMask(1 << static_cast<UINT>(LAYER_TYPE::LT_UI)); // ✅ UI 전용 렌더링
	//	//uiCamera->SetCameraOrder(1);

	//	renderer::uiCamera = uiCamera;
	//}

//카메라는 static변수에서 일반 변수로 변경
//다만 사이즈가 작아진 이유와 여전히 카메라가 적용이 안되는 이유도 포함할 것.

//void CTestScene::Init()
//{
//	CScene::Init();
//
//	// 플레이어 생성
//	// Instantiate의 vector로 캐릭터의 위치가 변경이 되지 않음... 왜?
//	m_pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
//
//	// 카메라 설정
//	CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
//	camera->SetName(L"MainCamera");
//	CCamera* cameraComp = camera->AddComponent<CCamera>();
//	cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);
//	//cameraComp->SetCameraOrder(0); // 오더 0
//
//	// LT_UI 레이어 제외하고 렌더링
//	/*UINT mask = 0xFFFFFFFF;
//	mask &= ~(1 << static_cast<UINT>(LAYER_TYPE::LT_UI));
//	cameraComp->SetLayerMask(mask);*/
//
//	CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
//	renderer::mainCamera = cameraComp;
//	//renderer::activeCamera = renderer::mainCamera;
//
//	cameraScript->SetTarget(m_pPlayer); // 추적 카메라 거리
//
//	/*CGameObject* uiCameraObj = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -5.0f));
//	uiCameraObj->SetName(L"UICamera");
//
//	CCamera* uiCamera = uiCameraObj->AddComponent<CCamera>();
//	uiCamera->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);*/
//	//uiCamera->SetCameraOrder(1); // 메인 카메라보다 높은 오더로 설정 (뒤에 렌더링)
//
//	//UINT uiMask = 0; // 모든 비트를 끈 상태에서
//	//uiMask |= (1 << static_cast<UINT>(LAYER_TYPE::LT_UI)); // UI 레이어만 렌더링하도록 마스크 설정
//	//uiCamera->SetLayerMask(uiMask);
//}

//// UI 카메라 생성
	//CGameObject* uiCamObj = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -15.0f));
	//uiCamObj->SetName(L"UICamera");
	//CCamera* uiCam = uiCamObj->AddComponent<CCamera>();
	//uiCam->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);
	//m_vecCameras.push_back(uiCam);