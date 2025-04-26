#include "CTransform.h"
#include "../Graphics/CGraphics.h"
#include "../Graphics/CConstantBuffer.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"

CTransform::CTransform() :
	CComponent(COMPONENT_TYPE::CT_Transform),
	m_tWorldMatrix(Matrix::Identity),
	m_pParent(nullptr)
{
}

CTransform::~CTransform()
{
}

void CTransform::Init()
{
}

void CTransform::Update()
{
	UpdateTransform();
}

void CTransform::LateUpdate()
{
}

void CTransform::Render(const Matrix& view, const Matrix& projection)
{
}

void CTransform::Bind(const Matrix& view, const Matrix& projection) const 
{
	TransformCB cbData = {};
	cbData.world = m_tWorldMatrix;
	cbData.view = view;
	cbData.projection = projection;

	CConstantBuffer* cb = renderer::constantBuffers[CBSLOT_TRANSFORM];
	cb->SetData(&cbData);
	cb->Bind(SHADER_STAGE::SS_ALL);
}

void CTransform::LookAt(const Vector3& targetPosition)
{
	// 타겟 방향 벡터 계산
	Vector3 direction = targetPosition - m_vWorldPosition;  // 타겟 위치에서 현재 위치를 뺀 벡터
	direction.Normalize();  // 방향 벡터를 정규화

	// Up 벡터를 기준으로 회전할 수 있도록 Right, Up, Forward 벡터를 계산
	Vector3 up = Vector3::Up;
	Vector3 right = up.Cross(direction);  // Up 벡터와 방향 벡터의 외적
	right.Normalize();  // 정규화

	Vector3 newUp = direction.Cross(right);  // 방향 벡터와 Right 벡터의 외적
	newUp.Normalize();  // 정규화

	// 새로운 회전 행렬을 생성
	m_vForward = direction;
	m_vRight = right;
	m_vUp = up;

	// 회전 행렬 생성
	Matrix rotation = Matrix::Identity;
	rotation.m[0][0] = m_vRight.x;  rotation.m[0][1] = m_vRight.y;  rotation.m[0][2] = m_vRight.z;
	rotation.m[1][0] = m_vUp.x;     rotation.m[1][1] = m_vUp.y;     rotation.m[1][2] = m_vUp.z;
	rotation.m[2][0] = m_vForward.x; rotation.m[2][1] = m_vForward.y; rotation.m[2][2] = m_vForward.z;

	// 계산된 회전 행렬을 기존 회전 행렬에 반영
	m_tWorldMatrix = Matrix::CreateScale(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z) * rotation * Matrix::CreateTranslation(m_vWorldPosition);
}

void CTransform::UpdateTransform()
{
	// 스케일, 회전, 이동 행렬 생성
	Matrix scale = Matrix::CreateScale(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);
	Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vLocalRotation.x));
	rotation *= Matrix::CreateRotationY(math::Radian(m_vLocalRotation.y));
	rotation *= Matrix::CreateRotationZ(math::Radian(m_vLocalRotation.z));
	Matrix translation = Matrix::CreateTranslation(m_vLocalPosition);

	// 로컬 행렬 조합
	m_tLocalMatrix = scale * rotation * translation;

	if (HasParent())
	{
		Matrix parentWorld = m_pParent->GetWorldMatrix();

		if (m_bIgnoreParentScale)
		{
			// 부모의 회전만 따로 계산
			Matrix parentRotation = Matrix::CreateRotationX(math::Radian(m_pParent->GetLocalRotation().x));
			parentRotation *= Matrix::CreateRotationY(math::Radian(m_pParent->GetLocalRotation().y));
			parentRotation *= Matrix::CreateRotationZ(math::Radian(m_pParent->GetLocalRotation().z));

			// 부모 위치 가져오기
			Matrix parentTranslation = Matrix::CreateTranslation(m_pParent->GetWorldPosition());

			// 스케일 제외된 부모 행렬 (회전 + 위치만)
			Matrix parentNoScale = parentRotation * parentTranslation;

			// 최종 월드 행렬
			m_tWorldMatrix = m_tLocalMatrix * parentNoScale;

			// 월드 스케일은 로컬 그대로 유지
			m_vWorldScale = m_vLocalScale;
		}
		else
		{
			// 일반적인 경우: 전체 부모 행렬 곱하기
			m_tWorldMatrix = m_tLocalMatrix * parentWorld;

			Vector3 parentScale = m_pParent->GetWorldScale();
			m_vWorldScale.x = m_vLocalScale.x * parentScale.x;
			m_vWorldScale.y = m_vLocalScale.y * parentScale.y;
			m_vWorldScale.z = m_vLocalScale.z * parentScale.z;
		}
	}
	else
	{
		m_tWorldMatrix = m_tLocalMatrix;
		m_vWorldScale = m_vLocalScale;
	}

	// 월드 위치 갱신
	m_vWorldPosition = Vector3(
		m_tWorldMatrix.m[3][0],
		m_tWorldMatrix.m[3][1],
		m_tWorldMatrix.m[3][2]
	);

	// 방향 벡터 갱신 (부모 회전 고려 안함, 순수 로컬 회전 기준)
	m_vForward = Vector3::TransformNormal(Vector3::Forward, rotation);
	m_vRight = Vector3::TransformNormal(Vector3::Right, rotation);
	m_vUp = Vector3::TransformNormal(Vector3::Up, rotation);

	// 자식들 재귀적으로 업데이트
	for (CTransform*& child : m_vecChildren)
	{
		child->UpdateTransform();
	}
}

