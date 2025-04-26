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

	// 트렌스폼 업데이트 함수
	void UpdateTransform();

	// 로컬 변환 함수
	Vector3 GetLocalScale() { return m_vLocalScale; }
	void SetLocalScale(const Vector3 _vLocalScale) { m_vLocalScale = _vLocalScale; UpdateTransform(); }
	Vector3 GetLocalRotation() { return m_vLocalRotation; }
	void SetLocalRotation(const Vector3 _vLocalRotation) { m_vLocalRotation = _vLocalRotation; UpdateTransform(); }
	Vector3 GetLocalPosition() { return m_vLocalPosition; }
	void SetLocalPosition(const Vector3 _vLocalPosition) { m_vLocalPosition = _vLocalPosition; UpdateTransform(); }

	void AddLocalPosition(const Vector3& _Value) { m_vLocalPosition += _Value; }

	// 월드 변환 함수
	Vector3 GetWorldScale() { return m_vWorldScale; }
	void SetWorldScale(const Vector3 _vWorldScale);
	Vector3 GetWorldRotation() { return m_vWorldRotation; }
	void SetWorldRotation(const Vector3 _vWorldRotation);
	Vector3 GetWorldPosition() { return m_vWorldPosition; }
	void SetWorldPosition(const Vector3 _vWorldPosition);

	// 월드 행렬 반환
	Matrix GetWorldMatrix() const { return m_tWorldMatrix; }
	void SetWorldMatrix(const Matrix& worldMatrix) { m_tWorldMatrix = worldMatrix; }

	//// 부모-자식 관계
	bool HasParent() { return m_pParent != nullptr; }
	CTransform* GetParent() { return m_pParent; }
	void SetParent(CTransform* _pParent) { m_pParent = _pParent; }
	vector<CTransform*> GetChildren() { return m_vecChildren; }
	void AddChildren(vector<CTransform*> _vecChildren) { m_vecChildren = _vecChildren; }

	// 방향 벡터 가져오기
	Vector3 Forward() const { return m_vForward; }
	Vector3 Right() const { return m_vRight; }
	Vector3 Up() const { return m_vUp; }

	void SetIgnoreParentScale(bool _bIgnore) { m_bIgnoreParentScale = _bIgnore; }

private:
	// 부모와 자식 트랜스폼
	CTransform*			m_pParent;
	vector<CTransform*>	m_vecChildren;

	// 로컬 및 월드 변환 행렬
	Matrix	m_tLocalMatrix = Matrix::Identity;
	Matrix  m_tWorldMatrix = Matrix::Identity;

	// 로컬 변환 속성
	Vector3 m_vLocalScale = Vector3::One;
	Vector3 m_vLocalRotation = Vector3::Zero;
	Vector3 m_vLocalPosition = Vector3::Zero;
	
	Vector3 m_vWorldScale = Vector3::One;
	Vector3 m_vWorldRotation = Vector3::Zero;
	Vector3 m_vWorldPosition = Vector3::Zero;

	// 방향 벡터
	Vector3 m_vForward;
	Vector3 m_vRight;
	Vector3 m_vUp;

	bool m_bIgnoreParentScale = false;
};

// 로컬좌표(Local Coordinate) - 특정 오브젝트의 자신만의 좌표계
// 오브젝트가 기준이 되며, 자신의 중심(Origin)을 (0, 0, 0)으로 두고 측정되는 좌표

// 월드좌표(World Coordinate) - 게임의 전체 월드 공간에서의 절대적인 위치.

//Vector3 GetTransformPosition() { return m_vPosition; }

//// 3D 위치 설정 및 가져오기
//void SetPosition(const float _fX, const float _fY, const float _fZ) { m_vPosition = Vector3(_fX, _fY, _fZ); }

//// 2D 위치 설정 및 가져오기
//void SetPosition2D(const float _fX, const float _fY) { m_vPosition = Vector3(_fX, _fY, m_vPosition.z); }
//void SetPosition2D(const Vector2& _vPosition) { m_vPosition = Vector3(_vPosition.x, _vPosition.y, m_vPosition.z); }
//Vector2 GetPosition2D() const { return Vector2(m_vPosition.x, m_vPosition.y); }

//void SetScale(const float _fX, const float _fY, const float _fZ) { m_vScale = Vector3(_fX, _fY, _fZ); }

//// Vector2 버전의 Set/Get
//void SetScale2D(const float _fX, const float _fY) { m_vScale = Vector3(_fX, _fY, m_vScale.z); }
//void SetScale2D(const Vector2& _vScale) { m_vScale = Vector3(_vScale.x, _vScale.y, m_vScale.z); }

//// 회전 설정 및 가져오기
//void SetRotation(const float _fX, const float _fY, const float _fZ) { m_vRotation = Vector3(_fX, _fY, _fZ); }
//void SetRotation(const Vector3& _vRotation) { m_vRotation = _vRotation; }
//Vector3 GetRotation() const { return m_vRotation; }

//// Vector2 버전의 Set/Get
//void SetRotation2D(const float _fX, const float _fY) { m_vRotation = Vector3(_fX, _fY, m_vRotation.z); }
//void SetRotation2D(const Vector2& _vRotation) { m_vRotation = Vector3(_vRotation.x, _vRotation.y, m_vRotation.z); }
//Vector2 GetRotation2D() const { return Vector2(m_vRotation.x, m_vRotation.y); }

