#pragma once
#include "../Entity/CEntity.h"
#include "CComponent.h"
#include "../Resource/CTexture.h"
#include "../Resource/CMaterial.h"
#include "../Resource/CMesh.h"

class CSpriteRenderer : public CComponent
{
public:
	CSpriteRenderer();
	~CSpriteRenderer();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void SetTexture(CTexture* _pTexture) { m_pTexture = _pTexture; }
	void SetMaterial(CMaterial* _pMaterial) { m_pMaterial = _pMaterial; }
	

private:
	CTexture*	m_pTexture;
	CMaterial*	m_pMaterial;
	CMesh*		m_pMesh;
};
