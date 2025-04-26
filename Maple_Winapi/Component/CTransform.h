#pragma once
#include "../Entity/CEntity.h"
#include "../Component/CComponent.h"
#include "CCollider.h"
#include "../Component/CCamera.h"

using namespace math;

class CTransform : public CComponent
{
public:
	CTransform();
	virtual ~CTransform();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	void Bind(const Matrix& view, const Matrix& projection) const;
	void LookAt(const Vector3& targetPosition);

	// Ʈ������ ������Ʈ �Լ�
	void UpdateTransform();

	// ���� ��ȯ �Լ�
	Vector3 GetLocalScale() { return m_vLocalScale; }
	void SetLocalScale(const Vector3 _vLocalScale) { m_vLocalScale = _vLocalScale; UpdateTransform(); }
	Vector3 GetLocalRotation() { return m_vLocalRotation; }
	void SetLocalRotation(const Vector3 _vLocalRotation) { m_vLocalRotation = _vLocalRotation; UpdateTransform(); }
	Vector3 GetLocalPosition() { return m_vLocalPosition; }
	void SetLocalPosition(const Vector3 _vLocalPosition) { m_vLocalPosition = _vLocalPosition; UpdateTransform(); }

	void AddLocalPosition(const Vector3& _Value) { m_vLocalPosition += _Value; }

	// ���� ��ȯ �Լ�
	Vector3 GetWorldScale() { return m_vWorldScale; }
	void SetWorldScale(const Vector3 _vWorldScale);
	Vector3 GetWorldRotation() { return m_vWorldRotation; }
	void SetWorldRotation(const Vector3 _vWorldRotation);
	Vector3 GetWorldPosition() { return m_vWorldPosition; }
	void SetWorldPosition(const Vector3 _vWorldPosition);

	// ���� ��� ��ȯ
	Matrix GetWorldMatrix() const { return m_tWorldMatrix; }
	void SetWorldMatrix(const Matrix& worldMatrix) { m_tWorldMatrix = worldMatrix; }

	//// �θ�-�ڽ� ����
	bool HasParent() { return m_pParent != nullptr; }
	CTransform* GetParent() { return m_pParent; }
	void SetParent(CTransform* _pParent) { m_pParent = _pParent; }
	vector<CTransform*> GetChildren() { return m_vecChildren; }
	void AddChildren(vector<CTransform*> _vecChildren) { m_vecChildren = _vecChildren; }

	// ���� ���� ��������
	Vector3 Forward() const { return m_vForward; }
	Vector3 Right() const { return m_vRight; }
	Vector3 Up() const { return m_vUp; }

	void SetIgnoreParentScale(bool _bIgnore) { m_bIgnoreParentScale = _bIgnore; }

private:
	// �θ�� �ڽ� Ʈ������
	CTransform*			m_pParent;
	vector<CTransform*>	m_vecChildren;

	// ���� �� ���� ��ȯ ���
	Matrix	m_tLocalMatrix = Matrix::Identity;
	Matrix  m_tWorldMatrix = Matrix::Identity;

	// ���� ��ȯ �Ӽ�
	Vector3 m_vLocalScale = Vector3::One;
	Vector3 m_vLocalRotation = Vector3::Zero;
	Vector3 m_vLocalPosition = Vector3::Zero;
	
	Vector3 m_vWorldScale = Vector3::One;
	Vector3 m_vWorldRotation = Vector3::Zero;
	Vector3 m_vWorldPosition = Vector3::Zero;

	// ���� ����
	Vector3 m_vForward;
	Vector3 m_vRight;
	Vector3 m_vUp;

	bool m_bIgnoreParentScale = false;
};

// ������ǥ(Local Coordinate) - Ư�� ������Ʈ�� �ڽŸ��� ��ǥ��
// ������Ʈ�� ������ �Ǹ�, �ڽ��� �߽�(Origin)�� (0, 0, 0)���� �ΰ� �����Ǵ� ��ǥ

// ������ǥ(World Coordinate) - ������ ��ü ���� ���������� �������� ��ġ.

//Vector3 GetTransformPosition() { return m_vPosition; }

//// 3D ��ġ ���� �� ��������
//void SetPosition(const float _fX, const float _fY, const float _fZ) { m_vPosition = Vector3(_fX, _fY, _fZ); }

