#pragma once
#include "CRigidBody.h"
#include "../Object/CBackGround.h"
#include "../Object/CPlayer.h"

class CPlayer;
class CBackGround;
class CGravity : public CRigidBody
{
public:
	CGravity();
	virtual ~CGravity();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	void Jump(float _fJumpForce);

	void SetGround(bool _bGround) { m_bGround = _bGround; }
	/*bool IsOnGround() const { return m_bGround; }*/

	//bool CheckGround(CBackGround* pGround, Vector3 _fPlusCheckPos);

	void SetGravityEnabled(bool _bEnable)
	{
		m_bIsGravity = _bEnable;

		if (!_bEnable)
		{
			// 중력이 꺼지면 수직 속도를 0으로 설정하여 멈춤
			m_vVelocity.y = 0.0f;
		}
	}

	void Gravity();

	void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }

private:
	bool		m_bIsGravity;
	bool		m_bGround;

	float		m_fGravityForce;
	float		m_fGravitySpeed;
	Vector2		m_vecMoveForce;

	float		m_fMaxGravity;

	Vector2		m_vGravity;

	CTransform* m_pTransform;

	bool IsGroundCheck = true;
	float SkipGround = 0.0f;
	set<float> NotGround;

	bool m_bPrevGround;

	CPlayer* m_pPlayer;
};