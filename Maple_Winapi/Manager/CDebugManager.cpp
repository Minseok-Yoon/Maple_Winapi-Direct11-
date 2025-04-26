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

    ImGui::Begin("�浹 ����� ����");

    // ����� �ɼ�
    ImGui::Checkbox("����� ���� ǥ��", &m_showDebugInfo);
    ImGui::SameLine();
    if (ImGui::Checkbox("���� �α�", &m_logToFile)) {
        if (m_logToFile && !m_logFile.is_open()) {
            m_logFile.open("collision_log.txt", std::ios::out | std::ios::app);
        }
        else if (!m_logToFile && m_logFile.is_open()) {
            m_logFile.close();
        }
    }

    ImGui::Separator();

    // �÷��̾� ����/���� ��ǥ ǥ��
    Vector2 localPos = player->GetLocalPosition();
    Vector2 worldPos = player->GetWorldPosition();
    ImGui::Text("�÷��̾� ���� ��ǥ: (%.2f, %.2f)", localPos.x, localPos.y);
    ImGui::Text("�÷��̾� ���� ��ǥ: (%.2f, %.2f)", worldPos.x, worldPos.y);

    // �浹 ���� ����
    Vector2 lineStart = collidingLine->GetStartPoint();
    Vector2 lineEnd = collidingLine->GetEndPoint();
    ImGui::Text("���� ������: (%.2f, %.2f)", lineStart.x, lineStart.y);
    ImGui::Text("���� ����: (%.2f, %.2f)", lineEnd.x, lineEnd.y);

    // �浹 ��� ����
    float distance = CalculateDistance(worldPos, collidingLine);
    ImGui::Text("�÷��̾�-���� �Ÿ�: %.2f", distance);
    ImGui::Text("�浹 ����: %s", distance <= player->GetCollisionThreshold() ? "�浹 ��" : "�浹 ����");

    // �浹 �Ӱ谪 ���� �����̴�
    float threshold = player->GetCollisionThreshold();
    if (ImGui::SliderFloat("�浹 �Ӱ谪", &threshold, 0.0f, 50.0f)) {
        player->SetCollisionThreshold(threshold);
    }

    // �α� �ܼ�
    if (ImGui::CollapsingHeader("�浹 �α�")) {
        ImGui::BeginChild("ScrollingLog", ImVec2(0, 200), true);
        for (const auto& log : m_consoleLog) {
            ImGui::TextWrapped("%s", log.c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();

        if (ImGui::Button("�α� �����")) {
            m_consoleLog.clear();
        }
    }

    ImGui::End();
}

// ȭ�鿡 ����� ���� ������
void CDebugManager::RenderDebugInfo(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!m_showDebugInfo || !player || !collidingLine) return;

    // �÷��̾� ��Ʈ�ڽ� ǥ�� (��: �簢��)
    Rect hitbox = player->GetHitbox();
    DrawRectangle(hitbox, RED, false); // �ܰ����� ���������� ǥ��

    // ���� �浹 ��꿡 ���Ǵ� ��ġ ǥ��
    Vector2 collisionPoint = player->GetCollisionPoint();
    DrawCircle(collisionPoint.x, collisionPoint.y, 3.0f, YELLOW);

    // ���� ǥ��
    DrawLine(collidingLine->GetStartPoint(), collidingLine->GetEndPoint(), BLUE, 2.0f);

    // �浹 ���������� �ִ� �Ÿ� ǥ��
    Vector2 closestPoint = GetClosestPointOnLine(player->GetWorldPosition(), collidingLine);
    DrawLine(player->GetWorldPosition(), closestPoint, GREEN, 1.0f);

    // ���� ��ǥ�� ǥ��
    DrawCoordinateSystem(player->GetWorldPosition(), 30.0f);
}

// �浹 ���� �α�
void CDebugManager::LogCollisionInfo(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!player || !collidingLine) return;

    Vector2 worldPos = player->GetWorldPosition();
    float distance = CalculateDistance(worldPos, collidingLine);
    bool isColliding = distance <= player->GetCollisionThreshold();

    // �α� �޽��� ����
    char buffer[256];
    snprintf(buffer, sizeof(buffer),
        "�ð�: %.2f, ��ġ: (%.2f, %.2f), �Ÿ�: %.2f, ����: %s",
        GetGameTime(), worldPos.x, worldPos.y, distance,
        isColliding ? "�浹" : "���浹");

    std::string logMsg(buffer);

    // �ܼ� �α׿� �߰�
    m_consoleLog.push_back(logMsg);
    if (m_consoleLog.size() > MAX_LOG_ENTRIES) {
        m_consoleLog.erase(m_consoleLog.begin());
    }

    // ���� �α�
    if (m_logToFile && m_logFile.is_open()) {
        m_logFile << logMsg << std::endl;
        m_logFile.flush();
    }
}

void CDebugManager::Update(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!player || !collidingLine) return;

    // �浹 ���� ���� �ÿ��� �α��Ϸ��� ���� ���¸� �����ص� �ʿ䰡 ����
    static bool wasColliding = false;
    float distance = CalculateDistance(player->GetWorldPosition(), collidingLine);
    bool isColliding = distance <= player->GetCollisionThreshold();

    if (isColliding != wasColliding) {
        LogCollisionInfo(player, collidingLine);
        wasColliding = isColliding;
    }

    // ImGui â ������Ʈ
    CreateDebugWindow(player, collidingLine);
}

void CDebugManager::Render(CGameObject* player, CLineRenderer* collidingLine)
{
    if (!m_showDebugInfo || !player || !collidingLine) return;
    RenderDebugInfo(player, collidingLine);
}
