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

// mesh�� material�� �̿��� ȭ�鿡 ����ϴ� ������ ������
// �ؽ�Ʈ ������ ���, �θ� ������ ����, ���̴� ���ҽ� ����, ��ǥ ��ȯ ����� ����