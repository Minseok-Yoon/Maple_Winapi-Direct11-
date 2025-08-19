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

	// 1. ����׷��� �������� ��Ʈ������ ���� �����
	UINT width = core.GetWidth();
	UINT height = core.GetHeight();

	// ���� ��� (0,0) ����, ���� �ϴ� (width, height)
	viewMatrix = Matrix::Identity;
	projMatrix = Matrix::CreateOrthographicOffCenterLH(
		0.0f, (float)width,     // left �� right
		0.0f, (float)height,    // top �� bottom �� �� ������ �߿�
		0.0f, 1.0f				// NearZ, FarZ
	);

	// Transform ��ġ�� �ȼ� ������� �ֱ� (NDC ��ȯ ����)
	CTransform* transform = GetOwner()->GetComponent<CTransform>();
	if (transform)
	{
		Vector3 screenPos = transform->GetLocalPosition(); // ���� ��ǥ�� �ȼ� �����̾�� ��
		Matrix worldMatrix = Matrix::CreateScale(transform->GetLocalScale()) *
			Matrix::CreateTranslation(screenPos);

		transform->SetWorldMatrix(worldMatrix); // ���ε����� ����� �� �ֵ���
	}

	CBaseRenderer::Render(viewMatrix, projMatrix);

	if (m_pTexture)
		m_pTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Sprite);

	CBaseRenderer::Draw();
}
