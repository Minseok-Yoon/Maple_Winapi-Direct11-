#pragma once
#include "CComponent.h"

class CScript : public CComponent
{
public:
	CScript();
	~CScript();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	virtual void OnCollisionEnter(class CCollider* _pOther);
	virtual void OnCollisionStay(class CCollider* _pOther);
	virtual void OnCollisionExit(class CCollider* _pOther);
};