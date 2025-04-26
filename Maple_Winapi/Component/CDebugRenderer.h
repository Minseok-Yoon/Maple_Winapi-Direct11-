#pragma once
#include "../CMath.h"

using namespace math;

class CDebugRenderer
{
public:
    static void DrawRect(const Vector3& bottomLeft, const Vector3& topRight, const Vector4& color);
    static void DrawLine(const Vector3& start, const Vector3& end, const Vector4& color);
    static void DrawCircle(const Vector3& center, float radius, const Vector4& color);

    static void RenderRect(const Vector3& bottomLeft, const Vector3& topRight, const Vector4& color);
    static void RenderLine(const Vector3& start, const Vector3& end, const Vector4& color);
    static void RenderCircle(const Vector3& center, float radius, const Vector4& color);

    static void Render(); // 모든 디버그 도형을 렌더링
    static void Clear();  // 프레임이 끝날 때 버퍼 정리

    // 디버그 도형 타입 열거형
    enum class DebugShapeType
    {
        Rect,
        Line,
        Circle
    };

    struct DebugShape 
    {
        DebugShapeType type;
        Vector3 point1;   // 사각형: bottomLeft, 선: start, 원: center
        Vector3 point2;   // 사각형: topRight, 선: end, 원: 사용 안 함
        float radius;     // 원 반지름 (사각형과 선에서는 사용 안 함)
        Vector4 color;    // RGBA 색상 (0.0f ~ 1.0f)
    };
    static std::vector<DebugShape> s_shapes;
};