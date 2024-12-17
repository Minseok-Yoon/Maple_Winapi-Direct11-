#pragma once
#include "CGameObject.h"
#include "../Resource/CTexture.h"

class CColliderPixel;

class CBackGround : public CGameObject
{
public:
    CBackGround();
    virtual ~CBackGround();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;

    void SetBackGroundTexture(CTexture* _pBackGroundTexture) { m_pBackGroundTexture = _pBackGroundTexture; }
    CTexture* GetBackGroundTexture() const { return m_pBackGroundTexture; }

private:
    CTexture* m_pBackGroundTexture;
};