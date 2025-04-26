#include "CLineCollider.h"
#include "CLineRenderer.h"

CLineCollider::CLineCollider() :
	m_fThickness(0.1f)
{
}

CLineCollider::~CLineCollider()
{
}

void CLineCollider::Init()
{
	CCollider::Init();
}

void CLineCollider::Update()
{
}

void CLineCollider::LateUpdate()
{
	CCollider::LateUpdate();
}

void CLineCollider::Render(const Matrix& view, const Matrix& projection)
{
}

void CLineCollider::SetLine(const Vector2& start, const Vector2& end, float thickness)
{
    m_vStart = start;
    m_vEnd = end;
    m_fThickness = thickness;

    // 선의 경계 상자 설정 (CCollider의 m_vBottomLeft와 m_vTopRight 업데이트)
    Vector2 minPos, maxPos;
    minPos.x = min(m_vStart.x, m_vEnd.x) - m_fThickness;
    minPos.y = min(m_vStart.y, m_vEnd.y) - m_fThickness;

    maxPos.x = max(m_vStart.x, m_vEnd.x) + m_fThickness;
    maxPos.y = max(m_vStart.y, m_vEnd.y) + m_fThickness;

    // 로컬 좌표 대신 월드 좌표를 직접 설정
    m_vWorldBottomLeft = Vector3(minPos.x, minPos.y, 0.0f);
    m_vWorldTopRight = Vector3(maxPos.x, maxPos.y, 0.0f);

    // 로컬 좌표도 설정 (필요할 경우)
    m_vBottomLeft = Vector3(minPos.x, minPos.y, 0.0f);
    m_vTopRight = Vector3(maxPos.x, maxPos.y, 0.0f);
}

bool CLineCollider::CheckCollision(CCollider* other)
{
    // 월드 좌표로 변환된 박스의 경계를 사용
    if (other->GetColliderType() == COLLIDER_TYPE::CT_Rect2D)
    {
        Vector2 boxMin(other->GetWorldBottomLeft().x, other->GetWorldBottomLeft().y);
        Vector2 boxMax(other->GetWorldTopRight().x, other->GetWorldTopRight().y);
        return LineToBoxCollision(boxMin, boxMax);
    }
    return false;
}

bool CLineCollider::LineToBoxCollision(const Vector2& boxMin, const Vector2& boxMax)
{
    // 선분과 박스 충돌 검사
    Vector2 dir = m_vEnd - m_vStart;
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    Vector2 dirNorm = (length > 0) ? dir / length : Vector2(0, 0);
    Vector2 perpNorm = Vector2(-dirNorm.y, dirNorm.x); // 수직 벡터

    // 두께를 적용한 선분의 네 꼭짓점 계산
    Vector2 halfThick = perpNorm * (m_fThickness * 0.5f);
    Vector2 corners[4] = {
        m_vStart + halfThick,
        m_vStart - halfThick,
        m_vEnd + halfThick,
        m_vEnd - halfThick
    };

    // 선분의 네 꼭짓점 중 하나라도 박스 내부에 있는지 확인
    for (int i = 0; i < 4; ++i) {
        if (corners[i].x >= boxMin.x && corners[i].x <= boxMax.x &&
            corners[i].y >= boxMin.y && corners[i].y <= boxMax.y) {
            return true;
        }
    }

    // 선분과 박스의 경계선 교차 검사를 수행
    float t;
    Vector2 intersect;

    // x축 경계선에 대한 교차 검사 (박스 왼쪽과 오른쪽 경계)
    if (dirNorm.x != 0)
    {
        // 왼쪽 경계와의 교차
        t = (boxMin.x - m_vStart.x) / dirNorm.x;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.y >= boxMin.y && intersect.y <= boxMax.y)
            {
                if (fabs(intersect.x - boxMin.x) < 0.01f) // 오차 범위 설정 (예: 0.01f)
                {
                    return true;
                }
            }
        }
        // 오른쪽 경계와의 교차
        t = (boxMax.x - m_vStart.x) / dirNorm.x;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.y >= boxMin.y && intersect.y <= boxMax.y)
            {
                if (fabs(intersect.x - boxMax.x) < 0.01f) // 오차 범위 설정 (예: 0.01f)
                {
                    return true;
                }
            }
        }
    }

    // y축 경계선에 대한 교차 검사 (박스 위쪽과 아래쪽 경계)
    if (dirNorm.y != 0)
    {
        // 아래쪽 경계와의 교차
        t = (boxMin.y - m_vStart.y) / dirNorm.y;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.x >= boxMin.x && intersect.x <= boxMax.x)
            {
                if (fabs(intersect.y - boxMin.y) < 0.01f) // 오차 범위 설정 (예: 0.01f)
                {
                    return true;
                }
            }
        }
        // 위쪽 경계와의 교차
        t = (boxMax.y - m_vStart.y) / dirNorm.y;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.x >= boxMin.x && intersect.x <= boxMax.x)
            {
                if (fabs(intersect.y - boxMax.y) < 0.01f) // 오차 범위 설정 (예: 0.01f)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

//// 라인 충돌체와의 충돌 검사
    //else if (other->GetComponentType() == COMPONENT_TYPE::CT_LineRenderer)
    //{
    //	CLineCollider* lineCollider = static_cast<CLineCollider*>(other);
    //	return LineToLineCollision(lineCollider->GetStart(), lineCollider->GetEnd());
    //}

//bool CLineCollider::LineToLineCollision(const Vector3& otherStart, const Vector3& otherEnd)
//{
//    // 두 선분 간의 최소 거리 계산
//    // 임계값(m_fThickness + 다른 선의 두께) 보다 작으면 충돌로 판정
//
//    // 간단한 구현: 2D에서만 작동하는 알고리즘
//    float x1 = m_vStart.x;
//    float y1 = m_vStart.y;
//    float x2 = m_vEnd.x;
//    float y2 = m_vEnd.y;
//
//    float x3 = otherStart.x;
//    float y3 = otherStart.y;
//    float x4 = otherEnd.x;
//    float y4 = otherEnd.y;
//
//    // 선분 교차 검사 공식
//    float den = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
//    if (den == 0)
//        return false; // 평행
//
//    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / den;
//    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / den;
//
//    // 선분 내에서 교차하는지 확인
//    return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
//}
