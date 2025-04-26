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

    // ���� ��� ���� ���� (CCollider�� m_vBottomLeft�� m_vTopRight ������Ʈ)
    Vector2 minPos, maxPos;
    minPos.x = min(m_vStart.x, m_vEnd.x) - m_fThickness;
    minPos.y = min(m_vStart.y, m_vEnd.y) - m_fThickness;

    maxPos.x = max(m_vStart.x, m_vEnd.x) + m_fThickness;
    maxPos.y = max(m_vStart.y, m_vEnd.y) + m_fThickness;

    // ���� ��ǥ ��� ���� ��ǥ�� ���� ����
    m_vWorldBottomLeft = Vector3(minPos.x, minPos.y, 0.0f);
    m_vWorldTopRight = Vector3(maxPos.x, maxPos.y, 0.0f);

    // ���� ��ǥ�� ���� (�ʿ��� ���)
    m_vBottomLeft = Vector3(minPos.x, minPos.y, 0.0f);
    m_vTopRight = Vector3(maxPos.x, maxPos.y, 0.0f);
}

bool CLineCollider::CheckCollision(CCollider* other)
{
    // ���� ��ǥ�� ��ȯ�� �ڽ��� ��踦 ���
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
    // ���а� �ڽ� �浹 �˻�
    Vector2 dir = m_vEnd - m_vStart;
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    Vector2 dirNorm = (length > 0) ? dir / length : Vector2(0, 0);
    Vector2 perpNorm = Vector2(-dirNorm.y, dirNorm.x); // ���� ����

    // �β��� ������ ������ �� ������ ���
    Vector2 halfThick = perpNorm * (m_fThickness * 0.5f);
    Vector2 corners[4] = {
        m_vStart + halfThick,
        m_vStart - halfThick,
        m_vEnd + halfThick,
        m_vEnd - halfThick
    };

    // ������ �� ������ �� �ϳ��� �ڽ� ���ο� �ִ��� Ȯ��
    for (int i = 0; i < 4; ++i) {
        if (corners[i].x >= boxMin.x && corners[i].x <= boxMax.x &&
            corners[i].y >= boxMin.y && corners[i].y <= boxMax.y) {
            return true;
        }
    }

    // ���а� �ڽ��� ��輱 ���� �˻縦 ����
    float t;
    Vector2 intersect;

    // x�� ��輱�� ���� ���� �˻� (�ڽ� ���ʰ� ������ ���)
    if (dirNorm.x != 0)
    {
        // ���� ������ ����
        t = (boxMin.x - m_vStart.x) / dirNorm.x;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.y >= boxMin.y && intersect.y <= boxMax.y)
            {
                if (fabs(intersect.x - boxMin.x) < 0.01f) // ���� ���� ���� (��: 0.01f)
                {
                    return true;
                }
            }
        }
        // ������ ������ ����
        t = (boxMax.x - m_vStart.x) / dirNorm.x;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.y >= boxMin.y && intersect.y <= boxMax.y)
            {
                if (fabs(intersect.x - boxMax.x) < 0.01f) // ���� ���� ���� (��: 0.01f)
                {
                    return true;
                }
            }
        }
    }

    // y�� ��輱�� ���� ���� �˻� (�ڽ� ���ʰ� �Ʒ��� ���)
    if (dirNorm.y != 0)
    {
        // �Ʒ��� ������ ����
        t = (boxMin.y - m_vStart.y) / dirNorm.y;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.x >= boxMin.x && intersect.x <= boxMax.x)
            {
                if (fabs(intersect.y - boxMin.y) < 0.01f) // ���� ���� ���� (��: 0.01f)
                {
                    return true;
                }
            }
        }
        // ���� ������ ����
        t = (boxMax.y - m_vStart.y) / dirNorm.y;
        if (t >= 0 && t <= length)
        {
            intersect = m_vStart + dirNorm * t;
            if (intersect.x >= boxMin.x && intersect.x <= boxMax.x)
            {
                if (fabs(intersect.y - boxMax.y) < 0.01f) // ���� ���� ���� (��: 0.01f)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

//// ���� �浹ü���� �浹 �˻�
    //else if (other->GetComponentType() == COMPONENT_TYPE::CT_LineRenderer)
    //{
    //	CLineCollider* lineCollider = static_cast<CLineCollider*>(other);
    //	return LineToLineCollision(lineCollider->GetStart(), lineCollider->GetEnd());
    //}

//bool CLineCollider::LineToLineCollision(const Vector3& otherStart, const Vector3& otherEnd)
//{
//    // �� ���� ���� �ּ� �Ÿ� ���
//    // �Ӱ谪(m_fThickness + �ٸ� ���� �β�) ���� ������ �浹�� ����
//
//    // ������ ����: 2D������ �۵��ϴ� �˰���
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
//    // ���� ���� �˻� ����
//    float den = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
//    if (den == 0)
//        return false; // ����
//
//    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / den;
//    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / den;
//
//    // ���� ������ �����ϴ��� Ȯ��
//    return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
//}
