#pragma once
#include "../Component/CScript.h"

class CItemScript : public CScript
{
public:
	CItemScript();
	virtual ~CItemScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	void OnCollisionEnter(class CCollider* _pOther);
	void OnCollisionStay(class CCollider* _pOther);
	void OnCollisionExit(class CCollider* _pOther);

	void PickUpItem();

private:
	CGameObject* m_pColliderItem = nullptr;
	int m_iItemCnt = 0;
	int m_iItemTarget = 10;
};

