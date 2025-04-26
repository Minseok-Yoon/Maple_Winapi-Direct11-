#pragma once
#include "../Object/CGameObject.h"
#include "../Component/CGravity.h"

class CGravity;
class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	void GroundCheck();

	bool CheckGround(Vector3 _fPlusCheckPos);

private:
	bool	m_bAttackCycle;
	float	m_fAttackDelayTime;		// ���� �� ��� �ð�
	float	m_fElapsedTime;			// ��� �ð�

	class CCollider* m_pCollider;

	// ���� �浹 ���� ���� (2025-03-31 �߰�)
	set<float> NotGround;
	float SkipGround = 0.0f;
	bool IsGroundCheck = true;


	CGravity* m_pGravity = nullptr;
};