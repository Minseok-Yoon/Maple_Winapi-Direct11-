#pragma once
#include "CGraphicsDevice_DX11.h"

class CIndexBuffer : public GpuBuffer
{
public:
	CIndexBuffer();
	virtual ~CIndexBuffer();

	bool Create(const vector<UINT>& _vecIndices);
	void Bind() const;

	UINT GetIndexCount() const { return m_iIndexCount; }

private:
	UINT m_iIndexCount;
};