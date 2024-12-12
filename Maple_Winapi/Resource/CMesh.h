#pragma once
#include "CResource.h"
#include "../Graphics/CInputLayout.h"
#include "../Graphics/CVertexBuffer.h"
#include "../Graphics/CIndexBuffer.h"

// 레스터라이저 전에 범위를 잡을 때 사용하는 VB(VertexBuffer), IB(IndexBuffer)를 관리하는 클래스
class CMesh : public CResource
{
public:
	struct Data
	{
		Data();
		~Data();

		D3D11_PRIMITIVE_TOPOLOGY	m_Topology;
		vector<Vertex>				m_vecVertices;
		vector<UINT>				m_vecIndices;
	};

	CMesh();
	virtual ~CMesh();

	virtual HRESULT Load(const wstring& _strPath) override;

	bool CreateVB(const vector<Vertex>& _vecVertices);
	bool CreateIB(const vector<UINT>& _vecIndices);

	void Bind();

	void SetVertexBufferParams(UINT _iVertexCount, D3D11_INPUT_ELEMENT_DESC* layout,
		const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength);

	UINT GetIndexCount() const { return m_pIndexBuffer.GetIndexCount(); }

private:
	CInputLayout	m_pInputLayout;
	CVertexBuffer	m_pVertexBuffer;
	CIndexBuffer	m_pIndexBuffer;

	Data			m_Data;
};

