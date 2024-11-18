#include "CLayer.h"

CLayer::CLayer() :
	m_vecGameObjects{}
{
}

CLayer::~CLayer()
{
	for (CGameObject* gameObj : m_vecGameObjects)
	{
		if (gameObj == nullptr)
			continue;

		delete gameObj;
		gameObj = nullptr;
	}
}

void CLayer::Init()
{
	for (CGameObject* gameObj : m_vecGameObjects)
	{
		if (gameObj == nullptr)
			continue;

		gameObj->Init();
	}
}

void CLayer::Update()
{
	for (CGameObject* gameObj : m_vecGameObjects)
	{
		if (gameObj == nullptr)
			continue;

		if (gameObj->IsActive() == false)
			continue;

		gameObj->Update();
	}
}

void CLayer::LateUpdate()
{
	for (CGameObject* gameObj : m_vecGameObjects)
	{
		if (gameObj == nullptr)
			continue;

		if (gameObj->IsActive() == false)
			continue;

		gameObj->LateUpdate();
	}
}

void CLayer::Render()
{
	for (CGameObject* gameObj : m_vecGameObjects)
	{
		if (gameObj == nullptr)
			continue;

		if (gameObj->IsActive() == false)
			continue;

		gameObj->Render();
	}
}

void CLayer::Destroy()
{
	vector<CGameObject*> deleteObjects = {};
	findDeadGameObjects(deleteObjects);
	deleteGameObjects(deleteObjects);
	eraseDeadGameObjects();
}

void CLayer::AddGameObject(CGameObject* _pAddGameObject)
{
	if (_pAddGameObject == nullptr)
		return;

	m_vecGameObjects.push_back(_pAddGameObject);
}

void CLayer::EraseGameObject(CGameObject* _pEraseGameObject)
{
	// 벡터에 저장된 게임 오브젝트 중에서 _pEraseGameObject와 같은 오브젝트를 찾아서 삭제
	erase_if(m_vecGameObjects,
		[=](CGameObject* gameObj)
	{
		return gameObj == _pEraseGameObject;
	});

#pragma region
	//// erase_if을 사용하지 않고 remove_if랑 erase를 사용해서 구현
	//// 1. remove_if를 사용하여, 삭제할 게임 오브젝트를 맨 뒤로 이동시키고, 그 시작 위치를 반환
	//auto iter = remove_if(m_vecGameObjects.begin(), m_vecGameObjects.end(),
	//	[=](CGameObject* gameObj)
	//{
	//	return gameObj == _pEraseGameObject;	// 삭제 조건: 특정 오브젝트와 같은지 비교
	//});


	//// 2. 반환된 시작 위치(iter)부터 벡터의 끝까지의 요소를 실제로 제거
	//m_vecGameObjects.erase(iter, m_vecGameObjects.end());
#pragma endregion
}

void CLayer::findDeadGameObjects(OUT vector<CGameObject*>& _vecGameObjects)
{
	for (CGameObject* gameObj : m_vecGameObjects)
	{
		OBJECT_STATE active = gameObj->GetObjectState();
		if (active == OBJECT_STATE::OS_Dead)
			_vecGameObjects.push_back(gameObj);
	}
}

void CLayer::deleteGameObjects(vector<CGameObject*> _vecGameObjects)
{
	for (CGameObject* gameObj : m_vecGameObjects)
	{
		delete gameObj;
		gameObj = nullptr;
	}
}

void CLayer::eraseDeadGameObjects()
{
	erase_if(m_vecGameObjects,
		[](CGameObject* gameObj)
	{
		return gameObj->IsDead();
	});
#pragma region
	/*auto iter = remove_if(m_vecGameObjects.begin(), m_vecGameObjects.end(),
		[](CGameObject* gameobj)
	{
		return gameobj->IsDead();
	});

	m_vecGameObjects.erase(iter, m_vecGameObjects.end());*/
#pragma endregion
}