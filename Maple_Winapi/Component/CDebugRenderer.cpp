#include "../pch.h"
#include "CDebugRenderer.h"

vector<CDebugRenderer::DebugShape> CDebugRenderer::s_shapes;

void CDebugRenderer::DrawRect(const Vector3& bottomLeft, const Vector3& topRight, const Vector4& color)
{
    DebugShape shape;
    shape.type = DebugShapeType::Rect;
    shape.point1 = bottomLeft;
    shape.point2 = topRight;
    shape.color = color;
    s_shapes.push_back(shape);
}

void CDebugRenderer::DrawLine(const Vector3& start, const Vector3& end, const Vector4& color)
{
    DebugShape shape;
    shape.type = DebugShapeType::Line;
    shape.point1 = start;
    shape.point2 = end;
    shape.color = color;
    s_shapes.push_back(shape);
}

void CDebugRenderer::DrawCircle(const Vector3& center, float radius, const Vector4& color)
{
    DebugShape shape;
    shape.type = DebugShapeType::Circle;
    shape.point1 = center;
    shape.radius = radius;
    shape.color = color;
    s_shapes.push_back(shape);
}

void CDebugRenderer::RenderRect(const Vector3& bottomLeft, const Vector3& topRight, const Vector4& color)
{
    // 사각형의 네 점 계산
    Vector3 topLeft(bottomLeft.x, topRight.y, bottomLeft.z);
    Vector3 bottomRight(topRight.x, bottomLeft.y, bottomLeft.z);

    // 사각형의 네 변을 선으로 그림
    RenderLine(bottomLeft, topLeft, color);
    RenderLine(topLeft, topRight, color);
    RenderLine(topRight, bottomRight, color);
    RenderLine(bottomRight, bottomLeft, color);
}

void CDebugRenderer::RenderLine(const Vector3& start, const Vector3& end, const Vector4& color)
{
    DrawLine(start, end, color);
}

void CDebugRenderer::RenderCircle(const Vector3& center, float radius, const Vector4& color)
{
    // 원을 그리기 위해 여러 개의 선을 사용하여 근사화
    const int segments = 16; // 세그먼트 수 (더 많으면 더 부드러운 원)
    const float step = 2.0f * 3.14159f / static_cast<float>(segments);

    for (int i = 0; i < segments; ++i)
    {
        float angle1 = step * static_cast<float>(i);
        float angle2 = step * static_cast<float>((i + 1) % segments);

        Vector3 point1(
            center.x + radius * cos(angle1),
            center.y + radius * sin(angle1),
            center.z
        );

        Vector3 point2(
            center.x + radius * cos(angle2),
            center.y + radius * sin(angle2),
            center.z
        );

        RenderLine(point1, point2, color);
    }
}

void CDebugRenderer::Render()
{
    for (const auto& shape : s_shapes)
    {
        switch (shape.type)
        {
        case DebugShapeType::Rect:
            RenderRect(shape.point1, shape.point2, shape.color);
            break;
        case DebugShapeType::Line:
            RenderLine(shape.point1, shape.point2, shape.color);
            break;
        case DebugShapeType::Circle:
            RenderCircle(shape.point1, shape.radius, shape.color);
            break;
        }
    }
}

void CDebugRenderer::Clear()
{
    s_shapes.clear();
}
