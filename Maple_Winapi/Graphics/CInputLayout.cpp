#include "CInputLayout.h"

CInputLayout::CInputLayout() :
	m_InputLayout(nullptr)
{
}

CInputLayout::~CInputLayout()
{
}

void CInputLayout::CreateInputLayout(UINT _iVertexCount, D3D11_INPUT_ELEMENT_DESC* _layout,
	const void* pShaderBytecodeWithInputSignature, SIZE_T _bytecodeLength)
{
	if (!(GetDevice()->CreateInputLayout(_layout, _iVertexCount,
		pShaderBytecodeWithInputSignature,
		_bytecodeLength,
		m_InputLayout.GetAddressOf())))
		assert(NULL && "Create InputLayout failed!");
}

void CInputLayout::Bind() const
{
	if (m_InputLayout)
		GetDevice()->BindInputLayout(m_InputLayout.Get());
}