//// 2D ��ġ ���� �� ��������
//void SetPosition2D(const float _fX, const float _fY) { m_vPosition = Vector3(_fX, _fY, m_vPosition.z); }
//void SetPosition2D(const Vector2& _vPosition) { m_vPosition = Vector3(_vPosition.x, _vPosition.y, m_vPosition.z); }
//Vector2 GetPosition2D() const { return Vector2(m_vPosition.x, m_vPosition.y); }

//void SetScale(const float _fX, const float _fY, const float _fZ) { m_vScale = Vector3(_fX, _fY, _fZ); }

//// Vector2 ������ Set/Get
//void SetScale2D(const float _fX, const float _fY) { m_vScale = Vector3(_fX, _fY, m_vScale.z); }
//void SetScale2D(const Vector2& _vScale) { m_vScale = Vector3(_vScale.x, _vScale.y, m_vScale.z); }

//// ȸ�� ���� �� ��������
//void SetRotation(const float _fX, const float _fY, const float _fZ) { m_vRotation = Vector3(_fX, _fY, _fZ); }
//void SetRotation(const Vector3& _vRotation) { m_vRotation = _vRotation; }
//Vector3 GetRotation() const { return m_vRotation; }

//// Vector2 ������ Set/Get
//void SetRotation2D(const float _fX, const float _fY) { m_vRotation = Vector3(_fX, _fY, m_vRotation.z); }
//void SetRotation2D(const Vector2& _vRotation) { m_vRotation = Vector3(_vRotation.x, _vRotation.y, m_vRotation.z); }
//Vector2 GetRotation2D() const { return Vector2(m_vRotation.x, m_vRotation.y); }

/*// Local ��ǥ �Լ� �߰�
void SetLocalPosition(const Vector3& localPos)
{
	if (m_pParent == nullptr)
	{
		// �θ� ������ ���� ��ǥ�� �״�� ������
		m_vPosition = localPos;
	}
	else
	{
		// �θ� ������, �θ��� ���� ����� �ݿ��Ͽ� ���� ��ǥ�� ��ȯ
		Matrix parentWorldMatrix = m_pParent->GetWorldMatrix();

		// ���� ��ǥ�� ���� ��ǥ�� ��ȯ: �θ��� ���� ����� �ݿ��ؼ� ���
		m_vPosition = Vector3::Transform(localPos, parentWorldMatrix.Invert());
	}
}
Vector3 GetLocalPosition() const { return m_vPosition; }

void SetWorldPosition(const Vector3& worldPos)
{
	if (m_pParent == nullptr)
	{
		// �θ� ������ ���� ��ǥ = ���� ��ǥ
		m_vPosition = worldPos;
	}
	else
	{
		// �θ� ������, �θ��� ���� ����� ����ȯ�Ͽ� ���� ��ǥ�� ��ȯ
		Matrix parentWorldMatrix = m_pParent->GetWorldMatrix().Invert();
		m_vPosition = Vector3::Transform(worldPos, parentWorldMatrix);
	}
}

Vector3 GetWorldPosition() const
{
	if (m_pParent == nullptr)
	{
		// �θ� ������ ���� ��ǥ = ���� ��ǥ
		return m_vPosition;
	}
	else
	{
		// �θ� ������ �θ��� ���� ����� ���ؼ� ���� ��ǥ�� ����
		return Vector3::Transform(m_vPosition, m_pParent->GetWorldMatrix());
	}
}

// ���� ������ ���� �� ��������
void SetLocalScale(const Vector3& _vScale) { m_vScale = _vScale; }
Vector3 GetLocalScale() const { return m_vScale; }

// ���� ������ ����
void SetWorldScale(const Vector3& worldScale)
{
	if (m_pParent == nullptr)
	{
		m_vScale = worldScale;  // �θ� ������ �״�� ���� ������ ����
	}
	else
	{
		Vector3 parentScale = m_pParent->GetWorldScale();
		m_vScale = worldScale / parentScale;  // �θ� �������� ������ ���� ������ ����
	}
}

// ���� ������ ��������
Vector3 GetWorldScale() const
{
	if (m_pParent == nullptr)
	{
		return m_vScale;  // �θ� ������ ���� ������ = ���� ������
	}
	else
	{
		return m_pParent->GetWorldScale() * m_vScale;  // �θ��� ���� �����ϰ� ����
	}
}*/


