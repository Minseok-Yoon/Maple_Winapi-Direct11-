#include "CVertexBuffer.h"

CVertexBuffer::CVertexBuffer()
{
}

CVertexBuffer::~CVertexBuffer()
{
}

// Vertex Buffer�� ���� ����
bool CVertexBuffer::Create(const vector<Vertex>& _vecVertexes)
{
	// Vertex buffer ����
	// GpuBuffer�� ��ӹް� �־� D3D11_BUFFER_DESC desc; �� �޾ƿ´�.
	//ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));							// desc ������ �ʱ�ȭ. �޸𸮸� 0���� ä�� �ʱⰪ�� ��Ȯ�� �Ѵ�.
	desc.ByteWidth = sizeof(Vertex) * (UINT)_vecVertexes.size();			// ������ ��ü ũ�⸦ ����. Vertex�� ũ��� ������ ���� �� �޸� �뷮�� ����
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;				// �� ���۰� VertexBuffer�� ���� ������ ����. VertexBuffer�� ���ε�
	desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;							// �� ������ ��� ����� ����. DYNAMIC���� �����Ͽ� CPU���� ���� ������Ʈ�� ����
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;	// CPU���� ���ۿ� �����Ͽ� �����͸� �� �� �ֵ��� ����. �̸� ���� Vertex �����͸� CPU�� ������ �� �ְ� ��

	// Subresource ����
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = _vecVertexes.data(); // pSysMem�� Vertex �迭�� �����͸� �����մϴ�. GPU�� ������ �ʱ� �����͸� ����

	if (!(GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf())))
		assert(NULL && "VertexBuffer ������ �����߽��ϴ�.");

	return true;
}

void CVertexBuffer::Bind()
{
	/*if (buffer.GetAddressOf() == nullptr)
	{
		assert(NULL, "���ؽ� ���۸� ���� �� �� �����ϴ�.");
	}*/

	UINT offset = 0;
	UINT vectexSize = sizeof(Vertex);
	GetDevice()->BindVertexBuffer(0, 1, buffer.GetAddressOf(), &vectexSize, &offset);
}

//void CVertexBuffer::BindVertexBuffer(UINT _iStartSlot, UINT _iNumBuffers, ID3D11Buffer* const* _ppVertexBuffers, const UINT* _pStrides, const UINT* _pOffsets)
//{
//	GetDeviceContext()->GetID3D11DeviceContext()->
//		IASetVertexBuffers(_iStartSlot, _iNumBuffers, _ppVertexBuffers, _pStrides, _pOffsets);
//}