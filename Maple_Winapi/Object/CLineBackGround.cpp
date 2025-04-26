#include "CLineBackGround.h"
#include "../Component/CLineRenderer.h"

CLineBackGround::CLineBackGround()
{
}

CLineBackGround::~CLineBackGround()
{
    // 생성한 모든 라인 오브젝트 정리
    for (auto* lineObj : m_vLineObjects)
    {
        if (lineObj)
            delete lineObj;
    }
    m_vLineObjects.clear();
}

void CLineBackGround::Init()
{
    CGameObject::Init();
}

void CLineBackGround::Update()
{
    CGameObject::Update();

    // 모든 라인 오브젝트 업데이트
    for (auto* lineObj : m_vLineObjects)
    {
        if (lineObj)
            lineObj->Update();
    }
}

void CLineBackGround::LateUpdate()
{
    CGameObject::LateUpdate();

    // 모든 라인 오브젝트 후기 업데이트
    for (auto* lineObj : m_vLineObjects)
    {
        if (lineObj)
            lineObj->LateUpdate();
    }
}

void CLineBackGround::Render(const Matrix& view, const Matrix& projection)
{
    CGameObject::Render(view, projection);

    // 모든 라인 오브젝트 렌더링
    for (auto* lineObj : m_vLineObjects)
    {
        if (lineObj)
            lineObj->Render(view, projection);
    }
}

void CLineBackGround::CreateLine(const Vector3& start, const Vector3& end, const Vector4& color)
{
    // 라인 오브젝트 생성
    CGameObject* lineObj = new CGameObject();

    // 라인 렌더러 추가
    CLineRenderer* lineRenderer = lineObj->AddComponent<CLineRenderer>();

    // 라인 데이터 설정
    std::vector<Vector3> points = { start, end };
    lineRenderer->SetLineData(points, color);

    // 초기화
    lineObj->Init();
    lineRenderer->Init();

    // 라인 오브젝트 저장
    m_vLineObjects.push_back(lineObj);
}

void CLineBackGround::CreateHorizontalLine(float y, float startX, float endX, const Vector4& color)
{
    Vector3 start(startX, y, 0.0f);
    Vector3 end(endX, y, 0.0f);
    CreateLine(start, end, color);
}

void CLineBackGround::CreateVerticalLine(float x, float startY, float endY, const Vector4& color)
{
    Vector3 start(x, startY, 0.0f);
    Vector3 end(x, endY, 0.0f);
    CreateLine(start, end, color);
}

void CLineBackGround::CreateGrid(float left, float right, float bottom, float top, float spacing, const Vector4& color)
{
    // 수평선 생성
    for (float y = bottom; y <= top; y += spacing)
    {
        CreateHorizontalLine(y, left, right, color);
    }

    // 수직선 생성
    for (float x = left; x <= right; x += spacing)
    {
        CreateVerticalLine(x, bottom, top, color);
    }
}
