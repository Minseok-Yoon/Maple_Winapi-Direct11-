#pragma once
#include "CGraphicsDevice_DX11.h"

class CVertexBuffer : public GpuBuffer
{
public:
	CVertexBuffer();
	virtual ~CVertexBuffer();

	bool Create(const vector<Vertex>& _vecVertexes);
	void Bind();
};

// 정점을 담고 있는 buffer 클래스이다.
// Dx11에서 Polygon을 그리기 위해서는 primitive data가 필요하다.