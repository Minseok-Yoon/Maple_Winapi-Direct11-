#include "CUISpriteRenderer.h"
#include "../Core/CCore.h"

CUISpriteRenderer::CUISpriteRenderer()
{
}

CUISpriteRenderer::~CUISpriteRenderer()
{
}

void CUISpriteRenderer::Init()
{
}

void CUISpriteRenderer::Update()
{
}

void CUISpriteRenderer::LateUpdate()
{
}

void CUISpriteRenderer::Render(const Matrix& view, const Matrix& projection)
{
}

float4 CUISpriteRenderer::GetScreenPosition()
{
    /*float4x4 ViewPort;
    float4 ScreenPos = m_pTransform->GetWorldPosition();
    float4 Scale = CCore::GetInst()->GetScale();
    ViewPort.ViewPort(Scale.X, Scale.Y, 0.0f, 0.0f);
    ScreenPos *= ViewPort.InverseReturn();
    ScreenPos *= m_pTransform->GetConstTransformDataRef().ProjectionMatrix.InverseReturn();
    ScreenPos *= m_pTransform->GetConstTransformDataRef().ViewMatrix.InverseReturn();
    return ScreenPos;*/
}