void CTransform::SetWorldScale(const Vector3 _vWorldScale)
{
	if (HasParent())
	{
		Vector3 parentScale = m_pParent->GetWorldScale();

		// 부모 스케일을 고려하여 로컬 스케일 계산
		Vector3 scale = _vWorldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(_vWorldScale);
	}
}

void CTransform::SetWorldRotation(const Vector3 _vWorldRotation)
{
	if (HasParent())
	{
		// 부모의 월드 행렬을 역행렬로 변환하여, 월드 좌표계에서 부모 로컬 좌표계로의 변환 행렬을 얻습니다.
		Matrix worldToParentLocalMatrix = m_pParent->GetWorldMatrix().Invert();

		// 월드 회전 값을 부모의 로컬 좌표계로 변환합니다.
		Vector3 rotation;
		rotation.Transform(_vWorldRotation, worldToParentLocalMatrix);

		// 계산된 회전 값을 현재 트랜스폼의 로컬 위치로 설정합니다.
		SetLocalPosition(rotation);
	}
	else
	{
		SetLocalRotation(_vWorldRotation);
	}
}

void CTransform::SetWorldPosition(const Vector3 _vWorldPosition)
{
	if (HasParent())
	{
		// 부모의 월드 행렬을 역행렬로 변환하여, 월드 좌표계에서 부모 로컬 좌표계로의 변환 행렬을 얻습니다.
		Matrix worldToParentLocalMatrix = m_pParent->GetWorldMatrix().Invert();

		// 월드 위치 값을 부모의 로컬 좌표계로 변환합니다.
		Vector3 position;
		position.Transform(_vWorldPosition, worldToParentLocalMatrix);

		// 계산된 위치 값을 현재 트랜스폼의 로컬 위치로 설정합니다.
		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(_vWorldPosition);
	}
}

/*void CTransform::Update()
{
	//UpdateWorldPosition();
	//UpdateWorldScale();

	//// 현재 변환 행렬 (스케일, 회전, 이동)
	//Vector3 worldScale = GetWorldScale();
	//Matrix scale = Matrix::CreateScale(worldScale.x, worldScale.y, worldScale.z);
	//Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vRotation.x));
	//rotation *= Matrix::CreateRotationY(math::Radian(m_vRotation.y));
	//rotation *= Matrix::CreateRotationZ(math::Radian(m_vRotation.z));
	//Matrix translation = Matrix::CreateTranslation(m_vWorldPosition);

	//Matrix localMatrix = scale * rotation * translation;

	//if (m_pParent != nullptr)
	//{
	//	// 부모의 변환 행렬에서 스케일을 제거
	//	Matrix parentMatrix = m_pParent->GetWorldMatrix();
	//	Vector3 parentScale = m_pParent->GetWorldScale();

	//	// 부모 행렬에서 X, Y, Z 스케일 제거
	//	parentMatrix._11 /= parentScale.x != 0.0f ? parentScale.x : 1.0f;
	//	parentMatrix._22 /= parentScale.y != 0.0f ? parentScale.y : 1.0f;
	//	parentMatrix._33 /= parentScale.z != 0.0f ? parentScale.z : 1.0f;

	//	m_tWorldMatrix = localMatrix * parentMatrix;
	//}
	//else
	//{
	//	m_tWorldMatrix = localMatrix;
	//}

	//m_vForward = Vector3::TransformNormal(Vector3::Forward, rotation);
	//m_vRight = Vector3::TransformNormal(Vector3::Right, rotation);
	//m_vUp = Vector3::TransformNormal(Vector3::Up, rotation);
}*/

//// 스케일, 회전, 이동 행렬을 계산
//Matrix scale = Matrix::CreateScale(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);
//Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vLocalRotation.x));
//rotation *= Matrix::CreateRotationY(math::Radian(m_vLocalRotation.y));
//rotation *= Matrix::CreateRotationZ(math::Radian(m_vLocalRotation.z));
//Matrix translation = Matrix::CreateTranslation(m_vLocalPosition);

//// 로컬 변환 행렬 설정
//m_tLocalMatrix = scale * rotation * translation;

//if (HasParent() && !m_bIgnoreParentScale)
//{
//	// 부모가 있으면 부모의 월드 행렬을 곱해서 월드 변환 행렬을 구함
//	m_tWorldMatrix = m_tLocalMatrix * m_pParent->GetWorldMatrix();

//	// 부모의 월드 스케일을 적용하여 월드 스케일을 계산
//	Vector3 parentScale = m_pParent->GetWorldScale();
//	m_vWorldScale.x = m_vLocalScale.x * parentScale.x;
//	m_vWorldScale.y = m_vLocalScale.y * parentScale.y;
//	m_vWorldScale.z = m_vLocalScale.z * parentScale.z;
//}
//else
//{
//	// 부모가 없으면 로컬 스케일이 곧 월드 스케일
//	m_tWorldMatrix = m_tLocalMatrix;
//	m_vWorldScale = m_vLocalScale;
//}

//// 월드 좌표 업데이트
//m_vWorldPosition = Vector3(m_tWorldMatrix.m[3][0], m_tWorldMatrix.m[3][1], m_tWorldMatrix.m[3][2]);

//// 방향 벡터 업데이트
//m_vForward = Vector3::TransformNormal(Vector3::Forward, rotation);
//m_vRight = Vector3::TransformNormal(Vector3::Right, rotation);
//m_vUp = Vector3::TransformNormal(Vector3::Up, rotation);

//// 자식들도 업데이트
//for (CTransform*& child : m_vecChildren)
//{
//	child->UpdateTransform();
//}