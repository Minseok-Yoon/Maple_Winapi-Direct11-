#include "../pch.h"
#include "CDebugManager.h"
#include <imgui.h>

CDebugManager::CDebugManager() : 
    m_showDebugInfo(true), 
    m_logToFile(false) 
{
    if (m_logToFile) {
        m_logFile.open("collision_log.txt", std::ios::out | std::ios::app);
    }
}

CDebugManager::~CDebugManager()
{
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void CDebugManager::CreateDebugWindow(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!m_showDebugInfo || !player || !collidingLine) return;

    ImGui::Begin("충돌 디버깅 정보");

    // 디버그 옵션
    ImGui::Checkbox("디버그 정보 표시", &m_showDebugInfo);
    ImGui::SameLine();
    if (ImGui::Checkbox("파일 로깅", &m_logToFile)) {
        if (m_logToFile && !m_logFile.is_open()) {
            m_logFile.open("collision_log.txt", std::ios::out | std::ios::app);
        }
        else if (!m_logToFile && m_logFile.is_open()) {
            m_logFile.close();
        }
    }

    ImGui::Separator();

    // 플레이어 로컬/월드 좌표 표시
    Vector2 localPos = player->GetLocalPosition();
    Vector2 worldPos = player->GetWorldPosition();
    ImGui::Text("플레이어 로컬 좌표: (%.2f, %.2f)", localPos.x, localPos.y);
    ImGui::Text("플레이어 월드 좌표: (%.2f, %.2f)", worldPos.x, worldPos.y);

    // 충돌 선분 정보
    Vector2 lineStart = collidingLine->GetStartPoint();
    Vector2 lineEnd = collidingLine->GetEndPoint();
    ImGui::Text("선분 시작점: (%.2f, %.2f)", lineStart.x, lineStart.y);
    ImGui::Text("선분 끝점: (%.2f, %.2f)", lineEnd.x, lineEnd.y);

    // 충돌 계산 정보
    float distance = CalculateDistance(worldPos, collidingLine);
    ImGui::Text("플레이어-선분 거리: %.2f", distance);
    ImGui::Text("충돌 상태: %s", distance <= player->GetCollisionThreshold() ? "충돌 중" : "충돌 없음");

    // 충돌 임계값 조정 슬라이더
    float threshold = player->GetCollisionThreshold();
    if (ImGui::SliderFloat("충돌 임계값", &threshold, 0.0f, 50.0f)) {
        player->SetCollisionThreshold(threshold);
    }

    // 로그 콘솔
    if (ImGui::CollapsingHeader("충돌 로그")) {
        ImGui::BeginChild("ScrollingLog", ImVec2(0, 200), true);
        for (const auto& log : m_consoleLog) {
            ImGui::TextWrapped("%s", log.c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();

        if (ImGui::Button("로그 지우기")) {
            m_consoleLog.clear();
        }
    }

    ImGui::End();
}

// 화면에 디버그 정보 렌더링
void CDebugManager::RenderDebugInfo(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!m_showDebugInfo || !player || !collidingLine) return;

    // 플레이어 히트박스 표시 (예: 사각형)
    Rect hitbox = player->GetHitbox();
    DrawRectangle(hitbox, RED, false); // 외곽선만 빨간색으로 표시

    // 실제 충돌 계산에 사용되는 위치 표시
    Vector2 collisionPoint = player->GetCollisionPoint();
    DrawCircle(collisionPoint.x, collisionPoint.y, 3.0f, YELLOW);

    // 선분 표시
    DrawLine(collidingLine->GetStartPoint(), collidingLine->GetEndPoint(), BLUE, 2.0f);

    // 충돌 지점까지의 최단 거리 표시
    Vector2 closestPoint = GetClosestPointOnLine(player->GetWorldPosition(), collidingLine);
    DrawLine(player->GetWorldPosition(), closestPoint, GREEN, 1.0f);

    // 현재 좌표계 표시
    DrawCoordinateSystem(player->GetWorldPosition(), 30.0f);
}

// 충돌 정보 로깅
void CDebugManager::LogCollisionInfo(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!player || !collidingLine) return;

    Vector2 worldPos = player->GetWorldPosition();
    float distance = CalculateDistance(worldPos, collidingLine);
    bool isColliding = distance <= player->GetCollisionThreshold();

    // 로그 메시지 생성
    char buffer[256];
    snprintf(buffer, sizeof(buffer),
        "시간: %.2f, 위치: (%.2f, %.2f), 거리: %.2f, 상태: %s",
        GetGameTime(), worldPos.x, worldPos.y, distance,
        isColliding ? "충돌" : "비충돌");

    std::string logMsg(buffer);

    // 콘솔 로그에 추가
    m_consoleLog.push_back(logMsg);
    if (m_consoleLog.size() > MAX_LOG_ENTRIES) {
        m_consoleLog.erase(m_consoleLog.begin());
    }

    // 파일 로깅
    if (m_logToFile && m_logFile.is_open()) {
        m_logFile << logMsg << std::endl;
        m_logFile.flush();
    }
}

void CDebugManager::Update(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!player || !collidingLine) return;

    // 충돌 상태 변경 시에만 로깅하려면 이전 상태를 저장해둘 필요가 있음
    static bool wasColliding = false;
    float distance = CalculateDistance(player->GetWorldPosition(), collidingLine);
    bool isColliding = distance <= player->GetCollisionThreshold();

    if (isColliding != wasColliding) {
        LogCollisionInfo(player, collidingLine);
        wasColliding = isColliding;
    }

    // ImGui 창 업데이트
    CreateDebugWindow(player, collidingLine);
}

void CDebugManager::Render(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!m_showDebugInfo || !player || !collidingLine) return;
    RenderDebugInfo(player, collidingLine);
}
