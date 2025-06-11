#include "CCollider.h"
#include "../Object/CGameObject.h"
#include "../Component/CScript.h"
#include "../Component/CLineRenderer.h"
#include "../Module/SelectGDI.h"
#include "../Test/CDrawCollisionBox.h"
#include "../Component/CTransform.h"
#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Component/CDebugRenderer.h"
#include "../Component/CGravity.h"
#include <imgui.h>
#include "CLineCollider.h"

UINT CCollider::g_iNextID = 1;
bool CCollider::g_bRenderColliders = false;

CCollider::CCollider() :
    CComponent(COMPONENT_TYPE::CT_Collider),
    m_iID(g_iNextID++),
    m_vScale(Vector2(1.0f, 1.0f)),
    m_bEnable(true),
    m_vBottomLeft(Vector3(-0.5f, -0.5f, -1.0f)), // 기본 크기
    m_vTopRight(Vector3(0.5f, 0.5f, -1.0f)),    // 기본 크기
    m_vColor(Vector4(0.0f, 0.0f, 255.0f, 100.0f)), // 기본 색상 (파란색)
    m_pTransform(nullptr)
{
    if (this == nullptr) {
        OutputDebugStringA("CCollider instance is null\n");
        return;
    }
}
CCollider::~CCollider()
{
}

void CCollider::Init()
{
    // Transform 컴포넌트 참조 가져오기
    m_pTransform = GetOwner()->GetComponent<CTransform>();
    if (!m_pTransform)
    {
        OutputDebugStringA("Collider requires a Transform component\n");
    }
}

void CCollider::Update()
{
    // 부모 Transform이 있다면 월드 좌표 계산
    if (m_pTransform && m_pTransform->GetParent())
    {
        Matrix parentWorldMatrix = m_pTransform->GetParent()->GetWorldMatrix();
        // 로컬 충돌 박스 좌표를 월드 좌표로 변환
        Vector3 worldBottomLeft = Vector3::Transform(m_vBottomLeft, parentWorldMatrix);
        Vector3 worldTopRight = Vector3::Transform(m_vTopRight, parentWorldMatrix);
        // 변환된 좌표로 충돌 검사 및 렌더링
        RectCollider(m_vScale, m_vColor, worldBottomLeft, worldTopRight);

    }
    else
    {
        Vector3 worldBottomLeft = m_vBottomLeft;
        Vector3 worldTopRight = m_vTopRight;
        /// 충돌 상태에 따른 색상 변경
        if (m_bIsColliding)
        {
            if (m_vColor != Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
                // 충돌 전 색상이 저장되지 않았다면 저장
                if (m_vColor != Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
                    m_vOriginalColor = m_vColor;
                }
                m_vColor = Vector4(255.0f, 0.0f, 0.0f, 1.0f); // 충돌 시 빨간색
            }
        }
        else // 충돌 상태가 아닐 때
        {
            // 충돌이 해제되면 원래 색상으로 복원
            if (m_vColor == Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
                m_vColor = m_vOriginalColor;
            }
        }
        RectCollider(m_vScale, m_vColor, m_vBottomLeft, m_vTopRight);
    }
    /*if (!m_pTransform)
        return;

    // 월드 좌표 계산
    Matrix worldMatrix = m_pTransform->GetWorldMatrix();

    // 로컬 충돌 박스 좌표를 월드 좌표로 변환
    Vector3 worldBottomLeft = Vector3::Transform(m_vBottomLeft, worldMatrix);
    Vector3 worldTopRight = Vector3::Transform(m_vTopRight, worldMatrix);

    // 충돌 상태에 따른 색상 변경
    if (m_bIsColliding)
    {
        if (m_vColor != Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
            m_vOriginalColor = m_vColor;
            m_vColor = Vector4(255.0f, 0.0f, 0.0f, 1.0f); // 충돌 시 빨간색
        }
    }
    else
    {
        if (m_vColor == Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
            m_vColor = m_vOriginalColor;
        }
    }

    // 월드 좌표로 충돌 박스 시각화
    RectCollider(m_vScale, m_vColor, worldBottomLeft, worldTopRight);*/
}

