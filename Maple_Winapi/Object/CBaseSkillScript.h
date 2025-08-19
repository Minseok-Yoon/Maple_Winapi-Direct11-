#pragma once
#include "../Component/CScript.h"
#include "CBackGround.h"

class CBaseSkill;
class CMonster;

struct SkillAnimInfo
{
	wstring		pathPattern;
	int			frameCount;
	float		frameDuration;
	Vector2		frameSize;
};

class CBaseSkillScript : public CScript
{
public:
	CBaseSkillScript();
	virtual ~CBaseSkillScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);

	void attack(Skill_State state);
	void Dead();
	void OnAnimationCycleComplete();

	void RestartCurrentAnimation();

	void EndSkillAnimation();
	void FinishAndHide();

	void PlayerSkillAttack(CMonster* _pMonster);

	void SetAnimationData(Skill_State state, const SkillAnimInfo& data)
	{
		m_AnimMap[state] = data;
		char dbg[256];
		sprintf_s(dbg, "[DEBUG] SetAnimationData called state=%d, current animMapSize=%d, this=%p\n",
			(int)state, (int)m_AnimMap.size(), this);
		OutputDebugStringA(dbg);
	}

	void SetSkillOwner(CBaseSkill* _pSkillOwner) { m_pSkillOwner = _pSkillOwner; }

private:
	wstring				m_strCurAnimName; // 현재 재생 중인 애니메이션 이름
	CBaseSkill* m_pSkillOwner = nullptr;	// 2025-05-26
	CTransform* m_pTransform = nullptr;
	CBackGround* m_pBackGround = nullptr;
	unordered_map<Skill_State, SkillAnimInfo> m_AnimMap;
	int m_iDir = 1;

	bool m_bRestartRequested = false; // 애니메이션이 끝난 뒤 재시작 예약 여부
	bool m_bIsAnimating = false;
	CGameObject* m_pColliderMonster = nullptr;
	bool		m_bHasDealtDamage = false;
};