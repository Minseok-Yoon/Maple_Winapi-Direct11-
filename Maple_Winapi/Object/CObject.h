#pragma once
#include "../Component/CComponent.h"
#include "../Component/CTransform.h"
#include "../Object/CGameObject.h"
#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Layer/CLayer.h"

template <typename T>
static T* Instantiate(LAYER_TYPE _eLayerType)
{
	T* gameObject = new T();
	gameObject->SetLayerType(_eLayerType);
	CScene* activeScene = CSceneManager::GetCurScene();
	CLayer* layer = activeScene->GetLayer(_eLayerType);
	layer->AddGameObject(gameObject);

	gameObject->Init();

	return gameObject;
}

template <typename T>
static T* Instantiate(LAYER_TYPE _eLayerType, Vector3 _vPosition)
{
	T* gameObject = new T();
	gameObject->SetLayerType(_eLayerType);
	CScene* activeScene = CSceneManager::GetCurScene();
	CLayer* layer = activeScene->GetLayer(_eLayerType);
	layer->AddGameObject(gameObject);

	CTransform* tr = gameObject->GetComponent<CTransform>();
	tr->SetLocalPosition(_vPosition);

	gameObject->Init();

	return gameObject;
}

static void DontDestroyOnLoad(CGameObject* _pGameObject)
{
	CScene* activeScene = CSceneManager::GetCurScene();

	// 현재씬에서 게임오브젝트를 지워준다.
	activeScene->EraseGameObject(_pGameObject);

	// 해당 게임오브젝트를 DontDestroy씬으로 넣어준다.
	CScene* dontDestroyOnLoad = CSceneManager::GetDontDestroyOnLoad();
	dontDestroyOnLoad->AddGameObject(_pGameObject, _pGameObject->GetLayerType());
}

class CObject : public CGameObject
{
public:
	CObject();
	~CObject();

protected:
	class CScene* GetScene();
	class CObject* GetObject();
};