#include "CTransform.h"
#include "../Graphics/CGraphics.h"
#include "../Graphics/CConstantBuffer.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"

CTransform::CTransform() :
	CComponent(COMPONENT_TYPE::CT_Transform),
	m_tWorldMatrix(Matrix::Identity),
	m_vScale(Vector3::One),
	m_vRotation(Vector3::Zero),
	m_vPosition(Vector3::Zero)
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
}

void CTransform::LateUpdate()
{
	// 월드 행렬 계산
	Matrix scale = Matrix::CreateScale(m_vScale.x, m_vScale.y, m_vScale.z);
	Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vRotation.x));
	rotation *= Matrix::CreateRotationY(math::Radian(m_vRotation.y));
	rotation *= Matrix::CreateRotationZ(math::Radian(m_vRotation.z));
	Matrix translation = Matrix::CreateTranslation(m_vPosition);

	// 변환 행렬 생성
	m_tWorldMatrix = scale * rotation * translation;

	// 방향 벡터 갱신
	m_vForward = Vector3::TransformNormal(Vector3::Forward, rotation);
	m_vRight = Vector3::TransformNormal(Vector3::Right, rotation);
	m_vUp = Vector3::TransformNormal(Vector3::Up, rotation);
}

void CTransform::Render()
{
}

void CTransform::Bind() const 
{
	TransformCB cbData = {};
	cbData.world = GetWorldMatrix();
	cbData.view = CCamera::GetGpuViewMatrix();
	cbData.projection = CCamera::GetGpuProjectionMatrix();

	CConstantBuffer* cb = renderer::constantBuffers[CBSLOT_TRANSFORM];
	cb->SetData(&cbData);
	cb->Bind(SHADER_STAGE::SS_ALL);
}

void CTransform::LookAt(const Vector3& targetPosition)
{
	// 타겟 방향 벡터 계산
	Vector3 direction = targetPosition - m_vPosition;  // 타겟 위치에서 현재 위치를 뺀 벡터
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
	m_tWorldMatrix = Matrix::CreateScale(m_vScale.x, m_vScale.y, m_vScale.z) * rotation * Matrix::CreateTranslation(m_vPosition);
}