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

CScene_Start::CScene_Start()
{
	CResourceManager::Load<CTexture>(L"BG", L"../Resources/Texture/StartPanel.bmp");
	//CResourceManager::Load<CTexture>(L"StartBtn", L"../Resources/Texture/StartBtn.bmp");
}

CScene_Start::~CScene_Start()
{
}

void CScene_Start::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
	// 배경 음악 로드 및 재생
	CAudioClip* ac = CResourceManager::Load<CAudioClip>(L"BGSound", L"../Resources/Texture/MoonlightShadow.wav");

	CScene::Enter(L"BG", L"BGSound");

	CUIManager::Push(UI_TYPE::UT_Button);
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

	//// 카메라 설정
	//CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
	//CCamera* cameraComp = camera->AddComponent<CCamera>();
	//cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);
	//cameraComp->SetSize(200.0f);

	// 시작 버튼 UI 객체 가져오기
	/*CBtnUI* pBtn = static_cast<CBtnUI*>(CUIManager::GetUI(UI_TYPE::UT_Button));
	pBtn->SetName(L"StartBtn");
	if (pBtn)
	{
		CTexture* pBtnTexture = CResourceManager::Find<CTexture>(L"StartBtn");
		pBtn->SetBtnTexture(pBtnTexture);

		pBtn->SetBtnPosition(Vector2(200.0f, 200.0f));
	}*/
}

void CScene_Start::Update()
{
	CScene::Update();
}

void CScene_Start::LateUpdate()
{
	CScene::LateUpdate();
}

void CScene_Start::Render()
{
	CScene::Render();
}