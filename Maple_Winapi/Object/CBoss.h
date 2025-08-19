#pragma once
#include "CGameObject.h"
#include "CBossScript.h"

#define BOSS_MAX_HP 10000;

class CBoss : public CGameObject
{
public:
	CBoss();
	virtual ~CBoss();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;
	
	//virtual void Release();

	void SetBossCurState(BOSS_MON_STATE _eBossMonState) { m_eBossMonState = _eBossMonState; }
	BOSS_MON_STATE GetBossCurState() const { return m_eBossMonState; }

protected:
	bool	m_bIsCoolDownUpdate = true;
	// 2025-07-28 Quarters가 어떤 역할을 하는지? 패턴? Phase?

	CTransform*			m_pBossTransform = nullptr;
	CSpriteRenderer*	m_pBossRenderer = nullptr;
	CCollider*			m_pBossCollider = nullptr;
	CGameObject*		m_pSound = nullptr;

	BOSS_MON_STATE			m_eBossMonState;
	class CBossScript* m_pBossScript = nullptr;
};