#pragma once
#include "CBaseSkill.h"

class CPlayer;
class Trinity : public CBaseSkill
{
public:
	Trinity();
	virtual ~Trinity();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;
	void Reset();

	void UseSkill();
	void EndSkill();

	CCollider* GetDetectedCollider() const { return m_pDetectedCollision; }
	CCollider* GetHitCollider() const { return m_pHitCollision; }

	void SetOwnerPlayer(CPlayer* player) { m_pOwnerPlayer = player; }

private:
	void startcombo();
	void nextstage();
	void resetcombo();

	int currentStage = 0;
	float comboTimer = 0.0f;
	float maxComboTime = 1.0f;
	float cooldown = 0.0f;
	float currentCooldown = 0.0f;
	Skill_State state = Skill_State::SS_Idle;
	std::vector<float> damages;

	CCollider* m_pDetectedCollision = nullptr;
	CCollider* m_pHitCollision = nullptr;
	CPlayer* m_pOwnerPlayer = nullptr;
};

// ��ų �ִϸ��̼��� 2�� ���ư��� ������ �����ΰ�?
// �̰��� ��� �ذ��ϸ� ������