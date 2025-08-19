#include "CUITransform.h"
#include "../Graphics/CGraphics.h"
#include "../Graphics/CConstantBuffer.h"
#include "../Component/CRenderer.h"
#include "../Core/CCore.h"

CUITransform::CUITransform() :
	CComponent(COMPONENT_TYPE::CT_UITransform)
{
}

CUITransform::~CUITransform()
{
}

void CUITransform::Init()
{
}

void CUITransform::Update()
{
	UpdateTransform();

	// �ڽ� ������Ʈ
	for (auto* child : m_Children)
	{
		child->Update();
	}
}

void CUITransform::LateUpdate()
{
}

void CUITransform::Render(const Matrix& view, const Matrix& projection)
{
}

void CUITransform::UIBind(const Matrix& view, const Matrix& projection) const
{
	OutputDebugStringA("=== CUITransform::UIBind called ===\n");

	TransformCB data = {};
	data.world = m_WorldMatrix.Transpose();
	data.view = Matrix::Identity.Transpose();
	data.projection = projection.Transpose();

	// ��� �� Ȯ��
	wchar_t buf[256];
	swprintf_s(buf, 256, L"World Matrix: [%.2f, %.2f, %.2f, %.2f]\n",
		m_WorldMatrix.m[3][0], m_WorldMatrix.m[3][1], m_WorldMatrix.m[3][2], m_WorldMatrix.m[3][3]);
	OutputDebugStringW(buf);

	CConstantBuffer* cb = renderer::constantBuffers[CBSLOT_TRANSFORM];
	if (cb) {
		cb->SetData(&data);
		cb->Bind(SHADER_STAGE::SS_ALL);
		OutputDebugStringA("Constant Buffer ���ε� ����\n");
	}
	else {
		OutputDebugStringA("ERROR: Constant Buffer�� null�Դϴ�\n");
	}
}

void CUITransform::UpdateTransform()
{
	//// �ǹ� �������� �ùٸ��� ���
	//Vector2 pivotOffset = Vector2(-m_Pivot.x * m_Size.x, -m_Pivot.y * m_Size.y);

	//Matrix scale = Matrix::CreateScale(m_Scale.x * m_Size.x, m_Scale.y * m_Size.y, 1.0f);
	//Matrix rotation = Matrix::CreateRotationZ(math::Radian(m_Rotation));
	//Matrix pivotTrans = Matrix::CreateTranslation(pivotOffset.x, pivotOffset.y, 0.1f);
	//Matrix translation = Matrix::CreateTranslation(m_Position.x, m_Position.y, 0.0f);

	//// ����: scale -> rotation -> pivot -> translation
	//m_LocalMatrix = scale * rotation * pivotTrans * translation;

	//if (m_Parent)
	//	m_WorldMatrix = m_LocalMatrix * m_Parent->GetWorldMatrix();
	//else
	//	m_WorldMatrix = m_LocalMatrix;
	// ��ũ�� ��ǥ�� �߽� ���� ��ǥ�� ��ȯ
	extern CCore core;
	float screenWidth = static_cast<float>(core.GetWidth());
	float screenHeight = static_cast<float>(core.GetHeight());

	// ��ũ�� ��ǥ (0~width, 0~height)�� �߽� ���� ��ǥ�� ��ȯ
	Vector2 centerBasedPos;
	centerBasedPos.x = m_Position.x - (screenWidth * 0.5f);
	centerBasedPos.y = (screenHeight * 0.5f) - m_Position.y; // Y�� ������

	Vector2 pivotOffset = Vector2(-m_Pivot.x * m_Size.x, -m_Pivot.y * m_Size.y);
	Matrix scale = Matrix::CreateScale(m_Scale.x * m_Size.x, m_Scale.y * m_Size.y, 1.f);
	Matrix rotation = Matrix::CreateRotationZ(math::Radian(m_Rotation));
	Matrix pivotTrans = Matrix::CreateTranslation(pivotOffset.x, pivotOffset.y, 0.f);
	Matrix translation = Matrix::CreateTranslation(centerBasedPos.x, centerBasedPos.y, -0.1f);

	m_LocalMatrix = scale * rotation * pivotTrans * translation;

	if (m_Parent)
		m_WorldMatrix = m_LocalMatrix * m_Parent->GetWorldMatrix();
	else
		m_WorldMatrix = m_LocalMatrix;
}