void CCollider::LateUpdate()
{
}

void CCollider::Render(const Matrix& view, const Matrix& projection)
{
    // 충돌체가 비활성화된 경우, 또는 렌더링이 비활성화된 경우 렌더링 중단
    if (!m_bEnable || !g_bRenderColliders)
        return;
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
    CScript* sr = GetOwner()->GetComponent<CScript>();
    if (sr)
    {
        sr->OnCollisionEnter(_pOther);
    }

    //// 충돌한 오브젝트가 배경이라면 중력 해제
    //if (_pOther->GetOwner()->GetName() == L"BackGround")
    //{
    //    CGravity* gravity = GetOwner()->GetComponent<CGravity>();
    //    if (gravity)
    //    {
    //        gravity->SetGround(true);
    //    }
    //}

    m_bIsColliding = true;
}
void CCollider::OnCollisionStay(CCollider* _pOther)
{
    CScript* sr = GetOwner()->GetComponent<CScript>();
    if (sr)
    {
        sr->OnCollisionStay(_pOther);
    }
    m_bIsColliding = true; // 충돌 상태 업데이트
}
void CCollider::OnCollisionExit(CCollider* _pOther)
{
    m_bIsColliding = false;

    // 배경에서 벗어나면 중력 다시 적용
    /*if (_pOther->GetOwner()->GetName() == L"BackGround")
    {
        CGravity* gravity = GetOwner()->GetComponent<CGravity>();
        if (gravity)
        {
            gravity->SetGround(false);
        }
    }*/

    CScript* sr = GetOwner()->GetComponent<CScript>();
    if (sr)
    {
        sr->OnCollisionExit(_pOther);
    }
}

void CCollider::RectCollider(const Vector2& scale, const Vector4& color, const Vector3& bottomLeft, const Vector3& topRight)
{
    /*// 지역 변수로 사용하여 멤버 변수 오염 방지
    Vector3 localBottomLeft = Vector3(-0.5f * scale.x, -0.5f * scale.y, -1.0f);
    Vector3 localTopRight = Vector3(0.5f * scale.x, 0.5f * scale.y, -1.0f);

    CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
    if (lineRenderer == nullptr)
    {
        lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
        lineRenderer->SetFollowParent(true);  // 충돌박스는 부모를 따라다니도록 설정
    }

    // 지역 변수를 사용하여 라인 렌더러 업데이트
    lineRenderer->SetShape(localBottomLeft, localTopRight, color);*/
    CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
    if (lineRenderer == nullptr)
    {
        lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
        /*lineRenderer->SetFollowParent(true);
        lineRenderer->Init();*/
    }

    // 충돌 박스의 모서리 점들을 벡터로 구성
    std::vector<Vector3> points = {
        bottomLeft,                                     // 왼쪽 아래
        Vector3(topRight.x, bottomLeft.y, bottomLeft.z), // 오른쪽 아래
        topRight,                                       // 오른쪽 위
        Vector3(bottomLeft.x, topRight.y, topRight.z),  // 왼쪽 위
        bottomLeft                                      // 다시 처음으로 (닫힌 선)
    };

    // 라인 렌더러에 점 데이터 설정
    lineRenderer->SetLineData(points, color);
}

