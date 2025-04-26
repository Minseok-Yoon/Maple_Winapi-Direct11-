#pragma once
#include <fstream>
#include "../Object/CGameObject.h"
#include "../Component/CLineRenderer.h"

class CDebugManager
{
public:
    CDebugManager();
    ~CDebugManager();

    void CreateDebugWindow(CGameObject* player, CLineRenderer* collidingLine);
    void RenderDebugInfo(CGameObject* player, CLineRenderer* collidingLine);
    void LogCollisionInfo(CGameObject* player, CLineRenderer* collidingLine);
    void Update(CGameObject* player, CLineRenderer* collidingLine);
    void Render(CGameObject* player, CLineRenderer* collidingLine);

private:
    // �÷��̾�� ���� ������ �Ÿ� ���
    float CalculateDistance(const Vector2& point, Line* line) {
        Vector2 start = line->GetStartPoint();
        Vector2 end = line->GetEndPoint();

        Vector2 lineDir = { end.x - start.x, end.y - start.y };
        float lineLengthSq = lineDir.x * lineDir.x + lineDir.y * lineDir.y;

        if (lineLengthSq == 0.0f) return Distance(point, start); // ������ ������ ��ҵ� ���

        // �����κ��� ���п� ���� ������ ���� ����
        float t = ((point.x - start.x) * lineDir.x + (point.y - start.y) * lineDir.y) / lineLengthSq;
        t = std::max(0.0f, std::min(1.0f, t)); // ���� ���� ������ ����

        Vector2 closestPoint = {
            start.x + t * lineDir.x,
            start.y + t * lineDir.y
        };

        return Distance(point, closestPoint);
    }

    // ���� ���� ���� ����� �� ���ϱ�
    Vector2 GetClosestPointOnLine(const Vector2& point, Line* line) {
        Vector2 start = line->GetStartPoint();
        Vector2 end = line->GetEndPoint();

        Vector2 lineDir = { end.x - start.x, end.y - start.y };
        float lineLengthSq = lineDir.x * lineDir.x + lineDir.y * lineDir.y;

        if (lineLengthSq == 0.0f) return start; // ������ ������ ��ҵ� ���

        // �����κ��� ���п� ���� ������ ���� ����
        float t = ((point.x - start.x) * lineDir.x + (point.y - start.y) * lineDir.y) / lineLengthSq;
        t = std::max(0.0f, std::min(1.0f, t)); // ���� ���� ������ ����

        return {
            start.x + t * lineDir.x,
            start.y + t * lineDir.y
        };
    }

    // �� �� ������ �Ÿ� ���
    float Distance(const Vector2& p1, const Vector2& p2) {
        float dx = p1.x - p2.x;
        float dy = p1.y - p2.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // ��ǥ�� �ð�ȭ
    void DrawCoordinateSystem(const Vector2& origin, float size) {
        // X�� (������)
        DrawLine({ origin.x - size, origin.y }, { origin.x + size, origin.y }, RED, 1.0f);
        DrawLine({ origin.x + size - 5, origin.y - 5 }, { origin.x + size, origin.y }, RED, 1.0f);
        DrawLine({ origin.x + size - 5, origin.y + 5 }, { origin.x + size, origin.y }, RED, 1.0f);

        // Y�� (���)
        DrawLine({ origin.x, origin.y - size }, { origin.x, origin.y + size }, GREEN, 1.0f);
        DrawLine({ origin.x - 5, origin.y - size + 5 }, { origin.x, origin.y - size }, GREEN, 1.0f);
        DrawLine({ origin.x + 5, origin.y - size + 5 }, { origin.x, origin.y - size }, GREEN, 1.0f);
    }

    // �簢�� �׸���
    void DrawRectangle(const Rect& rect, const Color& color, bool filled) {
        // �� �Լ��� �׷��� ���̺귯���� ���� ������ �޶��� �� �ֽ��ϴ�
        // ���� �����Դϴ�

        // �ܰ����� �׸��� ���
        if (!filled) {
            // �簢���� �� ���� �׸��ϴ�
            DrawLine({ rect.x, rect.y }, { rect.x + rect.width, rect.y }, color, 1.0f);
            DrawLine({ rect.x + rect.width, rect.y }, { rect.x + rect.width, rect.y + rect.height }, color, 1.0f);
            DrawLine({ rect.x + rect.width, rect.y + rect.height }, { rect.x, rect.y + rect.height }, color, 1.0f);
            DrawLine({ rect.x, rect.y + rect.height }, { rect.x, rect.y }, color, 1.0f);
        }
        // ä���� �簢�� ������ ����
    }

    // �� �׸���
    void DrawCircle(float x, float y, float radius, const Color& color) {
        // �� �Լ��� �׷��� ���̺귯���� ���� ������ �޶��� �� �ֽ��ϴ�
        // ���� �����Դϴ�
        // ���� ������ �׷��� API�� �°� �����ؾ� �մϴ�
    }

    // �� �׸���
    void DrawLine(const Vector2& start, const Vector2& end, const Color& color, float thickness) {
        // �� �Լ��� �׷��� ���̺귯���� ���� ������ �޶��� �� �ֽ��ϴ�
        // ���� �����Դϴ�
        // ���� ������ �׷��� API�� �°� �����ؾ� �մϴ�
    }

    // ���� �ð� �������� (�и���)
    float GetGameTime() {
        // ���� ������ ���� ������ �ð� �Լ��� ����ؾ� �մϴ�
        return 0.0f; // ���� ��ȯ��
    }

private:
    bool m_showDebugInfo;
    bool m_logToFile;
    std::ofstream m_logFile;
    std::vector<std::string> m_consoleLog;
    const int MAX_LOG_ENTRIES = 100;
};