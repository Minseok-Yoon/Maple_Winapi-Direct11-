#pragma once
#include "../Component/CScript.h"
#include "../Component/CTransform.h"

class CMonsterScript : public CScript
{
public:
	CMonsterScript();
	virtual ~CMonsterScript();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	void AttackEffect();

	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);
};