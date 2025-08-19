#include "../pch.h"
#include "CUI.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CCamera.h"
#include "../Module/SelectGDI.h"
#include "../Scene/CScene.h"
#include "../Core/CCore.h"

extern CCore core;

CUI::CUI(UI_TYPE _eUIType) :
	m_eUIType(_eUIType),
	m_bMouseOn(false),
	m_bEnable(false)
{
}

CUI::~CUI()
{
}

void CUI::Init()
{
	OnInit();
}

void CUI::Update()
{
	if (m_bEnable)
		OnUpdate();
}

void CUI::LateUpdate()
{
	if (m_bEnable)
		OnLateUpdate();
}

void CUI::Render()
{
	if (m_bEnable)
		OnRender();
}

void CUI::Active()
{
	m_bEnable = true;
	OnActive();
}

void CUI::InActive()
{
	m_bEnable = false;
	OnInActive();
}

void CUI::UIClear()
{
	InActive();
	OnClear();
}

void CUI::OnInit()
{
	CGameObject::Init();
}

void CUI::OnActive()
{
}

void CUI::OnInActive()
{
}

void CUI::OnUpdate()
{
}

void CUI::OnLateUpdate()
{
}

void CUI::OnRender()
{
}

void CUI::OnClear()
{
}

Vector2 CUI::GetScreenPosition()
{
    UINT screenWidth = CCore::GetInst()->GetWidth();
    UINT screenHeight = CCore::GetInst()->GetHeight();
    Vector2 pixelPos;

    if (m_bFullScreen)
    {
        // ��ü ȭ�� UI�� ��ġ �״�� �ȼ� ������ ���
        pixelPos = m_vLocalPosition;
    }
    else
    {
        // Transform�� ���� world position ���
        Vector3 worldPos = GetTransform()->GetWorldPosition();

        // NDC ��ǥ�� �ȼ� ��ǥ�� ��ȯ
        pixelPos.x = (worldPos.x + 1.0f) * 0.5f * screenWidth;
        pixelPos.y = (1.0f - worldPos.y) * 0.5f * screenHeight; // Y�� ����

        // �Ǵ� m_vPosition�� �̹� �ȼ� ��ǥ���
        // pixelPos = m_vPosition;
    }

    return pixelPos;
}

void CUI::SetUILocalPos(Vector2 _vPosition)
{
    m_vLocalPosition = _vPosition;

    float screenHeight = (float)core.GetHeight();
    Vector2 uiPos = Vector2(m_vLocalPosition.x, m_vLocalPosition.y);
    uiPos.y = screenHeight - uiPos.y;

    if (GetTransform())
        GetTransform()->SetLocalPosition(Vector3(uiPos.x, uiPos.y, 0.0f));
}

Vector2 CUI::GetUILocalPos()
{
    if (GetTransform())
        GetTransform()->GetLocalPosition();

    m_vLocalPosition.x = GetTransform()->GetLocalPosition().x;
    m_vLocalPosition.y = GetTransform()->GetLocalPosition().y;

    return Vector2(m_vLocalPosition.x, m_vLocalPosition.y);
}

void CUI::SetUILocalScale(Vector2 _vScale)
{
    m_vLocalScale = _vScale;

    if (GetTransform())
        GetTransform()->SetLocalScale(Vector3(m_vLocalScale.x, m_vLocalScale.y, 0.0f));
}

Vector2 CUI::GetUILocalScale()
{
    if(GetTransform())
        GetTransform()->GetLocalScale();

    m_vLocalScale.x = GetTransform()->GetLocalPosition().x;
    m_vLocalScale.y = GetTransform()->GetLocalPosition().y;

    return Vector2(m_vLocalScale.x, m_vLocalScale.y);
}

void CUI::SetParent(CUI* pParent)
{
    // ���� �θ𿡼� ����
        if (m_pParent)
            m_pParent->RemoveChild(this);

    // �� �θ� ����
    m_pParent = pParent;
    if (m_pParent)
    {
        m_pParent->AddChild(this);
        UpdateAbsolutePosition(); // �θ� ����� ��ġ ����
    }
}

void CUI::AddChild(CUI* pChild)
{
    if (pChild && find(m_vecChildren.begin(), m_vecChildren.end(), pChild) == m_vecChildren.end())
    {
        m_vecChildren.push_back(pChild);
        pChild->m_pParent = this;
        pChild->UpdateAbsolutePosition();
    }
}

void CUI::RemoveChild(CUI* pChild)
{
    auto it = find(m_vecChildren.begin(), m_vecChildren.end(), pChild);
    if (it != m_vecChildren.end())
    {
        (*it)->m_pParent = nullptr;
        m_vecChildren.erase(it);
    }
}

void CUI::SetRelativePos(const Vector2& relativePos)
{
    m_vRelativePos = relativePos;
    UpdateAbsolutePosition();
}

void CUI::UpdateAbsolutePosition()
{
    if (m_pParent)
    {
        Vector2 parentPos = m_pParent->GetUILocalPos();
        m_vFinalPosition = parentPos + m_vRelativePos;
    }
    else
    {
        m_vFinalPosition = m_vLocalPosition;
    }

    // Transform ������Ʈ
    if (GetTransform())
    {
        if (!m_bFullScreen)
        {
            GetTransform()->SetLocalPosition(Vector3(m_vFinalPosition.x, m_vFinalPosition.y, 0.0f));
        }
    }

    // �ڽĵ鵵 ������Ʈ
    updateChildrenPosition();
}

void CUI::updateChildrenPosition()
{
    for (CUI* child : m_vecChildren)
    {
        child->UpdateAbsolutePosition();
    }
}

// 2025-07-16
//Vector2 CUI::ScreenToNDC(const Vector2& screenPos)
//{
//    float screenWidth = (float)core.GetWidth();
//    float screenHeight = (float)core.GetHeight();
//
//    float ndcX = (screenPos.x / screenWidth) * 2.0f - 1.0f;
//    float ndcY = 1.0f - (screenPos.y / screenHeight) * 2.0f;
//
//    return Vector2(ndcX, ndcY);
//}
