#include "../pch.h"
#include "CItem.h"
#include "../Object/CGameObject.h"
#include "../Component/CCollider.h"
#include "../Module/SelectGDI.h"
#include "../Component/CTransform.h"
#include "../Component/CRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CSpriteRenderer.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}

void CItem::Init()
{
    CGameObject::Init();

    if (!m_pTransform) {
        m_pTransform = this->AddComponent<CTransform>();
        if (!m_pTransform) {
            OutputDebugStringA("ERROR: Transform creation failed in CMonster::Init()\n");
        }
    }

    // Collider ������Ʈ Ȯ�� �� �߰�
    if (!m_pItemCollider) {
        m_pItemCollider = this->AddComponent<CCollider>();
        if (m_pItemCollider) {
            m_pItemCollider->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
            m_pItemCollider->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));
            OutputDebugStringA("INFO: Collider successfully added to Item\n");
        }
        else {
            OutputDebugStringA("ERROR: Collider creation failed in CEtcItem::Init()\n");
        }
    }
}

void CItem::Update()
{
    CGameObject::Update();
}

void CItem::LateUpdate()
{
    CGameObject::LateUpdate();
}

void CItem::Render(const Matrix& view, const Matrix& projection)
{
    CGameObject::Render(view, projection);
}

void CItem::SetDropItem(wstring _MapName)
{
    SetName(_MapName);

    CTransform* MonTr = GetComponent<CTransform>();
    MonTr->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

    m_pItemTexture = CResourceManager::Find<CTexture>(_MapName);

    // ��������Ʈ ������ ����
    CSpriteRenderer* sr = AddComponent<CSpriteRenderer>();
    sr->SetTexture(m_pItemTexture);

    if (m_pItemTexture != nullptr)
    {
        // �ؽ�ó�� ���� ����� �״�� ���
        CTexture::TextureSize textureSize = m_pItemTexture->GetTextureSize();
        MonTr->SetLocalScale(Vector3(textureSize.width, textureSize.height, 0.0f));
    }
}

void CItem::OnPickUp()
{
    // ���� ��� �߰�
}

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