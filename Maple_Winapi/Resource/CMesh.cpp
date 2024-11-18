#include "CMesh.h"

CMesh::Data::Data() :
	m_Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	m_vecVertices{},
	m_vecIndices{}
{
}

CMesh::Data::~Data()
{
}

CMesh::CMesh() :
	CResource(RESOURCE_TYPE::RT_Mesh)
{
}

CMesh::~CMesh()
{
}

HRESULT CMesh::Load(const wstring& _strPath)
{
	return S_OK;
}

bool CMesh::CreateVB(const vector<Vertex>& _vecVertices)
{
	m_Data.m_vecVertices = _vecVertices;
	return m_pVertexBuffer.Create(_vecVertices);
}

bool CMesh::CreateIB(const vector<UINT>& _vecIndices)
{
	m_Data.m_vecIndices = _vecIndices;
	return m_pIndexBuffer.Create(_vecIndices);
}

void CMesh::Bind()
{
	m_pInputLayout.Bind();

	m_pVertexBuffer.Bind();
	m_pIndexBuffer.Bind();

	GetDevice()->BindPrimitiveTopology(m_Data.m_Topology);
}

void CMesh::SetVertexBufferParams(UINT _iVertexCount, D3D11_INPUT_ELEMENT_DESC* layout, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength)
{
	m_pInputLayout.CreateInputLayout(_iVertexCount, layout, pShaderBytecodeWithInputSignature, BytecodeLength);
}
