#pragma once
#include "../Scene/CScene.h"

class CScene;
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

private:
	static map<wstring, CScene*>	m_mapScene;	// ¸ðµç ¾À ¸ñ·Ï
	static CScene* m_pCurScene;							// ÇöÀç ¾À
	static CScene* m_pDontDestroyOnLoad;
};