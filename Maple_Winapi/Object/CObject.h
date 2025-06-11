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

	// 1. ��Ȱ��ȭ�� ��ü ��Ȱ��
	for (CGameObject* obj : layer->GetGameObjects())
	{
		if (!obj->IsActive() && obj->GetName() == _strName && dynamic_cast<T*>(obj))
		{
			obj->SetActive(true);
			return static_cast<T*>(obj);
		}
	}

	// 2. ������ ���� ����
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

	// ������ ����
	return Instantiate<T>(_eLayerType, _vPosition);
}

static void DontDestroyOnLoad(CGameObject* _pGameObject)
{
	CScene* activeScene = CSceneManager::GetCurScene();

	// ��������� ���ӿ�����Ʈ�� �����ش�.
	activeScene->EraseGameObject(_pGameObject);

	// �ش� ���ӿ�����Ʈ�� DontDestroy������ �־��ش�.
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

// �� Ŭ�������� �ʿ��� ��ü���� Init���� ��� ��ü�� ���� new �����ΰ�, ��Ȱ��ȭ �Ѵ���
// Enter�� ���� Ȱ��ȭ Exit�� ������ ��Ȱ��ȭ �ϴ� ������� ������ ��.

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
