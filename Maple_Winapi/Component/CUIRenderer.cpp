#include "CUIRenderer.h"
#include "CTransform.h"
#include "../Manager/CResourceManager.h"
#include "../Core/CCore.h"

extern CCore core;

CUIRenderer::CUIRenderer() :
	CBaseRenderer(COMPONENT_TYPE::CT_UIRenderer)
{
}

CUIRenderer::~CUIRenderer()
{
}

void CUIRenderer::Init()
{
	CBaseRenderer::Init();
	SetCameraOrder(CAMERA_ORDER::CO_UI);

	CMesh* m_pMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	CMaterial* m_pMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");

	SetMesh(m_pMesh);
	SetMaterial(m_pMaterial);
}

void CUIRenderer::Update()
{
	CBaseRenderer::Init();
}

void CUIRenderer::LateUpdate()
{
	CBaseRenderer::Init();
}

void CUIRenderer::Render(const Matrix& view, const Matrix& projection)
{
	Matrix viewMatrix = view;
	Matrix projMatrix = projection;

	// 1. 오쏘그래픽 프로젝션 매트릭스를 새로 만든다
	UINT width = core.GetWidth();
	UINT height = core.GetHeight();

	// 좌측 상단 (0,0) 기준, 우측 하단 (width, height)
	viewMatrix = Matrix::Identity;
	projMatrix = Matrix::CreateOrthographicOffCenterLH(
		0.0f, (float)width,     // left → right
		0.0f, (float)height,    // top → bottom ← 이 순서가 중요
		0.0f, 1.0f				// NearZ, FarZ
	);

	// Transform 위치도 픽셀 기반으로 넣기 (NDC 변환 없이)
	CTransform* transform = GetOwner()->GetComponent<CTransform>();
	if (transform)
	{
		Vector3 screenPos = transform->GetLocalPosition(); // 현재 좌표는 픽셀 기준이어야 함
		Matrix worldMatrix = Matrix::CreateScale(transform->GetLocalScale()) *
			Matrix::CreateTranslation(screenPos);

		transform->SetWorldMatrix(worldMatrix); // 바인딩에서 사용할 수 있도록
	}

	CBaseRenderer::Render(viewMatrix, projMatrix);

	if (m_pTexture)
		m_pTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Sprite);

	CBaseRenderer::Draw();
}
