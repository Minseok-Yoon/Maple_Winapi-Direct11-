#include "CBaseRenderer.h"
#include "CTransform.h"
#include "CUITransform.h"
#include "../Object/CGameObject.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"

CBaseRenderer::CBaseRenderer(COMPONENT_TYPE type) :
	CComponent(type),
	m_pMaterial(nullptr)
{
}

CBaseRenderer::~CBaseRenderer()
{
}

void CBaseRenderer::Init()
{
}

void CBaseRenderer::Update()
{
}

void CBaseRenderer::LateUpdate()
{
}

void CBaseRenderer::Render(const Matrix& view, const Matrix& projection)
{
	// ��Ȱ��ȭ ���¸� ������ ��ŵ
	if (!m_bEnabled || !GetOwner()->GetActive()) {
		return;
	}

	CTransform* transform = GetOwner()->GetComponent<CTransform>();
	if (transform)
		transform->Bind(view, projection);

	if (m_pMesh)
		m_pMesh->Bind();

	if (m_pMaterial)
		m_pMaterial->BindShader();
}

void CBaseRenderer::Draw()
{
	if (m_pMesh) {
		//OutputDebugStringA("=== Draw ȣ�� ===\n");

		//// �޽� ���� Ȯ��
		//wchar_t buf[128];
		//swprintf_s(buf, 128, L"Index Count: %d\n", m_pMesh->GetIndexCount());
		//OutputDebugStringW(buf);

		GetDevice()->DrawIndexed(m_pMesh->GetIndexCount(), 0, 0);
		//OutputDebugStringA("DrawIndexed �Ϸ�\n");
	}
	else {
		//OutputDebugStringA("ERROR: Mesh�� null�Դϴ�\n");
	}
}

void CBaseRenderer::SetCameraOrder(CAMERA_ORDER _eCamOrder)
{
	SetViewCameraSelect(static_cast<int>(_eCamOrder));
}

void CBaseRenderer::SetViewCameraSelect(int _iOrder)
{
	CScene* scene = CSceneManager::GetCurScene();

	CCamera* findCamera = scene->GetCamera(static_cast<int>(_iOrder));

	if (findCamera == nullptr)
	{
		OutputDebugStringA("ī�޶� �������� �ʽ��ϴ�.");
		return;
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->m_mapRenderers[GetOrder()].remove(dynamic_cast<CBaseRenderer*>(GetOwner()));
	}

	m_pCamera = findCamera;
	m_pCamera->m_mapRenderers[GetOrder()].push_back(dynamic_cast<CBaseRenderer*>(GetOwner()));
}
