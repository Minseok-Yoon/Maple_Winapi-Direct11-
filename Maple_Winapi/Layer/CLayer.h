#pragma once
#include "../pch.h"
#include "../Entity/CEntity.h"
#include "../Object/CGameObject.h"
#include "../Manager/CColliderManager.h"

class CColliderManager;
class CLayer : public CEntity
{
public:
	CLayer();
	virtual ~CLayer();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void Destroy();

	void AddGameObject(CGameObject* _pAddGameObject);
	void EraseGameObject(CGameObject* _pEraseGameObject);
	void MoveToPool(CGameObject* _pGameObject);
	CGameObject* FindObjectByName(const wstring& _strName);

	vector<CGameObject*>& GetGameObjects() { return m_vecGameObjects; }
	vector<CGameObject*>& GetPoolObjects() { return m_vecPoolObjects; }

private:
	void findDeadGameObjects(OUT vector<CGameObject*>& _vecGameObjects);
	void deleteGameObjects(vector<CGameObject*> _vecGameObjects);
	void eraseDeadGameObjects();

private:
	vector<CGameObject*> m_vecGameObjects;
	vector<CGameObject*> m_vecPoolObjects;
};

using CGameObjectIter = vector<CGameObject*>::iterator;