#include "CLineBackGround.h"
#include "../Component/CLineRenderer.h"

CLineBackGround::CLineBackGround()
{
}

CLineBackGround::~CLineBackGround()
{
    // ������ ��� ���� ������Ʈ ����
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

    // ��� ���� ������Ʈ ������Ʈ
    for (auto* lineObj : m_vLineObjects)
    {
        if (lineObj)
            lineObj->Update();
    }
}

void CLineBackGround::LateUpdate()
{
    CGameObject::LateUpdate();

    // ��� ���� ������Ʈ �ı� ������Ʈ
    for (auto* lineObj : m_vLineObjects)
    {
        if (lineObj)
            lineObj->LateUpdate();
    }
}

void CLineBackGround::Render(const Matrix& view, const Matrix& projection)
{
    CGameObject::Render(view, projection);

    // ��� ���� ������Ʈ ������
    for (auto* lineObj : m_vLineObjects)
    {
        if (lineObj)
            lineObj->Render(view, projection);
    }
}

void CLineBackGround::CreateLine(const Vector3& start, const Vector3& end, const Vector4& color)
{
    // ���� ������Ʈ ����
    CGameObject* lineObj = new CGameObject();

    // ���� ������ �߰�
    CLineRenderer* lineRenderer = lineObj->AddComponent<CLineRenderer>();

    // ���� ������ ����
    std::vector<Vector3> points = { start, end };
    lineRenderer->SetLineData(points, color);

    // �ʱ�ȭ
    lineObj->Init();
    lineRenderer->Init();

    // ���� ������Ʈ ����
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
    // ���� ����
    for (float y = bottom; y <= top; y += spacing)
    {
        CreateHorizontalLine(y, left, right, color);
    }

    // ������ ����
    for (float x = left; x <= right; x += spacing)
    {
        CreateVerticalLine(x, bottom, top, color);
    }
}
