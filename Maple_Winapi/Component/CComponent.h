#pragma once
#include "../Entity/CEntity.h"

class CGameObject;
class CTransform;
class CCollider;

class CComponent : public CEntity
{
public:
	CComponent(COMPONENT_TYPE _eComponentType);
	virtual ~CComponent();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(const Matrix& view, const Matrix& projection);

	void SetOwner(CGameObject* _pOwner) { m_pOwner = _pOwner; }
	CGameObject* GetOwner() { return m_pOwner; }

	COMPONENT_TYPE GetComponentType() { return m_eComponentType; }

public:
	CGameObject* m_pOwner;

private:
	COMPONENT_TYPE	m_eComponentType;
};