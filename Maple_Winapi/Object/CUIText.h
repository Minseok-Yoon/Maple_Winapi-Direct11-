#pragma once
#include "CUI.h"

class CUIText : public CUI
{
public:
    CUIText();
    virtual ~CUIText();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender() override;
    virtual void OnClear() override;

    void SetText(const std::wstring& str);
    void SetFullText(const std::wstring& str);

private:
    // 2025-07-17 MentBox에서 UIText로 아동
    wstring m_strFullText;  // 전체 텍스트
    wstring m_strCurrentText; // 타이핑 중인 텍스트
    float m_fTypingSpeed = 0.05f; // 한 글자당 출력 시간
    float m_fAccTime = 0.0f;
    size_t m_charIndex = 0;
};