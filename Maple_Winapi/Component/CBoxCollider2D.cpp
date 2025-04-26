#include "CBoxCollider2D.h"
#include "CTransform.h"
#include "../Object/CGameObject.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"
#include "../Module/SelectGDI.h"
#include "../Component/CSpriteRenderer.h"
#include "../Resource/CMaterial.h"
#include "../Manager/CResourceManager.h"

CBoxCollider2D::CBoxCollider2D() :
	m_iCol(0)
{
}

CBoxCollider2D::~CBoxCollider2D()
{
}

void CBoxCollider2D::Init()
{
}

void CBoxCollider2D::Update()
{
}

void CBoxCollider2D::LateUpdate()
{
}

void CBoxCollider2D::Render(const Matrix& view, const Matrix& projection)
{
    //CTransform* tr = GetOwner()->GetComponent<CTransform>();
    //Vector2 vPos = tr->GetPosition2D();
    //Vector2 vOffset = GetOffsetPos();

    //// ī�޶� �ִٸ� ��ġ�� ��ȯ
    //if (renderer::mainCamera)
    //    vPos = renderer::mainCamera->CaluatePosition2D(vPos);

    //// �浹 �ڽ��� ��ġ�� ũ�� ����
    //Vector2 vBoxSize = Vector2(54.0f, 65.0f);  // �浹 �ڽ��� ũ��

    //// SpriteRenderer ���� (�Ǵ� ���� SpriteRenderer ���)
    //CSpriteRenderer* pSpriteRenderer = new CSpriteRenderer();

    //// �ؽ�ó ���� �簢���� �׸���
    //CMaterial* pMaterial = new CMaterial();  // �ؽ�ó ���� �⺻ ��Ƽ������ ����
    //pSpriteRenderer->SetMaterial(pMaterial);

    //// �ؽ�ó ���� �簢�� �׸��� ���� �޽��� ���� (�̹� RectMesh�� �ִٰ� ����)
    //CMesh* pRectMesh = CResourceManager::Find<CMesh>(L"RectMesh");  // RectMesh�� �簢�� �޽�
    //pSpriteRenderer->SetMesh(pRectMesh);

    //// �簢���� ��ġ�� Ʈ�������� ���� ����
    //CTransform* pTransform = pSpriteRenderer->GetOwner()->GetComponent<CTransform>();
    //if (pTransform)
    //{
    //    pTransform->SetPosition2D(vPos + vOffset);  // �浹 �ڽ��� ��ġ ����
    //    pTransform->SetScale2D(vBoxSize);  // ũ�� ����
    //}

    //// �浹 �ڽ� �׸���
    //pSpriteRenderer->Render();
}