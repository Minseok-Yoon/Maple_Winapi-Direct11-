#pragma once
#include "CComponent.h"

class CUITransform : public CComponent
{
public:
	CUITransform();
	virtual ~CUITransform();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

    void UIBind(const Matrix& view, const Matrix& projection) const;

    void UpdateTransform();

    // 설정 함수
    void SetPosition(const Vector2& pos) { m_Position = pos; UpdateTransform(); }
    void SetSize(const Vector2& size) { m_Size = size; UpdateTransform(); }
    void SetScale(const Vector2& scale) { m_Scale = scale; UpdateTransform(); }
    void SetRotation(float degree) { m_Rotation = degree; UpdateTransform(); }
    void SetPivot(const Vector2& pivot) { m_Pivot = pivot; UpdateTransform(); }

    // 변환 관련
    Matrix GetLocalMatrix() const { return m_LocalMatrix; }
    Matrix GetWorldMatrix() const { return m_WorldMatrix; }
    Vector2 GetWorldPosition() const
    {
        return Vector2(
            m_WorldMatrix.m[3][0],
            m_WorldMatrix.m[3][1]
        );
    }
    Vector2 GetSize() const { return m_Size; }
    Vector2 GetPivot() const { return m_Pivot; }

    void SetParent(CUITransform* parent)
    {
        m_Parent = parent;
        if (parent)
            parent->AddChild(this);
    }

    void AddChild(CUITransform* child) { m_Children.push_back(child); }


private:
    Vector2 m_Position = Vector2::Zero;  // 로컬 위치 (부모 기준)
    Vector2 m_Size = Vector2(100, 100);  // UI 크기
    Vector2 m_Scale = Vector2::One;
    float   m_Rotation = 0.f;            // Degree
    Vector2 m_Pivot = Vector2(0.5f, 0.5f); // 0~1 (중앙)

    CUITransform* m_Parent = nullptr;
    std::vector<CUITransform*> m_Children;

    Matrix m_LocalMatrix = Matrix::Identity;
    Matrix m_WorldMatrix = Matrix::Identity;
};