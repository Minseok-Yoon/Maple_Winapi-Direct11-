#pragma once
#include "../Entity/CEntity.h"
#include "../Component/CComponent.h"

using namespace math;
class CTransform : public CComponent
{
public:
	CTransform();
	~CTransform();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void Bind();

	CTransform* GetParent() { return m_pParent; }
	const math::Matrix GetWorldMatrix() { return m_tWorldMatrix; }

	void SetPosition(float _fX, float _fY, float _fZ) { m_vPosition = Vector3(_fX, _fY, _fZ); }
	void SetPosition(Vector3 _vPosition) { m_vPosition = _vPosition; }
	const Vector3 GetPosition() { return m_vPosition; }

	void SetScale(float _fX, float _fY, float _fZ) { m_vScale = Vector3(_fX, _fY, _fZ); }
	void SetScale(Vector3 _vScale) { m_vScale = _vScale; }
	const Vector3 GetScale() { return m_vScale; }

	void SetRotation(float _fX, float _fY, float _fZ) { m_vRotation = Vector3(_fX, _fY, _fZ); }
	void SetRotation(Vector3 _vRotation) { m_vRotation = _vRotation; }
	const Vector3 GetRotation() { return m_vRotation; }

	const Vector3 Forward() { return m_vForward; }
	const Vector3 Right() { return m_vRight; }
	const Vector3 Up() { return m_vUp; }

private:
	CTransform*		m_pParent;
	math::Matrix	m_tWorldMatrix;

	// 위치, 크기, 회전
	Vector3	m_vPosition;	// 오브젝트의 위치
	Vector3	m_vScale;		// 크기 배율
	Vector3	m_vRotation;	// 회전 각도(degree)

	Vector3	m_vForward;
	Vector3	m_vRight;
	Vector3	m_vUp;
};