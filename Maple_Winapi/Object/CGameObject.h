#pragma once
#include "../pch.h"
#include "../Component/CComponent.h"
#include "../Component/CCollider.h"

void Destroy(CGameObject* _pGameObject);

class CGameObject : public CEntity
{
public:
	friend void Destroy(CGameObject* _pObj);

	CGameObject();
	~CGameObject();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

	template <typename T>
	T* AddComponent()
	{
		T* comp = new T();
		comp->Init();
		comp->SetOwner(this);
		m_vecComponents[(UINT)comp->GetComponentType()] = comp;

		return comp;
	}

	template <typename T>
	T* GetComponent()
	{
		T* component = nullptr;
		for (CComponent* comp : m_vecComponents)
		{
			component = dynamic_cast<T*>(comp);
			if (component)
				break;
		}
		return component;
	}

private:
	void initializeTransform();
	void death() { m_eObjectState = OBJECT_STATE::OS_Dead; }

public:
	void SetLayerType(const LAYER_TYPE _eLayerType) { m_eLayerType = _eLayerType; }
	LAYER_TYPE GetLayerType() const { return m_eLayerType; }

	OBJECT_STATE GetObjectState() { return m_eObjectState; }

	void SetActive(bool _bPower)
	{
		if (_bPower == true) m_eObjectState = OBJECT_STATE::OS_Active;
		if (_bPower == false) m_eObjectState = OBJECT_STATE::OS_Paused;
	}

	bool IsActive() const { return m_eObjectState == OBJECT_STATE::OS_Active; }
	bool IsDead() const { return m_eObjectState == OBJECT_STATE::OS_Dead; }

private:
	vector<CComponent*>		m_vecComponents;
	LAYER_TYPE				m_eLayerType;
	OBJECT_STATE			m_eObjectState;
};