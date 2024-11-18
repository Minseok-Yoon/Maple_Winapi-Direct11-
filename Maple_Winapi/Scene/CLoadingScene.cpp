#include "CLoadingScene.h"
#include "../Core/CCore.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Component/CRenderer.h"
#include "../Scene/CScene_Start.h"
#include "../Scene/CScene_Stage01.h"
#include "../Scene/CScene_Stage02.h"
#include "../Manager/CKeyManager.h"

extern CCore core;

CLoadingScene::CLoadingScene() :
	m_bLoadCompleted(false),
	m_pResourcesLoadThread(),
	m_pMutualExclusion()
{
}

CLoadingScene::~CLoadingScene()
{
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
}

void CLoadingScene::Render()
{
	if (m_bLoadCompleted)
	{
		// 만약 메인쓰레드가 종료되는데 자식 쓰레드가 남아있다면
		// 자식 쓰레드를 메인쓰레드에 편입시켜 메인쓰레드가 종료되기전까지 block
		m_pResourcesLoadThread->join();

		// 메인쓰레드와 완전 분리 시켜 독립적인 쓰레드 운영가능
		CSceneManager::LoadScene(L"Stage01", L"Stage01_BG", L"BGSound1");
	}
}

void CLoadingScene::Enter()
{
}

void CLoadingScene::Exit()
{
}

void CLoadingScene::resourcesLoad(std::mutex& _pMutex)
{
    auto startTime = std::chrono::steady_clock::now();

    while (true)
    {
        if (core.IsLoaded() == true)
            break;

        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() > 5)
        {
            OutputDebugStringW(L"TimeOut waiting for core to load.\n");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    _pMutex.lock();
    {
        // 리소스 로딩
        wstring texturePath = L"../Resources/Texture/Stage1.bmp";
        if (CResourceManager::Load<CTexture>(L"BG1", texturePath))
        {
            wstringstream ws;
            ws << L"Resource loaded successfully: " << texturePath << "\n";
            OutputDebugStringW(ws.str().c_str());
        }
        else
        {
            wstringstream ws;
            ws << L"Failed to load resource: " << texturePath << "\n";
            OutputDebugStringW(ws.str().c_str());
        }

        renderer::Init();

        // 씬 생성
        CSceneManager::CreateScene<CScene_Start>(L"StartScene");
        CSceneManager::CreateScene<CScene_Stage01>(L"Stage01");
        CSceneManager::CreateScene<CScene_Stage02>(L"Stage02");
    }
    _pMutex.unlock();

    // 현재 씬 설정 (LoadingScene으로 설정 후, LoadScene을 통해 전환)
    CSceneManager::SetCurScene(L"LoadingScene");

    // 로딩 완료 설정
    m_bLoadCompleted = true;
}