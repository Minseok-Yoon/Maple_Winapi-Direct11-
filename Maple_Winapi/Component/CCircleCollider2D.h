#pragma once
#include "CCollider.h"

class CCircleCollider2D : public CCollider
{
public:
	CCircleCollider2D();
	~CCircleCollider2D();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(const Matrix& view, const Matrix& projection);

private:
	float	m_fRadius;
};