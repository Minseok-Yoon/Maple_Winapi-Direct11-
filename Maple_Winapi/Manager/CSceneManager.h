#pragma once
#include "../Scene/CScene.h"
#include "../Object/CGameObject.h"

class CScene;
class CGameObject;
class CSceneManager
{
public:
	template <typename T>
	static CScene* CreateScene(const wstring& _strName)
	{
		T* scene = new T();
		m_mapScene.insert(make_pair(_strName, scene));

		scene->SetName(_strName);
		scene->Init();

		return scene;
	}

public:
	static void Init();
	static void Update();
	static void LateUpdate();
	static void Render();
	static void Destroy();
	static void Release();

	static bool SetCurScene(const wstring& _strName);
	static CScene* GetCurScene() { return m_pCurScene; }

	static const wchar_t* GetCurrentSceneName();

	static CScene* LoadScene(const wstring& _strName);
	static CScene* LoadScene(const wstring& _strName, const wstring& _strBackGroundName, const wstring& _strAudioName);
	static CScene* ChangeScene(const wstring& _strName, const wstring& _strBackGroundName, const wstring& _strAudioName);
	static vector<CGameObject*> GetGameObjects(LAYER_TYPE _eLayerType);
	static CScene* GetDontDestroyOnLoad() { return m_pDontDestroyOnLoad; }

	void SelectObject(CGameObject* _pGameObject) { m_pSelectedObject = _pGameObject; }
	CGameObject* GetSelectObject() { return m_pSelectedObject; }

private:
	static map<wstring, CScene*>	m_mapScene;	// 모든 씬 목록
	static CScene* m_pCurScene;							// 현재 씬
	static CScene* m_pDontDestroyOnLoad;
	CGameObject* m_pSelectedObject = nullptr;	// 선택된 오브젝트
};