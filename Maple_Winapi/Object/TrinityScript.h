#pragma once
#include "../Component/CScript.h"

//struct SkillAnimInfo
//{
//	wstring		pathPattern;
//	int			frameCount;
//	float		frameDuration;
//	Vector2		frameSize;
//};

class Trinity;
class TrinityScript : public CScript
{
public:
	TrinityScript();
	virtual ~TrinityScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	virtual void OnCollisionEnter(class CCollider* _pOther);
	virtual void OnCollisionStay(class CCollider* _pOther);
	virtual void OnCollisionExit(class CCollider* _pOther);

	void attack(Skill_State state);

	void ActivateHitbox();

	void DeactivateHitbox();

	void UpdateHitboxPosition();

	void SetSkillAnimationData(Skill_State state, const SkillAnimInfo& data)
	{
		m_SkillAniMap[state] = data;
		OutputDebugStringA("Animation data set for state\n");
	}

private:
	Trinity* skill;
	bool hitboxActive = false;
	float hitboxDuration = 0.2f;     // 히트박스 유지 시간

	unordered_map<Skill_State, SkillAnimInfo> m_SkillAniMap;
	int					m_iDir = 1;
	wstring				m_strCurAnimName;

	float attackTimer = 0.0f;        // 현재 공격 타이머
	// 근데 프레임 단위로 유지할 것인데 굳이 타이머를 따로 돌려야 할 필요가 있나?
};