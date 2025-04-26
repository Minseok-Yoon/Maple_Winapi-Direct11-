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
    // 플레이어와 선분 사이의 거리 계산
    float CalculateDistance(const Vector2& point, Line* line) {
        Vector2 start = line->GetStartPoint();
        Vector2 end = line->GetEndPoint();

        Vector2 lineDir = { end.x - start.x, end.y - start.y };
        float lineLengthSq = lineDir.x * lineDir.x + lineDir.y * lineDir.y;

        if (lineLengthSq == 0.0f) return Distance(point, start); // 선분이 점으로 축소된 경우

        // 점으로부터 선분에 내린 수선의 발을 구함
        float t = ((point.x - start.x) * lineDir.x + (point.y - start.y) * lineDir.y) / lineLengthSq;
        t = std::max(0.0f, std::min(1.0f, t)); // 선분 위의 점으로 제한

        Vector2 closestPoint = {
            start.x + t * lineDir.x,
            start.y + t * lineDir.y
        };

        return Distance(point, closestPoint);
    }

    // 선분 위의 가장 가까운 점 구하기
    Vector2 GetClosestPointOnLine(const Vector2& point, Line* line) {
        Vector2 start = line->GetStartPoint();
        Vector2 end = line->GetEndPoint();

        Vector2 lineDir = { end.x - start.x, end.y - start.y };
        float lineLengthSq = lineDir.x * lineDir.x + lineDir.y * lineDir.y;

        if (lineLengthSq == 0.0f) return start; // 선분이 점으로 축소된 경우

        // 점으로부터 선분에 내린 수선의 발을 구함
        float t = ((point.x - start.x) * lineDir.x + (point.y - start.y) * lineDir.y) / lineLengthSq;
        t = std::max(0.0f, std::min(1.0f, t)); // 선분 위의 점으로 제한

        return {
            start.x + t * lineDir.x,
            start.y + t * lineDir.y
        };
    }

    // 두 점 사이의 거리 계산
    float Distance(const Vector2& p1, const Vector2& p2) {
        float dx = p1.x - p2.x;
        float dy = p1.y - p2.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // 좌표계 시각화
    void DrawCoordinateSystem(const Vector2& origin, float size) {
        // X축 (빨간색)
        DrawLine({ origin.x - size, origin.y }, { origin.x + size, origin.y }, RED, 1.0f);
        DrawLine({ origin.x + size - 5, origin.y - 5 }, { origin.x + size, origin.y }, RED, 1.0f);
        DrawLine({ origin.x + size - 5, origin.y + 5 }, { origin.x + size, origin.y }, RED, 1.0f);

        // Y축 (녹색)
        DrawLine({ origin.x, origin.y - size }, { origin.x, origin.y + size }, GREEN, 1.0f);
        DrawLine({ origin.x - 5, origin.y - size + 5 }, { origin.x, origin.y - size }, GREEN, 1.0f);
        DrawLine({ origin.x + 5, origin.y - size + 5 }, { origin.x, origin.y - size }, GREEN, 1.0f);
    }

    // 사각형 그리기
    void DrawRectangle(const Rect& rect, const Color& color, bool filled) {
        // 이 함수는 그래픽 라이브러리에 따라 구현이 달라질 수 있습니다
        // 예시 구현입니다

        // 외곽선만 그리는 경우
        if (!filled) {
            // 사각형의 네 변을 그립니다
            DrawLine({ rect.x, rect.y }, { rect.x + rect.width, rect.y }, color, 1.0f);
            DrawLine({ rect.x + rect.width, rect.y }, { rect.x + rect.width, rect.y + rect.height }, color, 1.0f);
            DrawLine({ rect.x + rect.width, rect.y + rect.height }, { rect.x, rect.y + rect.height }, color, 1.0f);
            DrawLine({ rect.x, rect.y + rect.height }, { rect.x, rect.y }, color, 1.0f);
        }
        // 채워진 사각형 구현은 생략
    }

    // 원 그리기
    void DrawCircle(float x, float y, float radius, const Color& color) {
        // 이 함수는 그래픽 라이브러리에 따라 구현이 달라질 수 있습니다
        // 예시 구현입니다
        // 실제 구현은 그래픽 API에 맞게 수정해야 합니다
    }

    // 선 그리기
    void DrawLine(const Vector2& start, const Vector2& end, const Color& color, float thickness) {
        // 이 함수는 그래픽 라이브러리에 따라 구현이 달라질 수 있습니다
        // 예시 구현입니다
        // 실제 구현은 그래픽 API에 맞게 수정해야 합니다
    }

    // 게임 시간 가져오기 (밀리초)
    float GetGameTime() {
        // 실제 구현은 게임 엔진의 시간 함수를 사용해야 합니다
        return 0.0f; // 예시 반환값
    }

private:
    bool m_showDebugInfo;
    bool m_logToFile;
    std::ofstream m_logFile;
    std::vector<std::string> m_consoleLog;
    const int MAX_LOG_ENTRIES = 100;
};