#pragma once
#include "../Object/CGameObject.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	bool	m_bAttackCycle;
	float	m_fAttackDelayTime;		// 공격 후 대기 시간
	float	m_fElapsedTime;			// 경과 시간
};