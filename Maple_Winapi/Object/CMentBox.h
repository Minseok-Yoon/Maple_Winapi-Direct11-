#pragma once
#include "CUI.h"
#include "../Resource/CTexture.h"

class CQuest;
class CMentBox : public CUI
{
public:
    CMentBox();
    virtual ~CMentBox();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender() override;
    virtual void OnClear() override;

    // 2025-06-23 MentBox�� �ؽ�Ʈ �߰�.
    void SetText(const wstring& _strText);

    // 2025-06-26
    void SetQuest(CQuest* _pQuest) { m_pQuest = _pQuest; }
    void OnAcceptQuest();

    // 2025-07-11
    //void SetText(const std::wstring& str) { m_strMent = str; }
    void SetTextOffset(const Vector2& offset) { m_vTextOffset = offset; }
    void SetFontSize(float size) { m_fFontSize = size; }
    void SetFontColor(UINT32 color) { m_uFontColor = color; }

// 2025-07-11
protected:
    std::wstring m_strMent;
    Vector2 m_vTextOffset;  // �ؽ�Ʈ�� �ڽ� �ȿ��� ��� ��ġ���� ��� ������
    float m_fFontSize;
    UINT32 m_uFontColor;

private:
    // 2025-06-23
    wstring m_strFullText;  // ��ü �ؽ�Ʈ
    wstring m_strCurrentText; // Ÿ���� ���� �ؽ�Ʈ
    float m_fTypingSpeed = 0.05f; // �� ���ڴ� ��� �ð�
    float m_fAccTime = 0.0f;
    size_t m_charIndex = 0; 

    // 2025-06-25 ���콺 �巡�� ���
    bool m_bDragging = false;
    Vector2 m_vDragOffset = Vector2(0.0f, 0.0f);
    class CCollider* m_pMentBoxCol = nullptr;

    // 2025-06-26
    CQuest* m_pQuest = nullptr;

    class CCamera*  m_pCamera = nullptr;
    class CUIText*  m_pTextUI = nullptr;
    class CBtnUI*   m_pBtnUI = nullptr;
};