#pragma once
#include "../Entity/CEntity.h"

class CGameObject;

class CComponent : public CEntity
{
public:
	CComponent(COMPONENT_TYPE _eComponentType);
	virtual ~CComponent();

public:
	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

	void SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }
	CGameObject* GetOwner() { return m_pOwner; }

	COMPONENT_TYPE GetComponentType() { return m_eComponentType; }

private:
	CGameObject* m_pOwner;
	COMPONENT_TYPE	m_eComponentType;
};