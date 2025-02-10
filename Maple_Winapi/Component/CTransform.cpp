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
	// ���� ��� ���
	Matrix scale = Matrix::CreateScale(m_vScale.x, m_vScale.y, m_vScale.z);
	Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vRotation.x));
	rotation *= Matrix::CreateRotationY(math::Radian(m_vRotation.y));
	rotation *= Matrix::CreateRotationZ(math::Radian(m_vRotation.z));
	Matrix translation = Matrix::CreateTranslation(m_vPosition);

	// ��ȯ ��� ����
	m_tWorldMatrix = scale * rotation * translation;

	// ���� ���� ����
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
	// Ÿ�� ���� ���� ���
	Vector3 direction = targetPosition - m_vPosition;  // Ÿ�� ��ġ���� ���� ��ġ�� �� ����
	direction.Normalize();  // ���� ���͸� ����ȭ

	// Up ���͸� �������� ȸ���� �� �ֵ��� Right, Up, Forward ���͸� ���
	Vector3 up = Vector3::Up;
	Vector3 right = up.Cross(direction);  // Up ���Ϳ� ���� ������ ����
	right.Normalize();  // ����ȭ

	Vector3 newUp = direction.Cross(right);  // ���� ���Ϳ� Right ������ ����
	newUp.Normalize();  // ����ȭ

	// ���ο� ȸ�� ����� ����
	m_vForward = direction;
	m_vRight = right;
	m_vUp = up;

	// ȸ�� ��� ����
	Matrix rotation = Matrix::Identity;
	rotation.m[0][0] = m_vRight.x;  rotation.m[0][1] = m_vRight.y;  rotation.m[0][2] = m_vRight.z;
	rotation.m[1][0] = m_vUp.x;     rotation.m[1][1] = m_vUp.y;     rotation.m[1][2] = m_vUp.z;
	rotation.m[2][0] = m_vForward.x; rotation.m[2][1] = m_vForward.y; rotation.m[2][2] = m_vForward.z;

	// ���� ȸ�� ����� ���� ȸ�� ��Ŀ� �ݿ�
	m_tWorldMatrix = Matrix::CreateScale(m_vScale.x, m_vScale.y, m_vScale.z) * rotation * Matrix::CreateTranslation(m_vPosition);
}