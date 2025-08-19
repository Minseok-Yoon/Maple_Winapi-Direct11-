#include "CSpriteRenderer.h"
#include "CTransform.h"
#include "../Resource/CTexture.h"
#include "../Component/CRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"
#include "CUITransform.h"
#include "../Core/CCore.h"

extern CCore core;

CSpriteRenderer::CSpriteRenderer() :
	CBaseRenderer(COMPONENT_TYPE::CT_SpriteRenderer)
{
}

CSpriteRenderer::~CSpriteRenderer()
{
}

void CSpriteRenderer::Init()
{
	CBaseRenderer::Init();

	CMesh* m_pMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	CMaterial* m_pMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");

	SetMesh(m_pMesh);
	SetMaterial(m_pMaterial);
}

void CSpriteRenderer::Update()
{
	CBaseRenderer::Update();
}

void CSpriteRenderer::LateUpdate()
{
	CBaseRenderer::LateUpdate();
}

void CSpriteRenderer::Render(const Matrix& view, const Matrix& projection)
{
	// ��Ȱ��ȭ ���°ų� �ؽ�ó�� ������ ������ �� ��
	if (!IsEnabled() || !GetOwner()->GetActive() || !m_pTexture) {
		return;
	}

	Matrix viewMatrix = view;
	Matrix projMatrix = projection;

	if (m_bScreenSpace)
	{
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
	}

	CBaseRenderer::Render(viewMatrix, projMatrix);

	if (m_pTexture)
		m_pTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Sprite);

	CBaseRenderer::Draw();
	//Matrix viewMatrix = view;
	//Matrix projMatrix = projection;

	//if (m_bScreenSpace)
	//{
	//	// 1. ����׷��� �������� ��Ʈ������ ���� �����
	//	UINT width = core.GetWidth();
	//	UINT height = core.GetHeight();

	//	// ���� ��� (0,0) ����, ���� �ϴ� (width, height)
	//	viewMatrix = Matrix::Identity;
	//	projMatrix = Matrix::CreateOrthographicOffCenterLH(
	//		0.0f, (float)width,     // left �� right
	//		0.0f, (float)height,     // top �� bottom �� �� ������ �߿�
	//		0.0f, 1.0f				// NearZ, FarZ
	//	);

	//	// Transform ��ġ�� �ȼ� ������� �ֱ� (NDC ��ȯ ����)
	//	CTransform* transform = GetOwner()->GetComponent<CTransform>();
	//	if (transform)
	//	{
	//		Vector3 screenPos = transform->GetLocalPosition(); // ���� ��ǥ�� �ȼ� �����̾�� ��
	//		Matrix worldMatrix = Matrix::CreateScale(transform->GetLocalScale()) *
	//			Matrix::CreateTranslation(screenPos);

	//		transform->SetWorldMatrix(worldMatrix); // ���ε����� ����� �� �ֵ���
	//	}
	//}

	//CBaseRenderer::Render(viewMatrix, projMatrix);

	//if (m_pTexture)
	//	m_pTexture->Bind(SHADER_STAGE::SS_PS, (UINT)TEXTURE_TYPE::TT_Sprite);

	//CBaseRenderer::Draw();
}

void CSpriteRenderer::SetSprite(wstring strName, bool bScreenSpace)
{
	//CSpriteRenderer* sr = GetComponent<CSpriteRenderer>();

	//CTexture* pTexture = CResourceManager::Find<CTexture>(strName);
	//SetTexture(pTexture);         // ������ SetTexture(CTexture*) ȣ��
	//SetScreenSpace(bScreenSpace);
}
