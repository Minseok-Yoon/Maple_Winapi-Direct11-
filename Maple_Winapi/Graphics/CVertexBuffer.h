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
	//	const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets);	// ���� ���۸� GPU�� ���ε�

private:
	/*UINT offset = 0;
	UINT vectexSize = sizeof(Vertex);*/
};

// ������ ��� �ִ� buffer Ŭ�����̴�.
// Dx11���� Polygon�� �׸��� ���ؼ��� primitive data�� �ʿ��ϴ�.