#pragma once
#include "../Entity/CEntity.h"
#include "../Component/CComponent.h"

using namespace math;
class CTransform : public CComponent
{
public:
	CTransform();
	virtual ~CTransform();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void Bind() const;

	void LookAt(const Vector3& targetPosition);

	void SetParent(CTransform* _pParent) { m_pParent = _pParent; }
	CTransform* GetParent() { return m_pParent; }

	void SetWorldMatrix(const Matrix& _tWorldMatrix) { m_tWorldMatrix = _tWorldMatrix; }
	Matrix GetWorldMatrix() const { return m_tWorldMatrix; }

	void SetPosition(const float _fX, const float _fY, const float _fZ) { m_vPosition = Vector3(_fX, _fY, _fZ); }
	void SetPosition(const Vector3& _vPosition) { m_vPosition = _vPosition; }
	Vector3 GetPosition() const { return m_vPosition; }

	void SetScale(const float _fX, const float _fY, const float _fZ) { m_vScale = Vector3(_fX, _fY, _fZ); }
	void SetScale(const Vector3& _vScale) { m_vScale = _vScale; }
	Vector3 GetScale() const { return m_vScale; }

	void SetRotation(const float _fX, const float _fY, const float _fZ) { m_vRotation = Vector3(_fX, _fY, _fZ); }
	void SetRotation(const Vector3& _vRotation) { m_vRotation = _vRotation; }
	Vector3 GetRotation() const { return m_vRotation; }

	Vector3 Forward() const { return m_vForward; }
	Vector3 Right() const { return m_vRight; }
	Vector3 Up() const { return m_vUp; }

private:
	CTransform*		m_pParent;
	Matrix			m_tWorldMatrix;

	// 위치, 크기, 회전
	Vector3	m_vPosition;	// 오브젝트의 위치
	Vector3	m_vScale;		// 크기 배율
	Vector3	m_vRotation;	// 회전 각도(degree)

	Vector3	m_vForward;
	Vector3	m_vRight;
	Vector3	m_vUp;
};