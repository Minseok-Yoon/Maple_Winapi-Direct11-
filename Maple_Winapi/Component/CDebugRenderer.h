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

    static void Render(); // ��� ����� ������ ������
    static void Clear();  // �������� ���� �� ���� ����

    // ����� ���� Ÿ�� ������
    enum class DebugShapeType
    {
        Rect,
        Line,
        Circle
    };

    struct DebugShape 
    {
        DebugShapeType type;
        Vector3 point1;   // �簢��: bottomLeft, ��: start, ��: center
        Vector3 point2;   // �簢��: topRight, ��: end, ��: ��� �� ��
        float radius;     // �� ������ (�簢���� �������� ��� �� ��)
        Vector4 color;    // RGBA ���� (0.0f ~ 1.0f)
    };
    static std::vector<DebugShape> s_shapes;
};