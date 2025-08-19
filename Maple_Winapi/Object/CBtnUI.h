#pragma once
#include "CUI.h"
#include "../Resource/CTexture.h"

class CBtnUI : public CUI
{
public:
    CBtnUI();
    virtual ~CBtnUI();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender() override;
    virtual void OnClear() override;

    virtual void ButtonClick();

    void SetBtnTexture(CTexture* _pTexture) { m_pTexture = _pTexture; }
    void SetBtnPosition(Vector2 _vPos) { m_vPos = _vPos; }
    Vector2 GetBtnPosition() const { return m_vPos; }

    // 2025-07-18
    void SetButtonClickEvent(function<void()> Function)
    {
        ButtonClickEvent = Function;
    }
    void SetButtonClickEndEvent(function<void()> Function)
    {
        ButtonClickEndEvent = Function;
    }
    void SetButtonClickStayEvent(function<void()> Function)
    {
        ButtonStayEvent = Function;
    }

    function<void()> GetButtonClickEvent()
    {
        return ButtonClickEvent;
    }

    function<void()> GetButtonClickEndEvent()
    {
        return ButtonClickEndEvent;
    }

    function<void()> GetButtonStayEvent()
    {
        return ButtonStayEvent;
    }

    void ClickUpdate();

private:
    CTexture* m_pTexture;
    Vector2     m_vPos;
    tUIEvent    m_tOnClick;

    // 2025-07-18
    function<void()> ButtonClickEvent;
    function<void()> ButtonClickEndEvent;
    function<void()> ButtonStayEvent;
};