void CCollider::RenderUI()
{
    ImGui::Begin("Collider Inspector");

    // 현재 씬의 모든 게임 오브젝트를 가져온다.
    CScene* currentScene = CSceneManager::GetCurScene();
    if (!currentScene) {
        ImGui::Text("No current scene found.");
        ImGui::End();
        return;
    }

    wstring sceneName = CSceneManager::GetCurrentSceneName();
    ImGui::Text("Current Scene: %ls", sceneName.c_str());

    // 모든 레이어에서 게임 오브젝트 수집
    vector<CGameObject*> gameObjects;
    for (int i = static_cast<int>(LAYER_TYPE::LT_None) + 1; i < static_cast<int>(LAYER_TYPE::LT_End); i++) {
        vector<CGameObject*> layerObjects = currentScene->GetGameObjects(static_cast<LAYER_TYPE>(i));
        gameObjects.insert(gameObjects.end(), layerObjects.begin(), layerObjects.end());
    }

    if (gameObjects.empty()) {
        ImGui::Text("No game objects in the scene.");
        ImGui::End();
        return;
    }

    // 게임 오브젝트 선택 UI
    static int selectedIndex = 0;
    static vector<string> gameObjectNames;
    static vector<const char*> gameObjectNamesCStr;

    // 드롭다운 리스트 초기화
    gameObjectNames.clear();
    gameObjectNamesCStr.clear();

    // 게임 오브젝트 이름 수집 및 변환
    for (CGameObject* obj : gameObjects) {
        if (!obj || obj->IsDead()) continue;

        try {
            wstring name = obj->GetName();
            if (name.empty()) continue;

            string str = WStringToString(name);
            gameObjectNames.push_back(str);
        }
        catch (...) {
            OutputDebugStringA("Exception occurred while accessing object name\n");
            continue;
        }
    }

    // 문자열 포인터 벡터 초기화
    for (const string& name : gameObjectNames) {
        gameObjectNamesCStr.push_back(name.c_str());
    }

    ImGui::Text("Objects found: %d", (int)gameObjectNames.size());

    // 게임 오브젝트를 선택할 수 있는 드롭다운
    if (!gameObjectNamesCStr.empty()) {
        if (ImGui::Combo("Select Game Object", &selectedIndex, gameObjectNamesCStr.data(), (int)gameObjectNamesCStr.size())) {
            ImGui::Text("Selected Index: %d", selectedIndex);
        }
    }
    else {
        ImGui::Text("No selectable game objects.");
        ImGui::End();
        return;
    }

    // 선택된 오브젝트 UI 출력
    if (selectedIndex >= 0 && selectedIndex < gameObjects.size()) {
        CGameObject* selectedObject = gameObjects[selectedIndex];
        CCollider* selectedCollider = selectedObject->GetComponent<CCollider>();

        // 선분 충돌체 체크 추가
        CLineCollider* lineCollider = selectedObject->GetComponent<CLineCollider>();

        ImGui::Separator();
        ImGui::Text("Selected Object: %s", gameObjectNames[selectedIndex].c_str());

        if (selectedCollider) {
            // Collider 활성화 여부를 제어하는 체크박스
            if (ImGui::Checkbox("Activate Collider", &selectedCollider->m_bIsColliderActive)) {
                selectedCollider->SetColliderActive(selectedCollider->m_bIsColliderActive);
            }

            // 활성화되었을 때만 좌표 및 스케일 정보 표시
            if (selectedCollider->m_bIsColliderActive) {
                // 좌표 정보 추가
                ImGui::Separator();
                ImGui::Text("Coordinate Information");

                // 오브젝트 로컬 좌표
                ImGui::Text("Object");
                Vector3 localPos = selectedObject->GetComponent<CTransform>()->GetLocalPosition();
                ImGui::Text("Local Position: (%.2f, %.2f, %.2f)",
                    localPos.x, localPos.y, localPos.z);

                // 오브젝트 월드 좌표
                Vector3 worldPos = selectedObject->GetComponent<CTransform>()->GetWorldPosition();
                ImGui::Text("World Position: (%.2f, %.2f, %.2f)",
                    worldPos.x, worldPos.y, worldPos.z);

                // 선분 충돌체인 경우 다른 정보 표시
                if (lineCollider) {
                    ImGui::Separator();
                    ImGui::Text("Line Collider");

                    // 선분 정보 표시
                    Vector2 start = lineCollider->m_vStart;
                    Vector2 end = lineCollider->m_vEnd;
                    ImGui::Text("Start Point: (%.2f, %.2f)", start.x, start.y);
                    ImGui::Text("End Point: (%.2f, %.2f)", end.x, end.y);
                    ImGui::Text("Thickness: %.2f", lineCollider->m_fThickness);

                    // 세계 좌표계 충돌 박스 정보
                    ImGui::Text("Collider World Min: (%.2f, %.2f, %.2f)",
                        lineCollider->m_vWorldBottomLeft.x,
                        lineCollider->m_vWorldBottomLeft.y,
                        lineCollider->m_vWorldBottomLeft.z);
                    ImGui::Text("Collider World Max: (%.2f, %.2f, %.2f)",
                        lineCollider->m_vWorldTopRight.x,
                        lineCollider->m_vWorldTopRight.y,
                        lineCollider->m_vWorldTopRight.z);

                    // 선분 편집 UI
                    ImGui::Separator();
                    ImGui::Text("Edit Line Collider");

                    bool changed = false;
                    changed |= ImGui::DragFloat2("Start Point", &start.x, 0.1f);
                    changed |= ImGui::DragFloat2("End Point", &end.x, 0.1f);
                    float thickness = lineCollider->m_fThickness;
                    changed |= ImGui::SliderFloat("Thickness", &thickness, 0.01f, 1.0f);

                    if (changed) {
                        lineCollider->SetLine(start, end, thickness);

                        // LineRenderer도 함께 업데이트
                        CLineRenderer* lineRenderer = selectedObject->GetComponent<CLineRenderer>();
                        if (lineRenderer) {
                            std::vector<Vector3> linePoints = {
                                Vector3(start.x, start.y, 0.0f),
                                Vector3(end.x, end.y, 0.0f)
                            };
                            Vector4 lineColor = selectedCollider->m_vColor;
                            lineRenderer->SetLineData(linePoints, lineColor);
                        }
                    }

                    // 색상 편집
                    ImGui::Text("Line Color");
                    if (ImGui::ColorEdit4("Color", &selectedCollider->m_vColor.x)) {
                        // 라인 렌더러 색상 업데이트
                        CLineRenderer* lineRenderer = selectedObject->GetComponent<CLineRenderer>();
                        if (lineRenderer) {
                            std::vector<Vector3> linePoints = {
                                Vector3(start.x, start.y, 0.0f),
                                Vector3(end.x, end.y, 0.0f)
                            };
                            lineRenderer->SetLineData(linePoints, selectedCollider->m_vColor);
                        }
                    }
                }
                // 일반 충돌체인 경우 기존 UI 표시
                else {
                    // 충돌체 로컬 좌표
                    ImGui::Separator();
                    ImGui::Text("Collider");
                    ImGui::Text("Collider Local Min: (%.2f, %.2f, %.2f)",
                        selectedCollider->m_vBottomLeft.x,
                        selectedCollider->m_vBottomLeft.y,
                        selectedCollider->m_vBottomLeft.z);
                    ImGui::Text("Collider Local Max: (%.2f, %.2f, %.2f)",
                        selectedCollider->m_vTopRight.x,
                        selectedCollider->m_vTopRight.y,
                        selectedCollider->m_vTopRight.z);

                    // 충돌체 월드 좌표 계산
                    Vector3 worldBottomLeft = selectedCollider->m_vBottomLeft + worldPos;
                    Vector3 worldTopRight = selectedCollider->m_vTopRight + worldPos;
                    ImGui::Text("Collider World Min: (%.2f, %.2f, %.2f)",
                        worldBottomLeft.x,
                        worldBottomLeft.y,
                        worldBottomLeft.z);
                    ImGui::Text("Collider World Max: (%.2f, %.2f, %.2f)",
                        worldTopRight.x,
                        worldTopRight.y,
                        worldTopRight.z);

                    // 기존 좌표 정보 출력 코드 다음에 추가
                    ImGui::Separator();
                    ImGui::Text("Size Information");

                    // 오브젝트의 Transform 크기
                    Vector3 objectScale = selectedObject->GetComponent<CTransform>()->GetLocalScale();
                    ImGui::Text("Object Scale: (%.2f, %.2f, %.2f)",
                        objectScale.x, objectScale.y, objectScale.z);

                    // 충돌체 로컬 크기 계산
                    Vector3 colliderLocalSize = selectedCollider->m_vTopRight - selectedCollider->m_vBottomLeft;
                    ImGui::Text("Collider Local Size: (%.2f, %.2f, %.2f)",
                        colliderLocalSize.x, colliderLocalSize.y, colliderLocalSize.z);

                    // 충돌체 월드 크기 계산 (오브젝트 스케일 고려)
                    Vector3 colliderWorldSize = Vector3(
                        colliderLocalSize.x * objectScale.x,
                        colliderLocalSize.y * objectScale.y,
                        colliderLocalSize.z * objectScale.z
                    );
                    ImGui::Text("Collider World Size: (%.2f, %.2f, %.2f)",
                        colliderWorldSize.x, colliderWorldSize.y, colliderWorldSize.z);

                    // Collider 스케일 및 색상 편집 UI
                    if (selectedCollider->m_bEnable) {
                        ImGui::Separator();
                        ImGui::Text("Collider Scale");
                        bool scaleChanged = false;
                        scaleChanged |= ImGui::SliderFloat("Scale X", &selectedCollider->m_vScale.x, 0.1f, 50.0f);
                        scaleChanged |= ImGui::SliderFloat("Scale Y", &selectedCollider->m_vScale.y, 0.1f, 50.0f);

                        ImGui::Text("Collider Color");
                        bool colorChanged = ImGui::ColorEdit4("Color", &selectedCollider->m_vColor.x);

                        // 스케일 또는 색상 변경 시 처리
                        if (scaleChanged || colorChanged) {
                            selectedCollider->m_vBottomLeft = Vector3(-0.5f * selectedCollider->m_vScale.x,
                                -0.5f * selectedCollider->m_vScale.y,
                                -1.0f);
                            selectedCollider->m_vTopRight = Vector3(0.5f * selectedCollider->m_vScale.x,
                                0.5f * selectedCollider->m_vScale.y,
                                -1.0f);

                            selectedCollider->RectCollider(
                                selectedCollider->m_vScale,
                                selectedCollider->m_vColor,
                                selectedCollider->m_vBottomLeft,
                                selectedCollider->m_vTopRight
                            );
                        }
                    }
                }
            }
        }
        else {
            ImGui::Text("No Collider found in this object.");
        }
    }
    else {
        ImGui::Text("Invalid object selection.");
    }

    ImGui::End();
}

