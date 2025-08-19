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

CCollider::CCollider()
    : CComponent(COMPONENT_TYPE::CT_Collider)
    , m_iID(g_iNextID++)
{
    RebuildLocal();
}

CCollider::~CCollider() {}

void CCollider::Init()
{
    m_pTransform = GetOwner()->GetComponent<CTransform>();
    if (!m_pTransform)
        OutputDebugStringA("[Collider] requires CTransform\n");

    // 라인 렌더러 보장
    if (!GetOwner()->GetComponent<CLineRenderer>())
        GetOwner()->AddComponent<CLineRenderer>();

    if (CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>())
    {
        if (CTexture* tex = sr->GetTexture())
        {
            Vector2 texSize2(1.0f, 1.0f);
            texSize2 = Vector2(static_cast<float>(tex->GetWidth()),
                static_cast<float>(tex->GetHeight()));

            Vector3 localScale = m_pTransform ? m_pTransform->GetLocalScale() : Vector3(1.0f, 1.0f, 1.0f);

            m_vSize = Vector2(texSize2.x * localScale.x,
                texSize2.y * localScale.y);

            RebuildLocal();
        }
    }
    else if (m_pTransform) {
        Vector3 scale = m_pTransform->GetLocalScale();
        // Transform의 스케일을 그대로 콜라이더 크기로 사용 (필요시 곱셈 보정)
        m_vSize = Vector2(scale.x, scale.y);
        RebuildLocal();
    }
}

void CCollider::Update()
{
    if (!m_pTransform) return;

    // 월드 코너 갱신
    UpdateWorldCorners();

    // 충돌 시 색상 스왑
    static const Vector4 kRed(1.f, 0.f, 0.f, 1.f);
    if (m_bIsColliding) {
        if (m_vColor.x != kRed.x || m_vColor.y != kRed.y || m_vColor.z != kRed.z || m_vColor.w != kRed.w) {
            m_vOriginalColor = m_vColor;
            m_vColor = kRed;
        }
    }
    else {
        // 이전에 빨강이었다면 원색 복구
        if (m_vOriginalColor.w > 0.f &&
            m_vColor.x == kRed.x && m_vColor.y == kRed.y && m_vColor.z == kRed.z)
        {
            m_vColor = m_vOriginalColor;
        }
    }

    // 라인 데이터 공급: **로컬 좌표** 전달 (Transform->Bind()로 월드 변환 적용됨)
    if (auto* lr = GetOwner()->GetComponent<CLineRenderer>()) {
        float zOffset = 0.1f;
        Vector3 blLocal = m_vBottomLeft + Vector3(0.f, 0.f, zOffset);
        Vector3 brLocal = Vector3(m_vTopRight.x, m_vBottomLeft.y, m_vBottomLeft.z + zOffset);
        Vector3 trLocal = m_vTopRight + Vector3(0.f, 0.f, zOffset);
        Vector3 tlLocal = Vector3(m_vBottomLeft.x, m_vTopRight.y, m_vTopRight.z + zOffset);

        std::vector<Vector3> pts = { blLocal, brLocal, trLocal, tlLocal, blLocal };
        lr->SetLineData(pts, m_vColor); // 로컬 좌표 모드(기본)
    }
    //// 라인 데이터 공급(닫힌 라인스트립)
    //if (auto* lr = GetOwner()->GetComponent<CLineRenderer>()) {
    //    std::vector<Vector3> pts = { m_blW, m_brW, m_trW, m_tlW, m_blW };
    //    lr->SetLineData(pts, m_vColor);
    //}
}

void CCollider::LateUpdate()
{
}

void CCollider::Render(const Matrix& view, const Matrix& projection)
{
    if (!m_bEnable || !g_bRenderColliders) return;

    if (auto* lr = GetOwner()->GetComponent<CLineRenderer>())
        lr->Render(view, projection); // 실제 드로우는 라인렌더러가 수행
}

