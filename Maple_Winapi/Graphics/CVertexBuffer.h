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

// ������ ��� �ִ� buffer Ŭ�����̴�.
// Dx11���� Polygon�� �׸��� ���ؼ��� primitive data�� �ʿ��ϴ�.