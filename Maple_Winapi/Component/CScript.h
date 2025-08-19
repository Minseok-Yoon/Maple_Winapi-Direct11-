#pragma once
#include "CComponent.h"

class CAnimator;
class CPlayer;
class CScript : public CComponent
{
public:
	CScript(SCRIPT_TYPE _eScriptType);
	virtual ~CScript();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnLateUpdate();
	virtual void OnRender(const Matrix& view, const Matrix& projection);

	virtual void OnCollisionEnter(class CCollider* _pOther);
	virtual void OnCollisionStay(class CCollider* _pOther);
	virtual void OnCollisionExit(class CCollider* _pOther);

	// 2025-06-05
	SCRIPT_TYPE	GetScriptType() { return m_eScriptType; }

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

protected:
	CAnimator* m_pAnimator = nullptr;
	CPlayer* m_pPlayer = nullptr;

// 2025-06-05
private:
	SCRIPT_TYPE		m_eScriptType;
};