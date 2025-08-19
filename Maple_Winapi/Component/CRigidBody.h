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

// 2025-06-13
// 1. 얼음 지형이 아님에도 불구하고 가속도 때문에 미끄러짐 (살짝 완료)
// 1.1. 속도에 가속도를 더하면 더해지는 가속도 만큼 속도가 점점 빨라진다.
// -> 그렇다면 가속도를 계속 누적해서 더하는 것이 아닌 한번만 적용하면 어떨까?
// -> 다행히 완벽히 원하는 대로는 아니지만 나름 구현이 됨.
// 2. 아직 점프 기능이 재대로 구현이 되어 있지 않음
// -> alt키를 누르면 위쪽으로 올라가도록 구현은 했음.
// 2.1 점프 중일 때에도 중력이 적용되도록 해야 할듯
// 2~3번 반복해서 눌러야 제대로 적용이 됨.
// 3. 왼쪽을 보면 애니메이션도 그에 맞게 왼쪽 애니메이션이 재생이 되어야 함.
// AddForce() 힘을 더한다라는 함수인데, 속도를 일정하게 하려면 어떻게 해야 할까?
// 4. 분명 맵을 나갔다가 다시 들어오면 비활성화 되고 있던 것들이 활성화가 되는 것인데
// 왜 한번 더 추가가 될까?