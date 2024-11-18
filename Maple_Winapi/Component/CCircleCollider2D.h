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
	virtual void Render();

private:
	float	m_fRadius;
};