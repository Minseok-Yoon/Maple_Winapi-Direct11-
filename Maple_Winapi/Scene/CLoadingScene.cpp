#include "CLoadingScene.h"
#include "../Core/CCore.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Component/CRenderer.h"
#include "../Scene/CScene_Start.h"
#include "../Scene/CScene_Stage01.h"
#include "../Manager/CKeyManager.h"

#include "../Scene/CTestScene.h"
#include "../Scene/CRectDrawScene.h"
#include "../Scene/CPixScene.h"

extern CCore core;

CLoadingScene::CLoadingScene() :
	m_bLoadCompleted(false),
	m_pResourcesLoadThread(),
	m_pMutualExclusion()
{
}

CLoadingScene::~CLoadingScene()
{
    // 스레드가 종료되었는지 확인하고 join호출
    if (m_pResourcesLoadThread && m_pResourcesLoadThread->joinable())
    {
        m_pResourcesLoadThread->join();
    }

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
    if (m_bLoadCompleted.load()) // atomic 변수 사용
    {
        CSceneManager::LoadScene(L"StartScene", L"BG", L"BGSound");
    }
}

void CLoadingScene::Render()
{
}

void CLoadingScene::resourcesLoad(std::mutex& _pMutex)
{
    while (!core.IsLoaded() && !m_bLoadCompleted) // 추가 조건 추가
    {
        _pMutex.lock();
        {
            renderer::Init();

            CSceneManager::CreateScene<CScene_Start>(L"StartScene");
            CSceneManager::CreateScene<CScene_Stage01>(L"Stage01");
            CSceneManager::CreateScene<CTestScene>(L"TestScene");
            /*CSceneManager::CreateScene<CRectDrawScene>(L"DrawRectScene");*/
            CSceneManager::CreateScene<CPixScene>(L"PixScene");
        }
        _pMutex.unlock();

        CSceneManager::SetCurScene(L"LoadingScene");
        m_bLoadCompleted.store(true);
    }
}