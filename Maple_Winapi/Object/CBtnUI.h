#pragma once
#include "CUI.h"
#include "../Resource/CTexture.h"

class CBtnUI : public CUI
{
public:
    CBtnUI();
    ~CBtnUI();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender(HDC _Hdc) override;
    virtual void OnClear() override;

    void ButtonClick();

    void SetBtnTexture(CTexture* _pTexture) { m_pTexture = _pTexture; }
    void SetBtnPosition(Vector2 _vPos) { m_vPos = _vPos; }
    Vector2 GetBtnPosition() const { return m_vPos; }

private:
    CTexture* m_pTexture;
    Vector2     m_vPos;
    tUIEvent    m_tOnClick;
};