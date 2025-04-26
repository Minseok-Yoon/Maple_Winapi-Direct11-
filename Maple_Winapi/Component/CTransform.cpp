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
	// Ÿ�� ���� ���� ���
	Vector3 direction = targetPosition - m_vWorldPosition;  // Ÿ�� ��ġ���� ���� ��ġ�� �� ����
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
	m_tWorldMatrix = Matrix::CreateScale(m_vWorldScale.x, m_vWorldScale.y, m_vWorldScale.z) * rotation * Matrix::CreateTranslation(m_vWorldPosition);
}

void CTransform::UpdateTransform()
{
	// ������, ȸ��, �̵� ��� ����
	Matrix scale = Matrix::CreateScale(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);
	Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vLocalRotation.x));
	rotation *= Matrix::CreateRotationY(math::Radian(m_vLocalRotation.y));
	rotation *= Matrix::CreateRotationZ(math::Radian(m_vLocalRotation.z));
	Matrix translation = Matrix::CreateTranslation(m_vLocalPosition);

	// ���� ��� ����
	m_tLocalMatrix = scale * rotation * translation;

	if (HasParent())
	{
		Matrix parentWorld = m_pParent->GetWorldMatrix();

		if (m_bIgnoreParentScale)
		{
			// �θ��� ȸ���� ���� ���
			Matrix parentRotation = Matrix::CreateRotationX(math::Radian(m_pParent->GetLocalRotation().x));
			parentRotation *= Matrix::CreateRotationY(math::Radian(m_pParent->GetLocalRotation().y));
			parentRotation *= Matrix::CreateRotationZ(math::Radian(m_pParent->GetLocalRotation().z));

			// �θ� ��ġ ��������
			Matrix parentTranslation = Matrix::CreateTranslation(m_pParent->GetWorldPosition());

			// ������ ���ܵ� �θ� ��� (ȸ�� + ��ġ��)
			Matrix parentNoScale = parentRotation * parentTranslation;

			// ���� ���� ���
			m_tWorldMatrix = m_tLocalMatrix * parentNoScale;

			// ���� �������� ���� �״�� ����
			m_vWorldScale = m_vLocalScale;
		}
		else
		{
			// �Ϲ����� ���: ��ü �θ� ��� ���ϱ�
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

	// ���� ��ġ ����
	m_vWorldPosition = Vector3(
		m_tWorldMatrix.m[3][0],
		m_tWorldMatrix.m[3][1],
		m_tWorldMatrix.m[3][2]
	);

	// ���� ���� ���� (�θ� ȸ�� ��� ����, ���� ���� ȸ�� ����)
	m_vForward = Vector3::TransformNormal(Vector3::Forward, rotation);
	m_vRight = Vector3::TransformNormal(Vector3::Right, rotation);
	m_vUp = Vector3::TransformNormal(Vector3::Up, rotation);

	// �ڽĵ� ��������� ������Ʈ
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

		// �θ� �������� ����Ͽ� ���� ������ ���
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
		// �θ��� ���� ����� ����ķ� ��ȯ�Ͽ�, ���� ��ǥ�迡�� �θ� ���� ��ǥ����� ��ȯ ����� ����ϴ�.
		Matrix worldToParentLocalMatrix = m_pParent->GetWorldMatrix().Invert();

		// ���� ȸ�� ���� �θ��� ���� ��ǥ��� ��ȯ�մϴ�.
		Vector3 rotation;
		rotation.Transform(_vWorldRotation, worldToParentLocalMatrix);

		// ���� ȸ�� ���� ���� Ʈ�������� ���� ��ġ�� �����մϴ�.
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
		// �θ��� ���� ����� ����ķ� ��ȯ�Ͽ�, ���� ��ǥ�迡�� �θ� ���� ��ǥ����� ��ȯ ����� ����ϴ�.
		Matrix worldToParentLocalMatrix = m_pParent->GetWorldMatrix().Invert();

		// ���� ��ġ ���� �θ��� ���� ��ǥ��� ��ȯ�մϴ�.
		Vector3 position;
		position.Transform(_vWorldPosition, worldToParentLocalMatrix);

		// ���� ��ġ ���� ���� Ʈ�������� ���� ��ġ�� �����մϴ�.
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

	//// ���� ��ȯ ��� (������, ȸ��, �̵�)
	//Vector3 worldScale = GetWorldScale();
	//Matrix scale = Matrix::CreateScale(worldScale.x, worldScale.y, worldScale.z);
	//Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vRotation.x));
	//rotation *= Matrix::CreateRotationY(math::Radian(m_vRotation.y));
	//rotation *= Matrix::CreateRotationZ(math::Radian(m_vRotation.z));
	//Matrix translation = Matrix::CreateTranslation(m_vWorldPosition);

	//Matrix localMatrix = scale * rotation * translation;

	//if (m_pParent != nullptr)
	//{
	//	// �θ��� ��ȯ ��Ŀ��� �������� ����
	//	Matrix parentMatrix = m_pParent->GetWorldMatrix();
	//	Vector3 parentScale = m_pParent->GetWorldScale();

	//	// �θ� ��Ŀ��� X, Y, Z ������ ����
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

//// ������, ȸ��, �̵� ����� ���
//Matrix scale = Matrix::CreateScale(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);
//Matrix rotation = Matrix::CreateRotationX(math::Radian(m_vLocalRotation.x));
//rotation *= Matrix::CreateRotationY(math::Radian(m_vLocalRotation.y));
//rotation *= Matrix::CreateRotationZ(math::Radian(m_vLocalRotation.z));
//Matrix translation = Matrix::CreateTranslation(m_vLocalPosition);

//// ���� ��ȯ ��� ����
//m_tLocalMatrix = scale * rotation * translation;

//if (HasParent() && !m_bIgnoreParentScale)
//{
//	// �θ� ������ �θ��� ���� ����� ���ؼ� ���� ��ȯ ����� ����
//	m_tWorldMatrix = m_tLocalMatrix * m_pParent->GetWorldMatrix();

//	// �θ��� ���� �������� �����Ͽ� ���� �������� ���
//	Vector3 parentScale = m_pParent->GetWorldScale();
//	m_vWorldScale.x = m_vLocalScale.x * parentScale.x;
//	m_vWorldScale.y = m_vLocalScale.y * parentScale.y;
//	m_vWorldScale.z = m_vLocalScale.z * parentScale.z;
//}
//else
//{
//	// �θ� ������ ���� �������� �� ���� ������
//	m_tWorldMatrix = m_tLocalMatrix;
//	m_vWorldScale = m_vLocalScale;
//}

//// ���� ��ǥ ������Ʈ
//m_vWorldPosition = Vector3(m_tWorldMatrix.m[3][0], m_tWorldMatrix.m[3][1], m_tWorldMatrix.m[3][2]);

//// ���� ���� ������Ʈ
//m_vForward = Vector3::TransformNormal(Vector3::Forward, rotation);
//m_vRight = Vector3::TransformNormal(Vector3::Right, rotation);
//m_vUp = Vector3::TransformNormal(Vector3::Up, rotation);

//// �ڽĵ鵵 ������Ʈ
//for (CTransform*& child : m_vecChildren)
//{
//	child->UpdateTransform();
//}