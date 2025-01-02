#include "CLoadingScene.h"
#include "../Core/CCore.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Component/CRenderer.h"
#include "../Scene/CScene_Start.h"
#include "../Scene/CScene_Stage01.h"/*
#include "../Scene/CScene_Stage02.h"*/
#include "../Manager/CKeyManager.h"

#include "../Scene/CTestScene.h"

extern CCore core;

CLoadingScene::CLoadingScene() :
	m_bLoadCompleted(false),
	m_pResourcesLoadThread(),
	m_pMutualExclusion()
{
}

CLoadingScene::~CLoadingScene()
{
    m_pResourcesLoadThread->join();

	delete m_pResourcesLoadThread;
	m_pResourcesLoadThread = nullptr;
}

void CLoadingScene::Init()
{
	m_pResourcesLoadThread = new std::thread(&CLoadingScene::resourcesLoad,
		this, std::ref(m_pMutualExclusion));
}

void CLoadingScene::Update()
{
}

void CLoadingScene::LateUpdate()
{
    // 텍스처를 렌더링
    if (m_bLoadCompleted)
    {
        // 로딩 완료 시 처리
        CSceneManager::LoadScene(L"StartScene", L"BG", L"BGSound");
    }
}

void CLoadingScene::Render()
{
}

void CLoadingScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CLoadingScene::Enter()
{
}

void CLoadingScene::Exit()
{
}

void CLoadingScene::resourcesLoad(std::mutex& _pMutex)
{
    //auto startTime = std::chrono::steady_clock::now();

    //while (true)
    //{
    //    if (core.IsLoaded() == true)
    //        break;

    //    auto currentTime = std::chrono::steady_clock::now();
    //    if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() > 5)
    //    {
    //        OutputDebugStringW(L"TimeOut waiting for core to load.\n");
    //        break;
    //    }

    //    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //}

    //_pMutex.lock();
    //{
    //    // 리소스 로딩
    //    wstring texturePath = L"../Resources/Texture/StartPanel.bmp";
    //    if (CResourceManager::Load<CTexture>(L"BG", texturePath))
    //    {
    //        wstringstream ws;
    //        ws << L"Resource loaded successfully: " << texturePath << "\n";
    //        OutputDebugStringW(ws.str().c_str());
    //    }
    //    else
    //    {
    //        wstringstream ws;
    //        ws << L"Failed to load resource: " << texturePath << "\n";
    //        OutputDebugStringW(ws.str().c_str());
    //    }

    //    renderer::Init();

    //    // 씬 생성
    //    CSceneManager::CreateScene<CScene_Start>(L"StartScene");
    //    /*CSceneManager::CreateScene<CScene_Stage01>(L"Stage01");
    //    CSceneManager::CreateScene<CScene_Stage02>(L"Stage02");*/
    //}
    //_pMutex.unlock();

    //// 로딩 완료 설정
    //m_bLoadCompleted = true;

    while (true)
    {
        if (core.IsLoaded() == true)
            break;

        _pMutex.lock();
        {
            //CResourceManager::Load<CTexture>(L"Player", L"../Resources/Texture/Player/Player.bmp");

            renderer::Init();

            CSceneManager::CreateScene<CScene_Start>(L"StartScene");
            CSceneManager::CreateScene<CScene_Stage01>(L"Stage01");
            CSceneManager::CreateScene<CTestScene>(L"TestScene");
        }
        _pMutex.unlock();

        CSceneManager::SetCurScene(L"LoadingScene");
        m_bLoadCompleted = true;
    }
}