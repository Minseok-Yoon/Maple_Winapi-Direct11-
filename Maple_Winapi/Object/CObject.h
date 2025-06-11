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

template <typename T>
static T* InstantiateFromPool(LAYER_TYPE _eLayerType, const wstring& _strName)
{
	CScene* activeScene = CSceneManager::GetCurScene();
	CLayer* layer = activeScene->GetLayer(_eLayerType);

	// 1. 비활성화된 객체 재활용
	for (CGameObject* obj : layer->GetGameObjects())
	{
		if (!obj->IsActive() && obj->GetName() == _strName && dynamic_cast<T*>(obj))
		{
			obj->SetActive(true);
			return static_cast<T*>(obj);
		}
	}

	// 2. 없으면 새로 생성
	return Instantiate<T>(_eLayerType);
}

template <typename T>
static T* InstantiateFromPool(LAYER_TYPE _eLayerType, Vector3 _vPosition, const wstring& _strName)
{
	CScene* activeScene = CSceneManager::GetCurScene();
	CLayer* layer = activeScene->GetLayer(_eLayerType);

	for (CGameObject* obj : layer->GetGameObjects())
	{
		if (!obj->IsActive() && obj->GetName() == _strName && dynamic_cast<T*>(obj))
		{
			obj->SetActive(true);

			if (CTransform* tr = obj->GetComponent<CTransform>())
				tr->SetLocalPosition(_vPosition);

			return static_cast<T*>(obj);
		}
	}

	// 없으면 생성
	return Instantiate<T>(_eLayerType, _vPosition);
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

// 각 클래스별로 필요한 객체들을 Init으로 모든 객체를 먼저 new 만들어두고, 비활성화 한다음
// Enter로 들어가면 활성화 Exit로 나가면 비활성화 하는 방법으로 구현할 것.

/*template <typename T>
static T* InstantiateFromPool(LAYER_TYPE _eLayerType, Vector3 _vPosition)
{
	CScene* scene = CSceneManager::GetCurScene();
	CLayer* layer = scene->GetLayer(_eLayerType);

	T* gameObject = dynamic_cast<T*>(layer->GetPooledObject)(typeid(T)));

	if (!obj)
		obj = new T();

	obj->SetActive(true);
	obj->SetObjectState(OBJECT_STATE::OS_Active);
	obj->SetLayerType(_eLayerType);

	layer->AddGameObject(obj);

	return obj;
}*/
