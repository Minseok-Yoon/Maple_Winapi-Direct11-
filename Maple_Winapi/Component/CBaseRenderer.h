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

private:
    CMaterial* m_pMaterial;
    CMesh* m_pMesh;
};