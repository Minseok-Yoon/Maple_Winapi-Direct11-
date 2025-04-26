#pragma once
#include "CCollider.h"

using namespace math;

class CBoxCollider2D : public CCollider
{
public:
	CBoxCollider2D();
	~CBoxCollider2D();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(const Matrix& view, const Matrix& projection);

private:
	UINT			m_iCol;
};