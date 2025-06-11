#pragma once
#include "CComponent.h"
#include "../Resource/CMaterial.h"
#include "../Resource/CMesh.h"

class CBaseRenderer : public CComponent
{
public:
    CBaseRenderer(COMPONENT_TYPE type);
    virtual ~CBaseRenderer();

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render(const Matrix& view, const Matrix& projection) override;

    void Draw();

    const CMaterial* GetMaterial() const { return m_pMaterial; }
    void SetMaterial(CMaterial* material) { m_pMaterial = material; }

    const CMesh* GetMesh() const { return m_pMesh; }
    void SetMesh(CMesh* mesh) { m_pMesh = mesh; }

    void SetPivotType(const Vector2& _vPivot) { m_vPivot = _vPivot; }
    Vector2 GetPivotType() const { return m_vPivot; }

private:
    CMaterial* m_pMaterial;
    CMesh* m_pMesh;
    Vector2 m_vPivot = { 0.5f, 0.5f };
};