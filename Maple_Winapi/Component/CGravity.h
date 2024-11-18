#pragma once
#include "CRigidBody.h"

class CGravity : public CRigidBody
{
public:
	CGravity();
	virtual ~CGravity();

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

	void Jump(float _fJumpForce);

	void SetGround(bool _bGround) { m_bGround = _bGround; }
	bool IsOnGround() const { return m_bGround; }

private:
	bool		m_bGround;
	Vector2		m_vGravity;
};