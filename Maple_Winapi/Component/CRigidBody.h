#pragma once
#include "CComponent.h"

class CRigidBody : public CComponent
{
public:
	CRigidBody();
	~CRigidBody();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(const Matrix& view, const Matrix& projection);

	void Move();

	void SetMass(float _fMass) { m_fMass = _fMass; }
	void AddForce(Vector2 _vForce) { m_vForce = _vForce; }

	void SetVelocity(Vector2 _vVelocity) { m_vVelocity = _vVelocity; }
	Vector2 GetVelocity() { return m_vVelocity; }

	void SetVelocity3D(Vector3 _vVelocity) { m_vVelocity3D = _vVelocity; }
	Vector3 GetVelocity3D() { return m_vVelocity3D; }

	void ClearForce();

protected:
	float	m_fMass;		// 질량
	float	m_fFriction;	// 마찰 계수

	Vector2	m_vForce;		// 크기, 방향
	Vector2	m_vAccel;		// 가속도
	Vector2	m_vVelocity;	// 속도(크기 : 속력, 방향)
	Vector2	m_vMaxVelocity;	// 최대 속도

	Vector3 m_vVelocity3D;

	bool	m_bApplyFriction;
};