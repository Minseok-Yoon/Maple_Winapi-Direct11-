#include "../pch.h"
#include "CItem.h"
#include "../Object/CGameObject.h"
#include "../Component/CCollider.h"
#include "../Module/SelectGDI.h"
#include "../Component/CTransform.h"
#include "../Component/CRenderer.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::Init()
{
}

void CItem::Update()
{
}

void CItem::LateUpdate()
{
}

void CItem::Render(const Matrix& view, const Matrix& projection)
{
    //CGameObject* gameObj = m_pItem->GetOwner();
    //CTransform* tr = gameObj->GetComponent<CTransform>();
    //Vector2 vPos = tr->GetPosition();

    //if (mainCamera)
    //    vPos = mainCamera->CaluatePosition(vPos);

    //Vector2 vOffset = GetOffsetPos();


    //// m_vPos�� ���� ��ǥ�� ����
    //Vector2 vPos = GetPos();
    //vPos = CCamera::GetInst()->GetRenderPos(vPos);

    //// �������� ������ ������ ���⿡ �߰��� �� �ֽ��ϴ�.
    //Ellipse(_dc, vPos.x - 10, vPos.y - 10, vPos.x + 10, vPos.y + 10);

    //// Collider�� ��ġ�� ������
    //if (!GetCollider().empty())
    //{
    //    auto itemCollider = GetCollider().back();
    //    Vector2 colPos = itemCollider->GetPos();
    //    Vector2 colRenderPos = CCamera::GetInst()->GetRenderPos(colPos);
    //}
}