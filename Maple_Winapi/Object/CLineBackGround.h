#pragma once
#include "CGameObject.h"

class CLineBackGround : public CGameObject
{
public:
    CLineBackGround();
    virtual ~CLineBackGround();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;

    // 선분 생성 메서드
    void CreateLine(const Vector3& start, const Vector3& end, const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    void CreateHorizontalLine(float y, float startX, float endX, const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    void CreateVerticalLine(float x, float startY, float endY, const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

    // 그리드 생성 메서드
    void CreateGrid(float left, float right, float bottom, float top, float spacing, const Vector4& color = Vector4(0.5f, 0.5f, 0.5f, 1.0f));

private:
   vector<CGameObject*> m_vLineObjects;
};