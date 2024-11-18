#include "CInputLayout.h"

CInputLayout::CInputLayout() :
	m_InputLayout(nullptr)
{
}

CInputLayout::~CInputLayout()
{
	/*if (m_InputLayout != nullptr)
	{
		m_InputLayout->Release();
		m_InputLayout = nullptr;
	}*/
}

//void CInputLayout::AddInputLayoutDesc(LPCSTR _SemanticName, DXGI_FORMAT _Format, 
//	UINT _SemanticIndex, UINT _AligneByteOffset, D3D11_INPUT_CLASSIFICATION _InputSlotClass, 
//	UINT _InputSlot, UINT _InstanceDataStepRate)
//{
//	D3D11_INPUT_ELEMENT_DESC InputDesc;
//	InputDesc.SemanticName = _SemanticName;
//	InputDesc.SemanticIndex = _SemanticIndex;
//	InputDesc.Format = _Format;
//	InputDesc.InputSlot = _InputSlot;
//	InputDesc.InputSlotClass = _InputSlotClass;
//	InputDesc.InstanceDataStepRate = _InstanceDataStepRate;
//
//	// 바이드 오프셋 설정 - 기본값이 -1인 경우 내부 offset값 사용
//	if (_AligneByteOffset == -1)
//		InputDesc.AlignedByteOffset = Offset;
//	else
//		InputDesc.AlignedByteOffset = _AligneByteOffset;
//
//	// FormatSize 함수로 요소의 바이트 크기를 계산하여 offset에 추가
//	Offset += FormatSize(_Format);
//	m_vecInfo.push_back(InputDesc);
//}

void CInputLayout::CreateInputLayout(UINT _iVertexCount, D3D11_INPUT_ELEMENT_DESC* _layout,
	const void* pShaderBytecodeWithInputSignature, SIZE_T _bytecodeLength)
{
	if (!(GetDevice()->CreateInputLayout(_layout, _iVertexCount,
		pShaderBytecodeWithInputSignature,
		_bytecodeLength,
		m_InputLayout.GetAddressOf())))
		assert(NULL && "Create InputLayout failed!");
}

void CInputLayout::Bind()
{
	if (m_InputLayout == nullptr)
		assert(NULL);

	if (m_InputLayout)
		GetDevice()->BindInputLayout(m_InputLayout.Get());
}
