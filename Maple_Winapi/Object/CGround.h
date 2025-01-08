#pragma once
#include "CGameObject.h"
#include "../Resource/CTexture.h"

class CPixelCollider;

class CBackGround : public CGameObject
{
public:
    CBackGround();
    virtual ~CBackGround();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;

    void SetBackGroundTexture(CTexture* _pBackGroundTexture);
    CTexture* GetBackGroundTexture() const { return m_pBackGroundTexture; }

    CPixelCollider* GetPixelCollider() const { return m_pPixBackGround; }

private:
    CTexture*       m_pBackGroundTexture;
    CPixelCollider* m_pPixBackGround;
};