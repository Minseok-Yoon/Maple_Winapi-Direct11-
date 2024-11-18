#pragma once
#include "CGraphicsDevice_DX11.h"

class CVertexBuffer : public GpuBuffer
{
public:
	CVertexBuffer();
	~CVertexBuffer();

	bool Create(const vector<Vertex>& _vecVertexes);
	void Bind();
	//void BindVertexBuffer(UINT _iStartSlot, UINT _iNumBuffers, ID3D11Buffer* 
	//	const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets);	// 정점 버퍼를 GPU에 바인딩

private:
	/*UINT offset = 0;
	UINT vectexSize = sizeof(Vertex);*/
};

// 정점을 담고 있는 buffer 클래스이다.
// Dx11에서 Polygon을 그리기 위해서는 primitive data가 필요하다.