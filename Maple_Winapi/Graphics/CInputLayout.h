#pragma once
#include "CGraphicsDevice_DX11.h"

class CInputLayout
{
public:
	CInputLayout();
	~CInputLayout();

	//// InputLayout ��Ҹ� �߰�
	//void AddInputLayoutDesc(
	//	LPCSTR _SemanticName,														// ������ �ǹ�(ex: "POSITION")
	//	DXGI_FORMAT	_Format,														// ������ ����(ex: DXGI_FORMAT_R32G32B32_FLOAT)
	//	UINT _SemanticIndex = 0,													// ���� �ε���(ex: POSITION0)
	//	UINT _AligneByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,						// ����Ʈ ������(�ڵ� ����)
	//	D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,	// �Է� ����
	//	UINT _InputSlot = 0,														// �Է� ���� (��: 0)
	//	UINT _InstanceDataStepRate = 0												// �ν��Ͻ� ������ �ӵ� (�⺻�� 0)
	//	);

	void CreateInputLayout(UINT _iVertexCount, D3D11_INPUT_ELEMENT_DESC* _layout,
		const void* pShaderBytecodeWithInputSignature, SIZE_T _bytecodeLength);
	void Bind();

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_InputLayout;

	/*vector<D3D11_INPUT_ELEMENT_DESC>			m_vecInfo;
	int Offset = 0;
	UINT FormatSize(DXGI_FORMAT _Format);*/
};