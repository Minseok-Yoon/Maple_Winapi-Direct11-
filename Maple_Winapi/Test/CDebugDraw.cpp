#include "../pch.h"
#include "CDebugDraw.h"
#include "../Component/CLineRenderer.h"

void CDebugDraw::DrawBox(const Vector3& center, const Vector2& size, const TextureColor& color)
{
    //Vector3 tl = center + Vector3(-size.x * 0.5f, size.y * 0.5f, 0);
    //Vector3 tr = center + Vector3(size.x * 0.5f, size.y * 0.5f, 0);
    //Vector3 br = center + Vector3(size.x * 0.5f, -size.y * 0.5f, 0);
    //Vector3 bl = center + Vector3(-size.x * 0.5f, -size.y * 0.5f, 0);

    //// 선분 렌더링 (엔진 방식에 맞게 구현)
    //CLineRenderer::DrawLine(tl, tr, color);
    //CLineRenderer::DrawLine(tr, br, color);
    //CLineRenderer::DrawLine(br, bl, color);
    //CLineRenderer::DrawLine(bl, tl, color);
    Vector3 tl = center + Vector3(-size.x * 0.5f, size.y * 0.5f, 0);
    Vector3 tr = center + Vector3(size.x * 0.5f, size.y * 0.5f, 0);
    Vector3 br = center + Vector3(size.x * 0.5f, -size.y * 0.5f, 0);
    Vector3 bl = center + Vector3(-size.x * 0.5f, -size.y * 0.5f, 0);

    // LINELIST 방식: 각 선분마다 시작점과 끝점을 쌍으로 추가
    //m_pLineRenderer->DrawLine(tl, tr, color); // 상단
    //m_pLineRenderer->DrawLine(tr, br, color); // 우측
    //m_pLineRenderer->DrawLine(br, bl, color); // 하단
    //m_pLineRenderer->DrawLine(bl, tl, color); // 좌측
}

void CDebugDraw::DrawDot(const Vector3& center, const Vector4& color, float size /* = 2.0f */)
{
}