#include "../pch.h"
#include "The_Land_of_Weathered_gladness.h"
#include "../Object/CObject.h"
#include "../Object/CPortal.h"
#include "../Object/Erdas_of_Gladness.h"

#include "../Manager/CResourceManager.h"
#include "../Component/CAudioListener.h"
#include "../Component/CCameraScript.h"
#include "../Component/CRenderer.h"

The_Land_of_Weathered_gladness::The_Land_of_Weathered_gladness()
{
	CResourceManager::Load<CTexture>(L"Gladness", L"../Resources/Texture/The_Land_of_Weathered_gladness.png");
	CResourceManager::Load<CTexture>(L"Gladness_Pixel", L"../Resources/Texture/The_Land_of_Weathered_gladness_Pixel.png");
	CAudioClip* ac = CResourceManager::Load<CAudioClip>(L"Lake of Oblivion", L"../Resources/Sound/Lake Of Oblivion.mp3");
}

The_Land_of_Weathered_gladness::~The_Land_of_Weathered_gladness()
{
}

void The_Land_of_Weathered_gladness::Enter()
{
	The_Lake_of_Oblivion::Enter();

	if (m_pBackGround == nullptr)
	{
		m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
		m_pBackGround->CreateCollisionMap(L"Gladness_Pixel");
		m_pBackGround->CreateMap(L"Gladness");
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

	m_pPlayer = InstantiateFromPool<CPlayer>(LAYER_TYPE::LT_Player, L"Player");
	m_pPlayer->SetActive(true);

	// 포탈 생성
	CPortal* portal = InstantiateFromPool<CPortal>(LAYER_TYPE::LT_Portal, L"The_Land_of_Lake");
	portal->SetActive(true);
	portal->m_pTransform->SetLocalPosition(Vector3(-300.0f, -430.0f, -1.0f));
	portal->SetMoveMap(L"TestScene");

	CreateMonster<Erdas_of_Gladness>({ -400.0f, 0.0f, -1.0f });

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
	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Monster, true);
	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Item, true);
}

void The_Land_of_Weathered_gladness::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void The_Land_of_Weathered_gladness::Exit()
{
	The_Lake_of_Oblivion::Exit();

	if (m_pPlayer)
		m_pPlayer->SetActive(false);

	CPortal* portal = dynamic_cast<CPortal*>(FindObjectByName(L"The_Land_of_Lake"));
	if (portal)
		portal->SetActive(false);
}

void The_Land_of_Weathered_gladness::Init()
{
	PrevScene = L"Lake of Oblivion_1";
	NextScene = L"The_Land_of_Weathered_madness";

	The_Lake_of_Oblivion::Init();

	//m_pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);

	// 이전 포탈맵 생성
	CPortal* portal = Instantiate<CPortal>(LAYER_TYPE::LT_Portal);
	portal->SetName(L"The_Land_of_Lake");
	portal->m_pTransform->SetLocalPosition(Vector3(-300.0f, -430.0f, -1.0f));
	portal->SetActive(false);
}

void The_Land_of_Weathered_gladness::Update()
{
	The_Lake_of_Oblivion::Update();
}

void The_Land_of_Weathered_gladness::LateUpdate()
{
	The_Lake_of_Oblivion::LateUpdate();
}

void The_Land_of_Weathered_gladness::Render()
{
	The_Lake_of_Oblivion::Render();
}

//// 카메라 설정
	//CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
	//camera->SetName(L"MainCamera");
	//CCamera* cameraComp = camera->AddComponent<CCamera>();
	//cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

	//CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
	//cameraScript->SetTarget(m_pPlayer); // 추적 카메라 거리

	//renderer::mainCamera = cameraComp;
	//renderer::activeCamera = renderer::mainCamera;
	//cameraComp->SetCameraMask(~(1 << static_cast<UINT>(LAYER_TYPE::LT_UI)));
	//m_vecCameras.push_back(cameraComp);

	//// UI 카메라
	//CGameObject* uiCamObj = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.f, 0.f, -5.0f));
	//uiCamObj->SetName(L"UICamera");

	//CCamera* uiCamera = uiCamObj->AddComponent<CCamera>();
	//uiCamera->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);
	//uiCamera->SetCameraMask(1 << static_cast<UINT>(LAYER_TYPE::LT_UI));
	//m_vecCameras.push_back(uiCamera);

	//CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Portal, true);