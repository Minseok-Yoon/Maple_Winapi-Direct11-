#pragma once
#include "CGameObject.h"
#include "../Resource/CTexture.h"

class CPixelCollider;
class CBackGround : public CGameObject
{
public:
    // 2025-07-03
    struct SlopeInfo
    {
        bool isSlope;
        float angle;        // 경사 각도 (도 단위)
        Vector3 normal;     // 경사면 법선 벡터
        bool isUpSlope;     // 올라가는 경사인지 여부
    };

public:
    CBackGround();
    virtual ~CBackGround();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;

    bool CheckGround(Vector3 _vPlusCheckPos);

    TextureColor GetPixelColor(const Vector3& _vWorldPos);
    TextureColor GetColor(Vector3 _Pos, TextureColor _DefaultColor);

    void CreateMap(wstring _MapName);
    void CreateCollisionMap(wstring _MapCollisionName);

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

    // 2025-07-03
    SlopeInfo GetSlopeInfo(Vector3 _pos, float _direction = 0.0f)
    {
        SlopeInfo info;
        info.isSlope = false;
        info.angle = 0.0f;
        info.normal = Vector3(0.0f, 1.0f, 0.0f);
        info.isUpSlope = false;

        // 현재 위치와 좌우 픽셀들을 체크
        float leftHeight = GetGroundHeight(Vector3(_pos.x - 5.0f, _pos.y, 0));
        float centerHeight = GetGroundHeight(_pos);
        float rightHeight = GetGroundHeight(Vector3(_pos.x + 5.0f, _pos.y, 0));

        // 경사 계산
        float leftSlope = centerHeight - leftHeight;
        float rightSlope = rightHeight - centerHeight;

        // 이동 방향에 따른 경사 판정
        if (_direction > 0) // 오른쪽 이동
        {
            if (abs(rightSlope) > 1.0f) // 경사 임계값
            {
                info.isSlope = true;
                info.angle = atan2(rightSlope, 5.0f) * 180.0f / 3.14159f;
                info.isUpSlope = rightSlope > 0;
                info.normal = Vector3(-sin(info.angle * 3.14159f / 180.0f),
                    cos(info.angle * 3.14159f / 180.0f), 0);
            }
        }
        else if (_direction < 0) // 왼쪽 이동
        {
            if (abs(leftSlope) > 1.0f)
            {
                info.isSlope = true;
                info.angle = atan2(-leftSlope, 5.0f) * 180.0f / 3.14159f;
                info.isUpSlope = leftSlope < 0;
                info.normal = Vector3(sin(info.angle * 3.14159f / 180.0f),
                    cos(info.angle * 3.14159f / 180.0f), 0);
            }
        }

        return info;
    }

    float GetGroundHeight(Vector3 _pos);

private:
    wstring  m_strMapName;
    CTexture* m_pMapCollision;
    CTexture* m_pMap;


    CTexture*       m_pBackGroundTexture;
    CPixelCollider* m_pPixBackGround;

    // 2025-07-03
    Vector2         m_vMapOrigin;
    float           m_fPixelsPerUnit = 1.0f;
};