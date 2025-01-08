#include "../pch.h"
#include "CScene_Start.h"
#include "../Resource/CTexture.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"
#include "../Component/CCamera.h"
#include "../Core/CCore.h"

#include "../Object/CBtnUI.h"
#include "../Object/CUI.h"

#include "../Object/CGround.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"
#include "../Object/CGameObject.h"
#include "../Resource/CAudioClip.h"
#include "../Component/CAudioListener.h"
#include "../Component/CAudioSource.h"
#include "../Component/CSpriteRenderer.h"
#include "../Object/CObject.h"
#include "../Component/CBoxCollider2D.h"
#include "../Component/CCameraScript.h"

#include "../Object/CPlayer.h"

CScene_Start::CScene_Start()
{
	CTexture* bgTexture = CResourceManager::Load<CTexture>(L"BG", L"../Resources/Texture/StartPanel.bmp");
	CAudioClip* ac = CResourceManager::Load<CAudioClip>(L"BGSound", L"../Resources/Sound/MoonlightShadow.wav");
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
	CScene::Enter(L"BG", L"BGSound");
}

void CScene_Start::Enter()
{
	CScene::Enter();
}

void CScene_Start::Exit()
{
	// 배경 음악을 정지하는 로직 추가
	//CUIManager::Pop(UI_TYPE::UT_Button);

	CScene::Exit();
}

void CScene_Start::Init()
{
	CScene::Init();

	// 카메라 설정
	CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
	CCamera* cameraComp = camera->AddComponent<CCamera>();
	cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

	CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
	renderer::mainCamera = cameraComp;
}

void CScene_Start::Update()
{
	CScene::Update();
}

void CScene_Start::LateUpdate()
{
	CScene::LateUpdate();

	if (KEY_TAP(KEY_CODE::Y))
	{
		CSceneManager::LoadScene(L"Stage01", L"Stage01_BG", L"Stage01_BGSound");
	}
	if (KEY_TAP(KEY_CODE::U))
	{
		CSceneManager::LoadScene(L"TestScene");
	}

	if (KEY_TAP(KEY_CODE::O))
	{
		CSceneManager::LoadScene(L"DrawRectScene");
	}
}

void CScene_Start::Render()
{
	CScene::Render();
}