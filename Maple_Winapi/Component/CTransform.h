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

	// ���� ��ȯ ���� ����
	void SetWorldMatrix(const Matrix& _tWorldMatrix) { m_tWorldMatrix = _tWorldMatrix; }
	Matrix GetWorldMatrix() const { return m_tWorldMatrix; }

	// 3D ��ġ ���� �� ��������
	void SetPosition(const float _fX, const float _fY, const float _fZ) { m_vPosition = Vector3(_fX, _fY, _fZ); }
	void SetPosition(const Vector3& _vPosition) { m_vPosition = _vPosition; }
	Vector3 GetPosition() const { return m_vPosition; }

	// 2D ��ġ ���� �� ��������
	void SetPosition2D(const float _fX, const float _fY) { m_vPosition = Vector3(_fX, _fY, m_vPosition.z); }
	void SetPosition2D(const Vector2& _vPosition) { m_vPosition = Vector3(_vPosition.x, _vPosition.y, m_vPosition.z); }
	Vector2 GetPosition2D() const { return Vector2(m_vPosition.x, m_vPosition.y); }

	void SetScale(const float _fX, const float _fY, const float _fZ) { m_vScale = Vector3(_fX, _fY, _fZ); }
	void SetScale(const Vector3& _vScale) { m_vScale = _vScale; }
	Vector3 GetScale() const { return m_vScale; }

	// Vector2 ������ Set/Get
	void SetScale2D(const float _fX, const float _fY) { m_vScale = Vector3(_fX, _fY, m_vScale.z); }
	void SetScale2D(const Vector2& _vScale) { m_vScale = Vector3(_vScale.x, _vScale.y, m_vScale.z); }
	Vector2 GetScale2D() const { return Vector2(m_vScale.x, m_vScale.y); }

	// ȸ�� ���� �� ��������
	void SetRotation(const float _fX, const float _fY, const float _fZ) { m_vRotation = Vector3(_fX, _fY, _fZ); }
	void SetRotation(const Vector3& _vRotation) { m_vRotation = _vRotation; }
	Vector3 GetRotation() const { return m_vRotation; }

	// ���� ���� ��������
	Vector3 Forward() const { return m_vForward; }
	Vector3 Right() const { return m_vRight; }
	Vector3 Up() const { return m_vUp; }

private:
	CTransform*		m_pParent;			// �θ� Transform ������
	Matrix			m_tWorldMatrix;		// ���� ��ȯ ���

	// ��ġ, ũ��, ȸ��
	Vector3	m_vPosition;	// ������Ʈ�� ��ġ
	Vector3	m_vScale;		// ũ�� ����
	Vector3	m_vRotation;	// ȸ�� ����(degree)

	Vector3	m_vForward;
	Vector3	m_vRight;
	Vector3	m_vUp;
};