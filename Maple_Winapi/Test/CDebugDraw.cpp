#include "../pch.h"
#include "CDebugDraw.h"
#include "../Component/CLineRenderer.h"

void CDebugDraw::DrawBox(const Vector3& center, const Vector2& size, const TextureColor& color)
{
    //Vector3 tl = center + Vector3(-size.x * 0.5f, size.y * 0.5f, 0);
    //Vector3 tr = center + Vector3(size.x * 0.5f, size.y * 0.5f, 0);
    //Vector3 br = center + Vector3(size.x * 0.5f, -size.y * 0.5f, 0);
    //Vector3 bl = center + Vector3(-size.x * 0.5f, -size.y * 0.5f, 0);

    //// ���� ������ (���� ��Ŀ� �°� ����)
    //CLineRenderer::DrawLine(tl, tr, color);
    //CLineRenderer::DrawLine(tr, br, color);
    //CLineRenderer::DrawLine(br, bl, color);
    //CLineRenderer::DrawLine(bl, tl, color);
    Vector3 tl = center + Vector3(-size.x * 0.5f, size.y * 0.5f, 0);
    Vector3 tr = center + Vector3(size.x * 0.5f, size.y * 0.5f, 0);
    Vector3 br = center + Vector3(size.x * 0.5f, -size.y * 0.5f, 0);
    Vector3 bl = center + Vector3(-size.x * 0.5f, -size.y * 0.5f, 0);

    // LINELIST ���: �� ���и��� �������� ������ ������ �߰�
    //m_pLineRenderer->DrawLine(tl, tr, color); // ���
    //m_pLineRenderer->DrawLine(tr, br, color); // ����
    //m_pLineRenderer->DrawLine(br, bl, color); // �ϴ�
    //m_pLineRenderer->DrawLine(bl, tl, color); // ����
}

void CDebugDraw::DrawDot(const Vector3& center, const Vector4& color, float size /* = 2.0f */)
{
}