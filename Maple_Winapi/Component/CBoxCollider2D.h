#pragma once
#include "CCollider.h"

class CBoxCollider2D : public CCollider
{
public:
	CBoxCollider2D();
	~CBoxCollider2D();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

private:
	UINT			m_iCol;
};