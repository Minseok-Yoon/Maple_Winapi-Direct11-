#pragma once
#include "../Component/CComponent.h"
#include "../Component/CCollider.h"
#include "../Component/CTransform.h"
#include "../Component/CSpriteRenderer.h"

void Destroy(CGameObject* _pGameObject);

class CTransform;
class CComponent;

class CGameObject : public CEntity
{
public:
	friend class CMonsterScript;
	friend void Destroy(CGameObject* _pObj);

	CGameObject();
	virtual ~CGameObject();

	// -------------------------------
	// Lifecycle
	// -------------------------------
	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(const Matrix& view, const Matrix& projection);
	void Reset();

	// -------------------------------
	// State Control
	// -------------------------------
	void death() { m_eObjectState = OBJECT_STATE::OS_Dead; }

	bool IsActive() const { return m_eObjectState == OBJECT_STATE::OS_Active; }
	bool IsDead() const { return m_eObjectState == OBJECT_STATE::OS_Dead; }

	void SetDead() { IsDeathValue = true; SetActive(false); }
	void SetIsDead(bool b) { IsDeathValue = b; }
	void SetIsUpdate(bool b) { IsUpdateValue = b; }

	inline void AddHP(int _iHP)
	{
		m_iHP += _iHP;
		if (0 > m_iHP)
		{
			m_iHP = 0;
		}
	}

	inline void AddMP(int _iMP)
	{
		m_iMP += _iMP;
		if (0 > m_iMP)
		{
			m_iMP = 0;
		}
	}

	// set / get 함수
	void SetZOrder(int z) { m_iZOrder = z; }
	int GetZOrder() const { return m_iZOrder; }

	void SetLayerType(const LAYER_TYPE _eLayerType) { m_eLayerType = _eLayerType; }
	LAYER_TYPE GetLayerType() const { return m_eLayerType; }

	void SetObjectState(const OBJECT_STATE _eObjectType) { m_eObjectState = _eObjectType; }
	OBJECT_STATE GetObjectState() { return m_eObjectState; }

	void SetActive(bool _bPower)
	{
		if (_bPower == true) m_eObjectState = OBJECT_STATE::OS_Active;
		if (_bPower == false) m_eObjectState = OBJECT_STATE::OS_Paused;
	}

	bool GetActive() const { return m_bActive; }

	CTransform* GetTransform() { return m_pTransform; }

	inline int GetHP() { return m_iHP; }
	inline int GetMP() { return m_iMP; }

	// -------------------------------
	// Component Management
	// -------------------------------
	template <typename T>
	T* AddComponent()
	{
		T* comp = new T();
		comp->SetOwner(this);
		m_vecComponents[static_cast<UINT>(comp->GetComponentType())] = comp;

		comp->Init();
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

public:
	CTransform* m_pTransform = nullptr;

protected:
	int m_iHP = 0;
	int m_iMP = 0;
	CGameObject* m_pParent = nullptr;

	vector<CComponent*>		m_vecComponents;
	LAYER_TYPE				m_eLayerType = LAYER_TYPE::LT_None;
	OBJECT_STATE			m_eObjectState = OBJECT_STATE::OS_Active;
	int						m_iZOrder = 0;
	bool IsUpdateValue = true;
	bool IsDeathValue = false;

	bool m_bActive = true;
};

// 2025-06-04 수정 전
//template <typename T>
//T* AddComponent()
//{
//	T* comp = new T();
//	comp->SetOwner(this);
//
//	UINT index = static_cast<UINT>(comp->GetComponentType());
//	if (index >= m_vecComponents.size()) {
//		OutputDebugStringA("ComponentType index out of range!\n");
//		delete comp; // or use SAFE_DELETE(comp)
//		return nullptr;
//	}
//
//	// 2025-06-04 Script로부터 파생되었는지 확인
//	if constexpr (is_base_of<CScript, T>::value) {
//		m_vecScripts.push_back(comp);
//	}
//
//	comp->Init();
//
//	return comp;
//}
//
//template <typename T>
//T* GetComponent()
//{
//	T* component = nullptr;
//	for (CComponent* comp : m_vecComponents)
//	{
//		component = dynamic_cast<T*>(comp);
//		if (component)
//			break;
//	}
//	return component;
//}

/*bool IsUpdate()
	{
		return m_pParent == nullptr ? IsUpdateValue == true && IsDeathValue == false : m_pParent->IsUpdate() && IsUpdateValue == true && IsDeath() == false;
	}

	bool IsDeath()
	{
		return m_pParent == nullptr ? IsDeathValue : m_pParent->IsDeath() || IsDeathValue;
	}*/