#pragma once
#include "CBaseRenderer.h"
#include "CTransform.h"

enum class MASK_MODE
{
    MM_StaticMask,
    MM_DynamicMask
};

struct SpriteRendererInfo
{
    int FlipLeft = 0;
    int FlipUp = 0;
    float Temp1;
    float Temp2;
};

class CUISpriteRenderer : public CBaseRenderer
{
public:
    CUISpriteRenderer();
    virtual ~CUISpriteRenderer();

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render(const Matrix& view, const Matrix& projection) override;

    void SetAutoScaleRatio(float ratio)
    {
        AutoScaleRatio.X = ratio;
        AutoScaleRatio.Y = ratio;
    }

    void SetAutoScaleRatio(float4& ratio)
    {
        AutoScaleRatio = ratio;;
    }

    void AutoSpriteSizeOn() { m_bIsImageSize = true; }
    void AutoSpriteSizeOff() { m_bIsImageSize = false; }

    float4 GetScreenPosition();

private:
    float4      AutoScaleRatio = { 1.0f, 1.0f, 1.0f };      // Rendering ScaleRatio
    bool        m_bIsImageSize = false;                    // Rendering ImageSize

    CTransform* m_pTransform = nullptr;
};