/*// Local 좌표 함수 추가
void SetLocalPosition(const Vector3& localPos)
{
	if (m_pParent == nullptr)
	{
		// 부모가 없으면 로컬 좌표는 그대로 설정됨
		m_vPosition = localPos;
	}
	else
	{
		// 부모가 있으면, 부모의 월드 행렬을 반영하여 로컬 좌표로 변환
		Matrix parentWorldMatrix = m_pParent->GetWorldMatrix();

		// 월드 좌표를 로컬 좌표로 변환: 부모의 월드 행렬을 반영해서 계산
		m_vPosition = Vector3::Transform(localPos, parentWorldMatrix.Invert());
	}
}
Vector3 GetLocalPosition() const { return m_vPosition; }

void SetWorldPosition(const Vector3& worldPos)
{
	if (m_pParent == nullptr)
	{
		// 부모가 없으면 월드 좌표 = 로컬 좌표
		m_vPosition = worldPos;
	}
	else
	{
		// 부모가 있으면, 부모의 월드 행렬을 역변환하여 로컬 좌표로 변환
		Matrix parentWorldMatrix = m_pParent->GetWorldMatrix().Invert();
		m_vPosition = Vector3::Transform(worldPos, parentWorldMatrix);
	}
}

Vector3 GetWorldPosition() const
{
	if (m_pParent == nullptr)
	{
		// 부모가 없으면 월드 좌표 = 로컬 좌표
		return m_vPosition;
	}
	else
	{
		// 부모가 있으면 부모의 월드 행렬을 곱해서 월드 좌표를 구함
		return Vector3::Transform(m_vPosition, m_pParent->GetWorldMatrix());
	}
}

// 로컬 스케일 설정 및 가져오기
void SetLocalScale(const Vector3& _vScale) { m_vScale = _vScale; }
Vector3 GetLocalScale() const { return m_vScale; }

// 월드 스케일 설정
void SetWorldScale(const Vector3& worldScale)
{
	if (m_pParent == nullptr)
	{
		m_vScale = worldScale;  // 부모가 없으면 그대로 로컬 스케일 적용
	}
	else
	{
		Vector3 parentScale = m_pParent->GetWorldScale();
		m_vScale = worldScale / parentScale;  // 부모 스케일을 나누어 로컬 스케일 조정
	}
}

// 월드 스케일 가져오기
Vector3 GetWorldScale() const
{
	if (m_pParent == nullptr)
	{
		return m_vScale;  // 부모가 없으면 로컬 스케일 = 월드 스케일
	}
	else
	{
		return m_pParent->GetWorldScale() * m_vScale;  // 부모의 월드 스케일과 곱함
	}
}*/


//// 로컬 스케일 설정 및 가져오기
//void SetLocalScale(const Vector3& _vScale) { m_vScale = _vScale; }
//Vector3 GetLocalScale() const { return m_vScale; }

//// 월드 스케일 설정
//void SetWorldScale(const Vector3& worldScale)
//{
//	if (m_pParent == nullptr)
//	{
//		m_vScale = worldScale;  // 부모가 없으면 그대로 로컬 스케일 적용
//	}
//	else
//	{
//		Vector3 parentScale = m_pParent->GetWorldScale();
//		m_vScale = worldScale / parentScale;  // 부모 스케일을 나누어 로컬 스케일 조정
//	}
//}

//// 월드 스케일 가져오기
//Vector3 GetWorldScale() const
//{
//	if (m_pParent == nullptr)
//	{
//		return m_vScale;  // 부모가 없으면 로컬 스케일 = 월드 스케일
//	}
//	else
//	{
//		return m_pParent->GetWorldScale() * m_vScale;  // 부모의 월드 스케일과 곱함
//	}
//}

// 2025-03-18 최신본
/*void SetParent(CTransform* _pParent)
{
	if (this == nullptr)
	{
		OutputDebugStringA("Error: this is nullptr in SetParent!\n");
		return;
	}

	m_pParent = _pParent;
	UpdateWorldPosition(); // 부모가 변경되면 월드 좌표 갱신
}

CTransform* GetParent() { return m_pParent; }

// 월드 변환 형렬 설정
void SetWorldMatrix(const Matrix& _tWorldMatrix) { m_tWorldMatrix = _tWorldMatrix; }
Matrix GetWorldMatrix() const { return m_tWorldMatrix; }

//// 3D 위치 설정 및 가져오기
void SetPosition(const Vector3& _vPosition) { m_vLocalPosition = _vPosition; }
Vector3 GetPosition() const { return m_vLocalPosition; }

void SetScale(const Vector3& _vScale) { m_vLocalScale = _vScale; }
Vector3 GetScale() const { return m_vLocalScale; }
Vector2 GetScale2D() const { return Vector2(m_vLocalScale.x, m_vLocalScale.y); }

// 방향 벡터 가져오기
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
		// 부모의 월드 행렬을 고려하여 로컬 좌표 계산
		Matrix parentWorldMatrix = m_pParent->GetWorldMatrix();
		Matrix invParentWorldMatrix = parentWorldMatrix.Invert();
		m_vLocalPosition = Vector3::Transform(worldPos, invParentWorldMatrix);
	}
	else
	{
		// 부모가 없으면 로컬 좌표와 동일
		m_vLocalPosition = worldPos;
	}

	UpdateWorldPosition();  // 변경된 로컬 좌표를 반영하여 월드 좌표 갱신
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