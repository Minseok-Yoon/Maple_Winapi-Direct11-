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
    virtual void Render(const Matrix& view, const Matrix& projection) override;

    bool CheckGround(Vector3 _vPlusCheckPos);

    TextureColor GetColor(Vector3 _Pos, TextureColor _DefaultColor);

    void CreateMap(wstring _MapName);
    void CreateCollisionMap(wstring _MapCollisionName);

    bool CheckCollision(int playerX, int playerY);
    void CreateTextureFromPixel();
    //void SetBackGroundTexture(CTexture* _pBackGroundTexture);
    CTexture* GetBackGroundTexture() const { return m_pBackGroundTexture; }

    CPixelCollider* GetPixelCollider() const 
    { 
        if (m_pPixBackGround == nullptr)
        {
            OutputDebugStringA("CBackGround::GetPixelCollider: m_pPixBackGround is nullptr.\n");
        }

        return m_pPixBackGround; 
    }

    TextureColor GetGroundColor(Vector3 _Pos);

private:
    wstring  m_strMapName;
    CTexture* m_pMapCollision;


    CTexture*       m_pBackGroundTexture;
    CPixelCollider* m_pPixBackGround;
};