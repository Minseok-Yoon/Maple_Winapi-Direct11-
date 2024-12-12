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
	Matrix scale = Matrix::CreateScale(m_vScale.x, m_vScale.y, m_vScale.z);
	Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vRotation.x));
	rotation *= Matrix::CreateRotationY(math::Radian(m_vRotation.y));
	rotation *= Matrix::CreateRotationZ(math::Radian(m_vRotation.z));
	Matrix translation = Matrix::CreateTranslation(m_vPosition);

	m_tWorldMatrix = scale * rotation * translation;

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
