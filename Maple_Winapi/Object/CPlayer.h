#pragma once
#include "../Object/CGameObject.h"
#include "../Object/CMonster.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	bool CheckGround(Vector3 _fPlusCheckPos);

private:
	bool	m_bAttackCycle;
	float	m_fAttackDelayTime;		// 공격 후 대기 시간
	float	m_fElapsedTime;			// 경과 시간

	class CCollider* m_pCollider;

	// 지면 충돌 관련 변수 (2025-03-31 추가)
	set<float> NotGround;
	float SkipGround = 0.0f;
	bool IsGroundCheck = true;

	// 2025-05-24(플레이어 Hp, Mp 추가)
	static int PlayerHp;
	static int PlayerMp;
};