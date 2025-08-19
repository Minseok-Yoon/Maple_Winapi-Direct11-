#include "CUIText.h"
#include "../Manager/CFontManager.h"
#include "../Manager/CTimeManager.h"

CUIText::CUIText() :
	CUI(UI_TYPE::UT_Text)
{
}

CUIText::~CUIText()
{
}

void CUIText::OnInit()
{
    CUI::OnInit();
}

void CUIText::OnActive()
{
}

void CUIText::OnInActive()
{
}

void CUIText::OnUpdate()
{
    CUI::OnUpdate();

    Vector3 setPos = GetTransform()->GetLocalPosition();
    GetTransform()->SetLocalPosition(Vector3(setPos.x, setPos.y, 0.0f));

    if (m_charIndex < m_strFullText.length())
    {
        m_fAccTime += CTimeManager::GetfDeltaTime();
        if (m_fAccTime >= m_fTypingSpeed)
        {
            m_strCurrentText += m_strFullText[m_charIndex++];
            m_fAccTime = 0.0f;
        }
    }
}

void CUIText::OnLateUpdate()
{
    CUI::OnLateUpdate();
}

void CUIText::OnRender()
{
    CUI::OnRender();

    if (!IsEnable()) return;

    ID3D11DeviceContext* context = GetDeviceContext();
    if (!context || m_strCurrentText.empty()) return;

    Vector3 worldPos = GetTransform()->GetLocalPosition();
    float OffsetPos = 200.0;
    Vector2 screenPos(worldPos.x, worldPos.y);

    CFontManager::GetInst()->DrawText(
        context,
        m_strCurrentText,
        18.0f,
        screenPos.x,
        screenPos.y,
        0xFFFF0000,
        L"Arial"
    );
}

void CUIText::OnClear()
{
}

void CUIText::SetText(const std::wstring& str)
{
    m_strFullText = str;
    m_strCurrentText = str;
    m_charIndex = str.length();
}

void CUIText::SetFullText(const std::wstring& str)
{
    m_strFullText = str;
    m_strCurrentText.clear();
    m_charIndex = 0;
    m_fAccTime = 0.0f;
}
