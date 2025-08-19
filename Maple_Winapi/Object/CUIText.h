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
    // 2025-07-17 MentBox���� UIText�� �Ƶ�
    wstring m_strFullText;  // ��ü �ؽ�Ʈ
    wstring m_strCurrentText; // Ÿ���� ���� �ؽ�Ʈ
    float m_fTypingSpeed = 0.05f; // �� ���ڴ� ��� �ð�
    float m_fAccTime = 0.0f;
    size_t m_charIndex = 0;
};