//// ���� ������ ���� �� ��������
//void SetLocalScale(const Vector3& _vScale) { m_vScale = _vScale; }
//Vector3 GetLocalScale() const { return m_vScale; }

//// ���� ������ ����
//void SetWorldScale(const Vector3& worldScale)
//{
//	if (m_pParent == nullptr)
//	{
//		m_vScale = worldScale;  // �θ� ������ �״�� ���� ������ ����
//	}
//	else
//	{
//		Vector3 parentScale = m_pParent->GetWorldScale();
//		m_vScale = worldScale / parentScale;  // �θ� �������� ������ ���� ������ ����
//	}
//}

//// ���� ������ ��������
//Vector3 GetWorldScale() const
//{
//	if (m_pParent == nullptr)
//	{
//		return m_vScale;  // �θ� ������ ���� ������ = ���� ������
//	}
//	else
//	{
//		return m_pParent->GetWorldScale() * m_vScale;  // �θ��� ���� �����ϰ� ����
//	}
//}

// 2025-03-18 �ֽź�
/*void SetParent(CTransform* _pParent)
{
	if (this == nullptr)
	{
		OutputDebugStringA("Error: this is nullptr in SetParent!\n");
		return;
	}

	m_pParent = _pParent;
	UpdateWorldPosition(); // �θ� ����Ǹ� ���� ��ǥ ����
}

CTransform* GetParent() { return m_pParent; }

// ���� ��ȯ ���� ����
void SetWorldMatrix(const Matrix& _tWorldMatrix) { m_tWorldMatrix = _tWorldMatrix; }
Matrix GetWorldMatrix() const { return m_tWorldMatrix; }

//// 3D ��ġ ���� �� ��������
void SetPosition(const Vector3& _vPosition) { m_vLocalPosition = _vPosition; }
Vector3 GetPosition() const { return m_vLocalPosition; }

void SetScale(const Vector3& _vScale) { m_vLocalScale = _vScale; }
Vector3 GetScale() const { return m_vLocalScale; }
Vector2 GetScale2D() const { return Vector2(m_vLocalScale.x, m_vLocalScale.y); }

// ���� ���� ��������
Vector3 Forward() const { return m_vForward; }
Vector3 Right() const { return m_vRight; }
Vector3 Up() const { return m_vUp; }

Vector3 GetLocalPos() const { return m_vLocalPosition; }
Vector2 GetLocalPos2D() const { return Vector2(m_vLocalPosition.x, m_vLocalPosition.y); }

void SetLocalPosition(const Vector3& localPos)
{
	m_vLocalPosition = localPos;

	char debugMsg[256];
	sprintf_s(debugMsg, "SetLocalPosition: (%f, %f, %f)\n", m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);
	OutputDebugStringA(debugMsg);

	UpdateWorldPosition();
}

Vector3 GetLocalPosition() const { return m_vLocalPosition; }

void SetWorldPosition(const Vector3& worldPos)
{
	if (m_pParent)
	{
		// �θ��� ���� ����� ����Ͽ� ���� ��ǥ ���
		Matrix parentWorldMatrix = m_pParent->GetWorldMatrix();
		Matrix invParentWorldMatrix = parentWorldMatrix.Invert();
		m_vLocalPosition = Vector3::Transform(worldPos, invParentWorldMatrix);
	}
	else
	{
		// �θ� ������ ���� ��ǥ�� ����
		m_vLocalPosition = worldPos;
	}

	UpdateWorldPosition();  // ����� ���� ��ǥ�� �ݿ��Ͽ� ���� ��ǥ ����
}

Vector3 GetWorldPosition() const { return m_vWorldPosition; }

void SetLocalScale(const Vector3& localScale)
{
	m_vLocalScale = localScale;
	UpdateWorldScale();
}

Vector3 GetLocalScale() const { return m_vLocalScale; }

void SetWorldScale(const Vector3& worldScale)
{
	if (m_pParent == nullptr)
	{
		m_vLocalScale = worldScale;
	}
	else
	{
		Vector3 parentScale = m_pParent->GetWorldScale();
		m_vLocalScale = worldScale / parentScale;
	}

	UpdateWorldScale();
}

Vector3 GetWorldScale() const { return m_vWorldScale; }*/