void CCollider::RebuildLocal()
{
    // 오프셋 포함한 로컬 중심
    const Vector3 c(m_vCenter.x + m_vOffsetPos.x,
        m_vCenter.y + m_vOffsetPos.y,
        0.0f);
    const Vector3 half(m_vSize.x * 0.5f, m_vSize.y * 0.5f, 0.0f);
    m_vBottomLeft = c + Vector3(-half.x, -half.y, 0.0f);
    m_vTopRight = c + Vector3(half.x, half.y, 0.0f);
}

void CCollider::UpdateWorldCorners()
{
    const Matrix world = m_pTransform ? m_pTransform->GetWorldMatrix() : Matrix::Identity;

    const Vector3 bl = m_vBottomLeft;
    const Vector3 br(m_vTopRight.x, m_vBottomLeft.y, m_vBottomLeft.z);
    const Vector3 tr = m_vTopRight;
    const Vector3 tl(m_vBottomLeft.x, m_vTopRight.y, m_vTopRight.z);

    m_blW = Vector3::Transform(bl, world);
    m_brW = Vector3::Transform(br, world);
    m_trW = Vector3::Transform(tr, world);
    m_tlW = Vector3::Transform(tl, world);
}

// 충돌 콜백: 상태 토글 + 사용자 스크립트 전달
void CCollider::OnCollisionEnter(CCollider* other)
{
    m_bIsColliding = true;
    if (auto* sr = GetOwner()->GetComponent<CScript>())
        sr->OnCollisionEnter(other);
}
void CCollider::OnCollisionStay(CCollider* other)
{
    m_bIsColliding = true;
    if (auto* sr = GetOwner()->GetComponent<CScript>())
        sr->OnCollisionStay(other);
}
void CCollider::OnCollisionExit(CCollider* other)
{
    m_bIsColliding = false;
    if (auto* sr = GetOwner()->GetComponent<CScript>())
        sr->OnCollisionExit(other);
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

    CScene* dontDestroyScene = CSceneManager::GetDontDestroyOnLoad();
    if (dontDestroyScene) {
        for (int i = static_cast<int>(LAYER_TYPE::LT_None) + 1; i < static_cast<int>(LAYER_TYPE::LT_End); i++) {
            vector<CGameObject*> layerObjects = dontDestroyScene->GetGameObjects(static_cast<LAYER_TYPE>(i));
            gameObjects.insert(gameObjects.end(), layerObjects.begin(), layerObjects.end());
        }
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
                                -2.0f);
                            selectedCollider->m_vTopRight = Vector3(0.5f * selectedCollider->m_vScale.x,
                                0.5f * selectedCollider->m_vScale.y,
                                -2.0f);

                            /*selectedCollider->RectCollider(
                                selectedCollider->m_vScale,
                                selectedCollider->m_vColor,
                                selectedCollider->m_vBottomLeft,
                                selectedCollider->m_vTopRight
                            );*/
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
 
//UINT CCollider::g_iNextID = 1;
//bool CCollider::g_bRenderColliders = false;
//
//CCollider::CCollider() :
//    CComponent(COMPONENT_TYPE::CT_Collider),
//    m_iID(g_iNextID++),
//    m_vScale(Vector2(1.0f, 1.0f)),
//    m_bEnable(true),
//    m_vBottomLeft(Vector3(-0.5f, -0.5f, -1.0f)), // 기본 크기
//    m_vTopRight(Vector3(0.5f, 0.5f, -1.0f)),    // 기본 크기
//    m_vColor(Vector4(0.0f, 0.0f, 255.0f, 100.0f)), // 기본 색상 (파란색)
//    m_pTransform(nullptr)
//{
//}
//CCollider::~CCollider()
//{
//}
//
//void CCollider::Init()
//{
//    // Transform 컴포넌트 참조 가져오기
//    m_pTransform = GetOwner()->GetComponent<CTransform>();
//    if (!m_pTransform)
//    {
//        OutputDebugStringA("Collider requires a Transform component\n");
//    }
//}
//
//void CCollider::Update()
//{
//    if (!m_pTransform) return;
//
//    // 항상 자신의 월드 행렬 사용
//    const Matrix world = m_pTransform->GetWorldMatrix();
//
//    const Vector3 localCenter(m_vCenter.x + m_vOffsetPos.x,
//                              m_vCenter.y + m_vOffsetPos.y,
//                              m_vCenter.z);
//
//    const Vector3 halfSize = Vector3(
//        (m_vTopRight.x - m_vBottomLeft.x) * 0.5f,
//        (m_vTopRight.y - m_vBottomLeft.y) * 0.5f,
//        0.0f
//    );
//
//    Vector3 bl = localCenter + Vector3(-halfSize.x, -halfSize.y, 0.0f);
//    Vector3 br = localCenter + Vector3(halfSize.x, -halfSize.y, 0.0f);
//    Vector3 tr = localCenter + Vector3(halfSize.x, halfSize.y, 0.0f);
//    Vector3 tl = localCenter + Vector3(-halfSize.x, halfSize.y, 0.0f);
//
//    // 5) 각 코너를 월드 변환
//    bl = Vector3::Transform(bl, world);
//    br = Vector3::Transform(br, world);
//    tr = Vector3::Transform(tr, world);
//    tl = Vector3::Transform(tl, world);
//
//    // 6) 색상(충돌 여부) 처리
//    if (m_bIsColliding) {
//        if (m_vColor != Vector4(1.f, 0.f, 0.f, 1.f)) { // 0~1 일관
//            m_vOriginalColor = m_vColor;
//            m_vColor = Vector4(1.f, 0.f, 0.f, 1.f);
//        }
//    }
//    else {
//        if (m_vColor == Vector4(1.f, 0.f, 0.f, 1.f))
//            m_vColor = m_vOriginalColor;
//    }
//
//    // 7) 라인 렌더러로 전달 (축 섞기 금지, 그냥 변환된 코너들을 연결)
//    CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
//    if (!lineRenderer) lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
//
//    std::vector<Vector3> pts = { bl, br, tr, tl, bl };
//    lineRenderer->SetLineData(pts, m_vColor);
//
//    // (선택) AABB 저장이 필요하면 월드 코너들로 월드 AABB 갱신
//    m_vWorldBottomLeft = Vector3(
//        std::min(std::min(bl.x, br.x), std::min(tr.x, tl.x)),
//        std::min(std::min(bl.y, br.y), std::min(tr.y, tl.y)),
//        std::min(std::min(bl.z, br.z), std::min(tr.z, tl.z))
//    );
//    m_vWorldTopRight = Vector3(
//        std::max(std::max(bl.x, br.x), std::max(tr.x, tl.x)),
//        std::max(std::max(bl.y, br.y), std::max(tr.y, tl.y)),
//        std::max(std::max(bl.z, br.z), std::max(tr.z, tl.z))
//    );
//}
//
//void CCollider::LateUpdate()
//{
//}
//
//void CCollider::Render(const Matrix& view, const Matrix& projection)
//{
//    // 충돌체가 비활성화된 경우, 또는 렌더링이 비활성화된 경우 렌더링 중단
//    if (!m_bEnable || !g_bRenderColliders)
//        return;
//}
//
//void CCollider::OnCollisionEnter(CCollider* _pOther)
//{
//    CScript* sr = GetOwner()->GetComponent<CScript>();
//    if (sr)
//    {
//        sr->OnCollisionEnter(_pOther);
//    }
//
//    m_bIsColliding = true;
//}
//void CCollider::OnCollisionStay(CCollider* _pOther)
//{
//    CScript* sr = GetOwner()->GetComponent<CScript>();
//    if (sr)
//    {
//        sr->OnCollisionStay(_pOther);
//    }
//    m_bIsColliding = true; // 충돌 상태 업데이트
//}
//void CCollider::OnCollisionExit(CCollider* _pOther)
//{
//    m_bIsColliding = false;
//
//    CScript* sr = GetOwner()->GetComponent<CScript>();
//    if (sr)
//    {
//        sr->OnCollisionExit(_pOther);
//    }
//}
//
//void CCollider::RectCollider(const Vector2& scale, const Vector4& color, const Vector3& bottomLeft, const Vector3& topRight)
//{
//   CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
//    if (lineRenderer == nullptr)
//    {
//        lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
//    }
//
//    // 충돌 박스의 모서리 점들을 벡터로 구성
//    std::vector<Vector3> points = {
//        bottomLeft,                                     // 왼쪽 아래
//        Vector3(topRight.x, bottomLeft.y, bottomLeft.z), // 오른쪽 아래
//        topRight,                                       // 오른쪽 위
//        Vector3(bottomLeft.x, topRight.y, topRight.z),  // 왼쪽 위
//        bottomLeft                                      // 다시 처음으로 (닫힌 선)
//    };
//
//    // 라인 렌더러에 점 데이터 설정
//    lineRenderer->SetLineData(points, color);
//}
//
//void CCollider::RenderUI()
//{
//    ImGui::Begin("Collider Inspector");
//
//    // 현재 씬의 모든 게임 오브젝트를 가져온다.
//    CScene* currentScene = CSceneManager::GetCurScene();
//    if (!currentScene) {
//        ImGui::Text("No current scene found.");
//        ImGui::End();
//        return;
//    }
//
//    wstring sceneName = CSceneManager::GetCurrentSceneName();
//    ImGui::Text("Current Scene: %ls", sceneName.c_str());
//
//    // 모든 레이어에서 게임 오브젝트 수집
//    vector<CGameObject*> gameObjects;
//    for (int i = static_cast<int>(LAYER_TYPE::LT_None) + 1; i < static_cast<int>(LAYER_TYPE::LT_End); i++) {
//        vector<CGameObject*> layerObjects = currentScene->GetGameObjects(static_cast<LAYER_TYPE>(i));
//        gameObjects.insert(gameObjects.end(), layerObjects.begin(), layerObjects.end());
//    }
//
//    if (gameObjects.empty()) {
//        ImGui::Text("No game objects in the scene.");
//        ImGui::End();
//        return;
//    }
//
//    // 게임 오브젝트 선택 UI
//    static int selectedIndex = 0;
//    static vector<string> gameObjectNames;
//    static vector<const char*> gameObjectNamesCStr;
//
//    // 드롭다운 리스트 초기화
//    gameObjectNames.clear();
//    gameObjectNamesCStr.clear();
//
//    // 게임 오브젝트 이름 수집 및 변환
//    for (CGameObject* obj : gameObjects) {
//        if (!obj || obj->IsDead()) continue;
//
//        try {
//            wstring name = obj->GetName();
//            if (name.empty()) continue;
//
//            string str = WStringToString(name);
//            gameObjectNames.push_back(str);
//        }
//        catch (...) {
//            OutputDebugStringA("Exception occurred while accessing object name\n");
//            continue;
//        }
//    }
//
//    // 문자열 포인터 벡터 초기화
//    for (const string& name : gameObjectNames) {
//        gameObjectNamesCStr.push_back(name.c_str());
//    }
//
//    ImGui::Text("Objects found: %d", (int)gameObjectNames.size());
//
//    // 게임 오브젝트를 선택할 수 있는 드롭다운
//    if (!gameObjectNamesCStr.empty()) {
//        if (ImGui::Combo("Select Game Object", &selectedIndex, gameObjectNamesCStr.data(), (int)gameObjectNamesCStr.size())) {
//            ImGui::Text("Selected Index: %d", selectedIndex);
//        }
//    }
//    else {
//        ImGui::Text("No selectable game objects.");
//        ImGui::End();
//        return;
//    }
//
//    // 선택된 오브젝트 UI 출력
//    if (selectedIndex >= 0 && selectedIndex < gameObjects.size()) {
//        CGameObject* selectedObject = gameObjects[selectedIndex];
//        CCollider* selectedCollider = selectedObject->GetComponent<CCollider>();
//
//        // 선분 충돌체 체크 추가
//        CLineCollider* lineCollider = selectedObject->GetComponent<CLineCollider>();
//
//        ImGui::Separator();
//        ImGui::Text("Selected Object: %s", gameObjectNames[selectedIndex].c_str());
//
//        if (selectedCollider) {
//            // Collider 활성화 여부를 제어하는 체크박스
//            if (ImGui::Checkbox("Activate Collider", &selectedCollider->m_bIsColliderActive)) {
//                selectedCollider->SetColliderActive(selectedCollider->m_bIsColliderActive);
//            }
//
//            // 활성화되었을 때만 좌표 및 스케일 정보 표시
//            if (selectedCollider->m_bIsColliderActive) {
//                // 좌표 정보 추가
//                ImGui::Separator();
//                ImGui::Text("Coordinate Information");
//
//                // 오브젝트 로컬 좌표
//                ImGui::Text("Object");
//                Vector3 localPos = selectedObject->GetComponent<CTransform>()->GetLocalPosition();
//                ImGui::Text("Local Position: (%.2f, %.2f, %.2f)",
//                    localPos.x, localPos.y, localPos.z);
//
//                // 오브젝트 월드 좌표
//                Vector3 worldPos = selectedObject->GetComponent<CTransform>()->GetWorldPosition();
//                ImGui::Text("World Position: (%.2f, %.2f, %.2f)",
//                    worldPos.x, worldPos.y, worldPos.z);
//
//                // 선분 충돌체인 경우 다른 정보 표시
//                if (lineCollider) {
//                    ImGui::Separator();
//                    ImGui::Text("Line Collider");
//
//                    // 선분 정보 표시
//                    Vector2 start = lineCollider->m_vStart;
//                    Vector2 end = lineCollider->m_vEnd;
//                    ImGui::Text("Start Point: (%.2f, %.2f)", start.x, start.y);
//                    ImGui::Text("End Point: (%.2f, %.2f)", end.x, end.y);
//                    ImGui::Text("Thickness: %.2f", lineCollider->m_fThickness);
//
//                    // 세계 좌표계 충돌 박스 정보
//                    ImGui::Text("Collider World Min: (%.2f, %.2f, %.2f)",
//                        lineCollider->m_vWorldBottomLeft.x,
//                        lineCollider->m_vWorldBottomLeft.y,
//                        lineCollider->m_vWorldBottomLeft.z);
//                    ImGui::Text("Collider World Max: (%.2f, %.2f, %.2f)",
//                        lineCollider->m_vWorldTopRight.x,
//                        lineCollider->m_vWorldTopRight.y,
//                        lineCollider->m_vWorldTopRight.z);
//
//                    // 선분 편집 UI
//                    ImGui::Separator();
//                    ImGui::Text("Edit Line Collider");
//
//                    bool changed = false;
//                    changed |= ImGui::DragFloat2("Start Point", &start.x, 0.1f);
//                    changed |= ImGui::DragFloat2("End Point", &end.x, 0.1f);
//                    float thickness = lineCollider->m_fThickness;
//                    changed |= ImGui::SliderFloat("Thickness", &thickness, 0.01f, 1.0f);
//
//                    if (changed) {
//                        lineCollider->SetLine(start, end, thickness);
//
//                        // LineRenderer도 함께 업데이트
//                        CLineRenderer* lineRenderer = selectedObject->GetComponent<CLineRenderer>();
//                        if (lineRenderer) {
//                            std::vector<Vector3> linePoints = {
//                                Vector3(start.x, start.y, 0.0f),
//                                Vector3(end.x, end.y, 0.0f)
//                            };
//                            Vector4 lineColor = selectedCollider->m_vColor;
//                            lineRenderer->SetLineData(linePoints, lineColor);
//                        }
//                    }
//
//                    // 색상 편집
//                    ImGui::Text("Line Color");
//                    if (ImGui::ColorEdit4("Color", &selectedCollider->m_vColor.x)) {
//                        // 라인 렌더러 색상 업데이트
//                        CLineRenderer* lineRenderer = selectedObject->GetComponent<CLineRenderer>();
//                        if (lineRenderer) {
//                            std::vector<Vector3> linePoints = {
//                                Vector3(start.x, start.y, 0.0f),
//                                Vector3(end.x, end.y, 0.0f)
//                            };
//                            lineRenderer->SetLineData(linePoints, selectedCollider->m_vColor);
//                        }
//                    }
//                }
//                // 일반 충돌체인 경우 기존 UI 표시
//                else {
//                    // 충돌체 로컬 좌표
//                    ImGui::Separator();
//                    ImGui::Text("Collider");
//                    ImGui::Text("Collider Local Min: (%.2f, %.2f, %.2f)",
//                        selectedCollider->m_vBottomLeft.x,
//                        selectedCollider->m_vBottomLeft.y,
//                        selectedCollider->m_vBottomLeft.z);
//                    ImGui::Text("Collider Local Max: (%.2f, %.2f, %.2f)",
//                        selectedCollider->m_vTopRight.x,
//                        selectedCollider->m_vTopRight.y,
//                        selectedCollider->m_vTopRight.z);
//
//                    // 충돌체 월드 좌표 계산
//                    Vector3 worldBottomLeft = selectedCollider->m_vBottomLeft + worldPos;
//                    Vector3 worldTopRight = selectedCollider->m_vTopRight + worldPos;
//                    ImGui::Text("Collider World Min: (%.2f, %.2f, %.2f)",
//                        worldBottomLeft.x,
//                        worldBottomLeft.y,
//                        worldBottomLeft.z);
//                    ImGui::Text("Collider World Max: (%.2f, %.2f, %.2f)",
//                        worldTopRight.x,
//                        worldTopRight.y,
//                        worldTopRight.z);
//
//                    // 기존 좌표 정보 출력 코드 다음에 추가
//                    ImGui::Separator();
//                    ImGui::Text("Size Information");
//
//                    // 오브젝트의 Transform 크기
//                    Vector3 objectScale = selectedObject->GetComponent<CTransform>()->GetLocalScale();
//                    ImGui::Text("Object Scale: (%.2f, %.2f, %.2f)",
//                        objectScale.x, objectScale.y, objectScale.z);
//
//                    // 충돌체 로컬 크기 계산
//                    Vector3 colliderLocalSize = selectedCollider->m_vTopRight - selectedCollider->m_vBottomLeft;
//                    ImGui::Text("Collider Local Size: (%.2f, %.2f, %.2f)",
//                        colliderLocalSize.x, colliderLocalSize.y, colliderLocalSize.z);
//
//                    // 충돌체 월드 크기 계산 (오브젝트 스케일 고려)
//                    Vector3 colliderWorldSize = Vector3(
//                        colliderLocalSize.x * objectScale.x,
//                        colliderLocalSize.y * objectScale.y,
//                        colliderLocalSize.z * objectScale.z
//                    );
//                    ImGui::Text("Collider World Size: (%.2f, %.2f, %.2f)",
//                        colliderWorldSize.x, colliderWorldSize.y, colliderWorldSize.z);
//
//                    // Collider 스케일 및 색상 편집 UI
//                    if (selectedCollider->m_bEnable) {
//                        ImGui::Separator();
//                        ImGui::Text("Collider Scale");
//                        bool scaleChanged = false;
//                        scaleChanged |= ImGui::SliderFloat("Scale X", &selectedCollider->m_vScale.x, 0.1f, 50.0f);
//                        scaleChanged |= ImGui::SliderFloat("Scale Y", &selectedCollider->m_vScale.y, 0.1f, 50.0f);
//
//                        ImGui::Text("Collider Color");
//                        bool colorChanged = ImGui::ColorEdit4("Color", &selectedCollider->m_vColor.x);
//
//                        // 스케일 또는 색상 변경 시 처리
//                        if (scaleChanged || colorChanged) {
//                            selectedCollider->m_vBottomLeft = Vector3(-0.5f * selectedCollider->m_vScale.x,
//                                -0.5f * selectedCollider->m_vScale.y,
//                                -2.0f);
//                            selectedCollider->m_vTopRight = Vector3(0.5f * selectedCollider->m_vScale.x,
//                                0.5f * selectedCollider->m_vScale.y,
//                                -2.0f);
//
//                            selectedCollider->RectCollider(
//                                selectedCollider->m_vScale,
//                                selectedCollider->m_vColor,
//                                selectedCollider->m_vBottomLeft,
//                                selectedCollider->m_vTopRight
//                            );
//                        }
//                    }
//                }
//            }
//        }
//        else {
//            ImGui::Text("No Collider found in this object.");
//        }
//    }
//    else {
//        ImGui::Text("Invalid object selection.");
//    }
//
//    ImGui::End();
//}
//
//string CCollider::WStringToString(const wstring& wstr)
//{
//	/*if (wstr.empty()) return "";
//	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
//	string result(sizeNeeded, 0);
//	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], sizeNeeded, NULL, NULL);
//	return result;*/
//    if (wstr.empty()) return "";
//
//    try {
//        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
//        if (sizeNeeded <= 0) return "";
//
//        string result(sizeNeeded, 0);
//        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], sizeNeeded, NULL, NULL);
//        return result;
//    }
//    catch (...) {
//        return "[Invalid wstring]";
//    }
//}

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

// 2025-07-02 CCollider.cpp 코드 정리
//void CCollider::Update()
//{
//    // 부모 Transform이 있다면 월드 좌표 계산
//    if (m_pTransform && m_pTransform->GetParent())
//    {
//        Matrix parentWorldMatrix = m_pTransform->GetParent()->GetWorldMatrix();
//        // 로컬 충돌 박스 좌표를 월드 좌표로 변환
//        Vector3 worldBottomLeft = Vector3::Transform(m_vBottomLeft, parentWorldMatrix);
//        Vector3 worldTopRight = Vector3::Transform(m_vTopRight, parentWorldMatrix);
//        // 변환된 좌표로 충돌 검사 및 렌더링
//        RectCollider(m_vScale, m_vColor, worldBottomLeft, worldTopRight);
//
//    }
//    else
//    {
//        Vector3 worldBottomLeft = m_vBottomLeft;
//        Vector3 worldTopRight = m_vTopRight;
//        /// 충돌 상태에 따른 색상 변경
//        if (m_bIsColliding)
//        {
//            if (m_vColor != Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
//                // 충돌 전 색상이 저장되지 않았다면 저장
//                if (m_vColor != Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
//                    m_vOriginalColor = m_vColor;
//                }
//                m_vColor = Vector4(255.0f, 0.0f, 0.0f, 1.0f); // 충돌 시 빨간색
//            }
//        }
//        else // 충돌 상태가 아닐 때
//        {
//            // 충돌이 해제되면 원래 색상으로 복원
//            if (m_vColor == Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
//                m_vColor = m_vOriginalColor;
//            }
//        }
//        RectCollider(m_vScale, m_vColor, m_vBottomLeft, m_vTopRight);
//    }
//    /*if (!m_pTransform)
//        return;
//
//    // 월드 좌표 계산
//    Matrix worldMatrix = m_pTransform->GetWorldMatrix();
//
//    // 로컬 충돌 박스 좌표를 월드 좌표로 변환
//    Vector3 worldBottomLeft = Vector3::Transform(m_vBottomLeft, worldMatrix);
//    Vector3 worldTopRight = Vector3::Transform(m_vTopRight, worldMatrix);
//
//    // 충돌 상태에 따른 색상 변경
//    if (m_bIsColliding)
//    {
//        if (m_vColor != Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
//            m_vOriginalColor = m_vColor;
//            m_vColor = Vector4(255.0f, 0.0f, 0.0f, 1.0f); // 충돌 시 빨간색
//        }
//    }
//    else
//    {
//        if (m_vColor == Vector4(255.0f, 0.0f, 0.0f, 1.0f)) {
//            m_vColor = m_vOriginalColor;
//        }
//    }
//
//    // 월드 좌표로 충돌 박스 시각화
//    RectCollider(m_vScale, m_vColor, worldBottomLeft, worldTopRight);*/
//}
//
//void CCollider::LateUpdate()
//{
//}
//
//void CCollider::Render(const Matrix& view, const Matrix& projection)
//{
//    // 충돌체가 비활성화된 경우, 또는 렌더링이 비활성화된 경우 렌더링 중단
//    if (!m_bEnable || !g_bRenderColliders)
//        return;
//}
//
//void CCollider::OnCollisionEnter(CCollider* _pOther)
//{
//    CScript* sr = GetOwner()->GetComponent<CScript>();
//    if (sr)
//    {
//        sr->OnCollisionEnter(_pOther);
//    }
//
//    //// 충돌한 오브젝트가 배경이라면 중력 해제
//    //if (_pOther->GetOwner()->GetName() == L"BackGround")
//    //{
//    //    CGravity* gravity = GetOwner()->GetComponent<CGravity>();
//    //    if (gravity)
//    //    {
//    //        gravity->SetGround(true);
//    //    }
//    //}
//
//    m_bIsColliding = true;
//}
//void CCollider::OnCollisionStay(CCollider* _pOther)
//{
//    CScript* sr = GetOwner()->GetComponent<CScript>();
//    if (sr)
//    {
//        sr->OnCollisionStay(_pOther);
//    }
//    m_bIsColliding = true; // 충돌 상태 업데이트
//}
//void CCollider::OnCollisionExit(CCollider* _pOther)
//{
//    m_bIsColliding = false;
//
//    // 배경에서 벗어나면 중력 다시 적용
//    /*if (_pOther->GetOwner()->GetName() == L"BackGround")
//    {
//        CGravity* gravity = GetOwner()->GetComponent<CGravity>();
//        if (gravity)
//        {
//            gravity->SetGround(false);
//        }
//    }*/
//
//    CScript* sr = GetOwner()->GetComponent<CScript>();
//    if (sr)
//    {
//        sr->OnCollisionExit(_pOther);
//    }
//}
//
//void CCollider::RectCollider(const Vector2& scale, const Vector4& color, const Vector3& bottomLeft, const Vector3& topRight)
//{
//    /*// 지역 변수로 사용하여 멤버 변수 오염 방지
//    Vector3 localBottomLeft = Vector3(-0.5f * scale.x, -0.5f * scale.y, -1.0f);
//    Vector3 localTopRight = Vector3(0.5f * scale.x, 0.5f * scale.y, -1.0f);
//
//    CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
//    if (lineRenderer == nullptr)
//    {
//        lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
//        lineRenderer->SetFollowParent(true);  // 충돌박스는 부모를 따라다니도록 설정
//    }
//
//    // 지역 변수를 사용하여 라인 렌더러 업데이트
//    lineRenderer->SetShape(localBottomLeft, localTopRight, color);*/
//    CLineRenderer* lineRenderer = GetOwner()->GetComponent<CLineRenderer>();
//    if (lineRenderer == nullptr)
//    {
//        lineRenderer = GetOwner()->AddComponent<CLineRenderer>();
//        /*lineRenderer->SetFollowParent(true);
//        lineRenderer->Init();*/
//    }
//
//    // 충돌 박스의 모서리 점들을 벡터로 구성
//    std::vector<Vector3> points = {
//        bottomLeft,                                     // 왼쪽 아래
//        Vector3(topRight.x, bottomLeft.y, bottomLeft.z), // 오른쪽 아래
//        topRight,                                       // 오른쪽 위
//        Vector3(bottomLeft.x, topRight.y, topRight.z),  // 왼쪽 위
//        bottomLeft                                      // 다시 처음으로 (닫힌 선)
//    };
//
//    // 라인 렌더러에 점 데이터 설정
//    lineRenderer->SetLineData(points, color);
//}

// 2025-08-16 Collider::Update 수정전
/*void CCollider::Update()
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
        auto collObj = m_pTransform->GetOwner();
        //OutputDebugStringW((L"[디버그] Collider 오브젝트: " + collObj->GetName() + L"\n").c_str());

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
}*/