#include "CLayer.h"
#include "../Manager/CColliderManager.h"
#include "../Object/CObjectPool.h"

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

		if (gameObj && !gameObj->IsDead())
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
	// ���Ϳ� ����� ���� ������Ʈ �߿��� _pEraseGameObject�� ���� ������Ʈ�� ã�Ƽ� ����
	erase_if(m_vecGameObjects,
		[=](CGameObject* gameObj)
	{
		return gameObj == _pEraseGameObject;
	});

#pragma region
	//// erase_if�� ������� �ʰ� remove_if�� erase�� ����ؼ� ����
	//// 1. remove_if�� ����Ͽ�, ������ ���� ������Ʈ�� �� �ڷ� �̵���Ű��, �� ���� ��ġ�� ��ȯ
	//auto iter = remove_if(m_vecGameObjects.begin(), m_vecGameObjects.end(),
	//	[=](CGameObject* gameObj)
	//{
	//	return gameObj == _pEraseGameObject;	// ���� ����: Ư�� ������Ʈ�� ������ ��
	//});


	//// 2. ��ȯ�� ���� ��ġ(iter)���� ������ �������� ��Ҹ� ������ ����
	//m_vecGameObjects.erase(iter, m_vecGameObjects.end());
#pragma endregion
}

void CLayer::MoveToPool(CGameObject* _pGameObject)
{
	auto iter = find(m_vecGameObjects.begin(), m_vecGameObjects.end(), _pGameObject);
	if (iter != m_vecGameObjects.end())
	{
		m_vecGameObjects.erase(iter);
		_pGameObject->SetActive(false);
		m_vecPoolObjects.push_back(_pGameObject);
	}
}

CGameObject* CLayer::FindObjectByName(const wstring& _strName)
{
	for (CGameObject* obj : m_vecGameObjects)
	{
		if (obj && obj->GetName() == _strName)
			return obj;
	}

	return nullptr;
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
	/*for (CGameObject* gameObj : _vecGameObjects)
	{
		delete gameObj;
		gameObj = nullptr;
	}*/
	for (CGameObject* obj : _vecGameObjects)
	{
		obj->Reset(); // ���� �ʱ�ȭ �ʿ�
		obj->SetActive(false);

		// Ÿ�Կ� ���� Ǯ�� �ֱ�
		CObjectPool::Get().Push(obj);
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

/*for (CGameObject* gameObj : m_vecGameObjects)
	{
		if (gameObj == nullptr)
			continue;

		if (gameObj->IsActive() == false)
			continue;

		gameObj->Render();
	}*/

	// �� �̵�(�� ��ȯ)�� �� ��ü�� ����� ������ �ϰų�, �ٽ� ������ ���̶�� ������ ������ �� ����� ��
	// ���� ��ü�� ���µ� �ּҴ� �����ǰ� �־� ��۸������� �߻��ϴ� �� ����.