#pragma once
#include "CGameObject.h"
#include "CBaseSkillScript.h"

class CBaseSkill : public CGameObject
{
public:
	CBaseSkill();
	virtual ~CBaseSkill();

	// -------------------------------
	// Lifecycle
	// -------------------------------
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;
	void Reset();

	void ActiveSkill(SKILL_TYPE eSkillType);
	void DeactiveSkill(SKILL_TYPE eSkillType);
	void DeactivateAllSkills();

	virtual void UseSkill() = 0;
	virtual void EndSkill() = 0;

	void SetSkillState(Skill_State _eSkillState) { m_eSkillState = _eSkillState; }
	Skill_State GetSkillState() { return m_eSkillState; }

	void SetOwnerPlayer(CPlayer* player) { m_pOwnerPlayer = player; }
	CPlayer* GetOwnerPlayer() const { return m_pOwnerPlayer; }

	void SetActive(bool active) { m_bActive = active; }
	bool GetActive() const { return m_bActive; }

protected:
	CCollider* m_pSkillCollider = nullptr;
	Skill_State m_eSkillState;
	CBaseSkillScript* m_pSkillScript = nullptr;
	CPlayer* m_pOwnerPlayer = nullptr;
	bool m_bActive;
};

// 스킬의 공통 렌더링 처리
// 플레이어 위치 및 상태 동기화
// 스킬 실행 공통 로직
// 스킬 종료 처리
// Flip 기능 제공