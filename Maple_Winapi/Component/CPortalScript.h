#pragma once
#include "../Component/CScript.h"

class CPortalScript : public CScript
{
public:
	CPortalScript();
	virtual ~CPortalScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	void OnCollisionEnter(class CCollider* _pOther) override;
	void OnCollisionStay(class CCollider* _pOther) override;
	void OnCollisionExit(class CCollider* _pOther) override;

private:
};