#pragma once
#include "../pch.h"
#include "../Entity/CEntity.h"
#include "../Object/CGameObject.h"

class CLayer : public CEntity
{
public:
	CLayer();
	~CLayer();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void Destroy();

	void AddGameObject(CGameObject* _pAddGameObject);
	void EraseGameObject(CGameObject* _pEraseGameObject);

	const vector<CGameObject*> GetGameObjects() { return m_vecGameObjects; }

private:
	void findDeadGameObjects(OUT vector<CGameObject*>& _vecGameObjects);
	void deleteGameObjects(vector<CGameObject*> _vecGameObjects);
	void eraseDeadGameObjects();

private:
	vector<CGameObject*> m_vecGameObjects;
};

typedef vector<CGameObject*>::iterator GameObjectIter;