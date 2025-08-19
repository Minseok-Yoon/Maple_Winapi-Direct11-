#pragma once
#include "CBaseRenderer.h"

class CUIRenderer : public CBaseRenderer
{
	CUIRenderer();
    virtual ~CUIRenderer();

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render(const Matrix& view, const Matrix& projection) override;

private:
    CTexture* m_pTexture;
};