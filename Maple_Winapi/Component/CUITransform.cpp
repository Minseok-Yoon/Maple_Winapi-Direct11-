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

	// 자식 업데이트
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

	// 행렬 값 확인
	wchar_t buf[256];
	swprintf_s(buf, 256, L"World Matrix: [%.2f, %.2f, %.2f, %.2f]\n",
		m_WorldMatrix.m[3][0], m_WorldMatrix.m[3][1], m_WorldMatrix.m[3][2], m_WorldMatrix.m[3][3]);
	OutputDebugStringW(buf);

	CConstantBuffer* cb = renderer::constantBuffers[CBSLOT_TRANSFORM];
	if (cb) {
		cb->SetData(&data);
		cb->Bind(SHADER_STAGE::SS_ALL);
		OutputDebugStringA("Constant Buffer 바인딩 성공\n");
	}
	else {
		OutputDebugStringA("ERROR: Constant Buffer가 null입니다\n");
	}
}

void CUITransform::UpdateTransform()
{
	//// 피벗 오프셋을 올바르게 계산
	//Vector2 pivotOffset = Vector2(-m_Pivot.x * m_Size.x, -m_Pivot.y * m_Size.y);

	//Matrix scale = Matrix::CreateScale(m_Scale.x * m_Size.x, m_Scale.y * m_Size.y, 1.0f);
	//Matrix rotation = Matrix::CreateRotationZ(math::Radian(m_Rotation));
	//Matrix pivotTrans = Matrix::CreateTranslation(pivotOffset.x, pivotOffset.y, 0.1f);
	//Matrix translation = Matrix::CreateTranslation(m_Position.x, m_Position.y, 0.0f);

	//// 순서: scale -> rotation -> pivot -> translation
	//m_LocalMatrix = scale * rotation * pivotTrans * translation;

	//if (m_Parent)
	//	m_WorldMatrix = m_LocalMatrix * m_Parent->GetWorldMatrix();
	//else
	//	m_WorldMatrix = m_LocalMatrix;
	// 스크린 좌표를 중심 기준 좌표로 변환
	extern CCore core;
	float screenWidth = static_cast<float>(core.GetWidth());
	float screenHeight = static_cast<float>(core.GetHeight());

	// 스크린 좌표 (0~width, 0~height)를 중심 기준 좌표로 변환
	Vector2 centerBasedPos;
	centerBasedPos.x = m_Position.x - (screenWidth * 0.5f);
	centerBasedPos.y = (screenHeight * 0.5f) - m_Position.y; // Y축 뒤집기

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