#pragma once
#include "CGraphicsDevice_DX11.h"

class CInputLayout
{
public:
	CInputLayout();
	~CInputLayout();

	//// InputLayout 요소를 추가
	//void AddInputLayoutDesc(
	//	LPCSTR _SemanticName,														// 데이터 의미(ex: "POSITION")
	//	DXGI_FORMAT	_Format,														// 데이터 형식(ex: DXGI_FORMAT_R32G32B32_FLOAT)
	//	UINT _SemanticIndex = 0,													// 세부 인덱스(ex: POSITION0)
	//	UINT _AligneByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,						// 바이트 오프셋(자동 정렬)
	//	D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,	// 입력 유형
	//	UINT _InputSlot = 0,														// 입력 슬롯 (예: 0)
	//	UINT _InstanceDataStepRate = 0												// 인스턴스 데이터 속도 (기본값 0)
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