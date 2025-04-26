#pragma once
#include "CComponent.h"
#include "../Resource/CTexture.h"
#include "../Resource/CMaterial.h"
#include "../Resource/CMesh.h"
#include "../Component/CBaseRenderer.h"

class CSpriteRenderer : public CBaseRenderer
{
public:
	CSpriteRenderer();
	virtual ~CSpriteRenderer();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	void SetTexture(CTexture* _pTexture) { m_pTexture = _pTexture; }

private:
	CTexture*	m_pTexture;
};

// mesh와 material을 이용해 화면에 출력하는 역할을 하지만
// 텍스트 렌더링 기능, 부모 렌더러 연결, 셰이더 리소스 관리, 좌표 변환 기능은 없음