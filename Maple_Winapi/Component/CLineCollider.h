#pragma once

#include "CCollider.h"
class CLineCollider :
    public CCollider
{
public:
    CLineCollider();
    virtual ~CLineCollider();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;

    void SetLine(const Vector2& start, const Vector2& end, float thickness = 0.1f);

    // �浹 �˻�
    bool CheckCollision(CCollider* other);
    bool LineToBoxCollision(const Vector2& boxMin, const Vector2& boxMax);
    //bool LineToLineCollision(const Vector2& otherStart, const Vector2& otherEnd);

    // Getter
    Vector2 GetStart() const { return m_vStart; }
    Vector2 GetEnd() const { return m_vEnd; }
    float GetThickness() const { return m_fThickness; }

public:
    bool    m_bNeedVisualUpdate;
    Vector2 m_vStart;       // ���� ������
    Vector2 m_vEnd;         // ���� ����
    float m_fThickness;     // ���� �β�
};