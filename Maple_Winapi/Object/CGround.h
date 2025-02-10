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

    bool CheckCollision(int playerX, int playerY);
    void CreateTextureFromPixel();
    void SetBackGroundTexture(CTexture* _pBackGroundTexture);
    CTexture* GetBackGroundTexture() const { return m_pBackGroundTexture; }

    CPixelCollider* GetPixelCollider() const 
    { 
        if (m_pPixBackGround == nullptr)
        {
            OutputDebugStringA("CBackGround::GetPixelCollider: m_pPixBackGround is nullptr.\n");
        }

        return m_pPixBackGround; 
    }

private:
    CTexture*       m_pBackGroundTexture;
    CPixelCollider* m_pPixBackGround;
};