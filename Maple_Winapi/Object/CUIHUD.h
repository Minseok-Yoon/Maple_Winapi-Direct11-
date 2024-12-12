#pragma once
#include "CUI.h"
#include "../Resource/CTexture.h"

class CUIHUD : public CUI
{
public:
    CUIHUD();
    virtual ~CUIHUD();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender() override;
    virtual void OnClear() override;

private:
    CTexture* m_pTexture;
};