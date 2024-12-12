#include "CVertexBuffer.h"

CVertexBuffer::CVertexBuffer()
{
}

CVertexBuffer::~CVertexBuffer()
{
}

// Vertex Buffer의 생성 순서
bool CVertexBuffer::Create(const vector<Vertex>& _vecVertexes)
{
	// Vertex buffer 설정
	// GpuBuffer을 상속받고 있어 D3D11_BUFFER_DESC desc; 는 받아온다.
	//ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));									// desc 변수를 초기화. 메모리를 0으로 채워 초기값을 명확히 한다.
	desc.ByteWidth		= sizeof(Vertex) * static_cast<UINT>(_vecVertexes.size());	// 버퍼의 전체 크기를 설정. Vertex의 크기와 개수를 곱해 총 메모리 용량을 지정
	desc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;									// 이 버퍼가 VertexBuffer로 사용될 것임을 지정. VertexBuffer로 바인드
	desc.Usage			= D3D11_USAGE_DYNAMIC;										// 이 버퍼의 사용 방법을 지정. DYNAMIC으로 설정하여 CPU에서 자주 업데이트를 수행
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;									// CPU에서 버퍼에 접근하여 데이터를 쓸 수 있도록 설정. 이를 통해 Vertex 데이터를 CPU가 수정할 수 있게 함

	// Subresource 설정
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = _vecVertexes.data(); // pSysMem에 Vertex 배열의 데이터를 연결합니다. GPU로 전달할 초기 데이터를 지정

	if (!(GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf())))
		assert(NULL && "VertexBuffer 생성에 실패했습니다.");

	return true;
}

void CVertexBuffer::Bind()
{
	UINT offset = 0;
	UINT vectexSize = sizeof(Vertex);
	GetDevice()->BindVertexBuffer(0, 1, buffer.GetAddressOf(), &vectexSize, &offset);
}