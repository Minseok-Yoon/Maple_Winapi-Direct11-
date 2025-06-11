#include "CSceneManager.h"
#include "../Scene/CDontDestroyOnLoad.h"
#include "CKeyManager.h"

wstring CSceneManager::m_strNextSceneName = L"";
bool CSceneManager::m_bSceneChangeRequested = false;

CScene* CSceneManager::m_pCurScene = nullptr;
CScene* CSceneManager::m_pDontDestroyOnLoad = nullptr;
map<wstring, CScene*> CSceneManager::m_mapScene = {};

void CSceneManager::Init()
{
	m_pDontDestroyOnLoad = CreateScene<CDontDestroyOnLoad>(L"DontDestroyOnLoad");
}

void CSceneManager::Update()
{
	if (m_bSceneChangeRequested)
	{
		ChangeScene(m_strNextSceneName);  // 이제 오버로딩된 함수 또는 static 변수로 처리
		m_bSceneChangeRequested = false;
	}

	m_pCurScene->Update();
	m_pDontDestroyOnLoad->Update();
}

void CSceneManager::LateUpdate()
{
	m_pCurScene->LateUpdate();
	m_pDontDestroyOnLoad->LateUpdate();
}

void CSceneManager::Render()
{
	m_pCurScene->Render();
	m_pDontDestroyOnLoad->Render();
}

void CSceneManager::Destroy()
{
	m_pCurScene->Destroy();
	m_pDontDestroyOnLoad->Destroy();
}

void CSceneManager::Release()
{
	for (auto& iter : m_mapScene)
	{
		delete iter.second;
		iter.second = nullptr;
	}
}

bool CSceneManager::SetCurScene(const wstring& _strName)
{
	auto iter = m_mapScene.find(_strName);

	if (iter == m_mapScene.end())
		return false;

	m_pCurScene = iter->second;
	return true;
}

const wchar_t* CSceneManager::GetCurrentSceneName()
{
	return m_pCurScene ? m_pCurScene->GetName().c_str() : L"No Current Scene";
}

CScene* CSceneManager::LoadScene(const wstring& _strName)
{
	if (m_pCurScene)
		m_pCurScene->Exit();

	if (!SetCurScene(_strName))
		return nullptr;

	m_pCurScene->Enter();

	const wchar_t* currentSceneName = CSceneManager::GetCurrentSceneName();

	std::wstringstream ss;
	ss << L"CurScene: " << currentSceneName << "\n";
	OutputDebugStringW(ss.str().c_str());

	return m_pCurScene;
}

CScene* CSceneManager::LoadScene(const wstring& _strName, const wstring& _strBackGroundName, const wstring& _strAudioName)
{
	if (m_pCurScene)
		m_pCurScene->Exit();

	if (!SetCurScene(_strName))
		return nullptr;

	m_pCurScene->Enter(_strBackGroundName, _strAudioName);

	return m_pCurScene;
}

CScene* CSceneManager::ChangeScene(const wstring& _strName,
	const wstring& _strBackGroundName, const wstring& _strAudioName)
{
	if (m_pCurScene)
	{
		m_pCurScene->Exit();
		
		auto iter = m_mapScene.find(m_pCurScene->GetName());
		if (iter != m_mapScene.end())
		{
			delete iter->second;
			m_mapScene.erase(iter);
		}
	}

	LoadScene(_strName, _strBackGroundName, _strAudioName);

	return m_pCurScene;
}

vector<CGameObject*> CSceneManager::GetGameObjects(LAYER_TYPE _eLayerType)
{
	vector<CGameObject*> gameObjects = m_pCurScene->GetLayer(_eLayerType)->GetGameObjects();
	vector<CGameObject*> dontDestroyOnLoad = m_pDontDestroyOnLoad->GetLayer(_eLayerType)->GetGameObjects();

	gameObjects.insert(gameObjects.end(),
		dontDestroyOnLoad.begin(), dontDestroyOnLoad.end());

	return gameObjects;
}

void CSceneManager::RequestSceneChange(const wstring& sceneName)
{
	m_strNextSceneName = sceneName;
	m_bSceneChangeRequested = true;
}
