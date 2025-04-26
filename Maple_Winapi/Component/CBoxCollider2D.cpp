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

    //// 카메라가 있다면 위치를 변환
    //if (renderer::mainCamera)
    //    vPos = renderer::mainCamera->CaluatePosition2D(vPos);

    //// 충돌 박스의 위치와 크기 설정
    //Vector2 vBoxSize = Vector2(54.0f, 65.0f);  // 충돌 박스의 크기

    //// SpriteRenderer 생성 (또는 기존 SpriteRenderer 사용)
    //CSpriteRenderer* pSpriteRenderer = new CSpriteRenderer();

    //// 텍스처 없이 사각형만 그리기
    //CMaterial* pMaterial = new CMaterial();  // 텍스처 없는 기본 머티리얼을 설정
    //pSpriteRenderer->SetMaterial(pMaterial);

    //// 텍스처 없이 사각형 그리기 위한 메쉬를 설정 (이미 RectMesh가 있다고 가정)
    //CMesh* pRectMesh = CResourceManager::Find<CMesh>(L"RectMesh");  // RectMesh는 사각형 메쉬
    //pSpriteRenderer->SetMesh(pRectMesh);

    //// 사각형의 위치를 트랜스폼을 통해 설정
    //CTransform* pTransform = pSpriteRenderer->GetOwner()->GetComponent<CTransform>();
    //if (pTransform)
    //{
    //    pTransform->SetPosition2D(vPos + vOffset);  // 충돌 박스의 위치 설정
    //    pTransform->SetScale2D(vBoxSize);  // 크기 설정
    //}

    //// 충돌 박스 그리기
    //pSpriteRenderer->Render();
}