string CCollider::WStringToString(const wstring& wstr)
{
	/*if (wstr.empty()) return "";
	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], sizeNeeded, NULL, NULL);
	return result;*/
    if (wstr.empty()) return "";

    try {
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        if (sizeNeeded <= 0) return "";

        string result(sizeNeeded, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], sizeNeeded, NULL, NULL);
        return result;
    }
    catch (...) {
        return "[Invalid wstring]";
    }
}

//void CCollider::RectCollider(const Vector2& scale, const Vector4& color, const Vector3& bottomLeft, const Vector3& topRight)
//{
//    CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
//    if (lineRenderer == nullptr)
//    {
//        lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
//    }
//
//    // 오브젝트의 Transform을 가져와서 위치를 반영
//    CGameObject* owner = GetOwner();
//    CTransform* transform = owner->GetComponent<CTransform>();
//    Vector3 objectPos = Vector3(0.0f, 0.0f, 0.0f);  // 기본값 설정
//
//    if (transform == nullptr)
//    {
//        OutputDebugStringA("CTransform component not found!\n");
//    }
//    else
//    {
//        objectPos = transform->GetTransformPosition();
//    }
//
//    // 충돌 박스의 크기는 m_vScale에 맞게 조정
//    // m_vScale 값을 사용하여 bottomLeft와 topRight 값을 계산
//    Vector3 adjustedBottomLeft = objectPos + Vector3(-scale.x * 0.5f, -scale.y * 0.5f, 0.0f);
//    Vector3 adjustedTopRight = objectPos + Vector3(scale.x * 0.5f, scale.y * 0.5f, 0.0f);
//
//    // `lineRenderer`로 충돌 박스를 그리기
//    lineRenderer->SetShape(bottomLeft, topRight, color);
//}