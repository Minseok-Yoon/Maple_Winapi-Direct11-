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
	tr->SetPosition(_vPosition);

	return gameObject;
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