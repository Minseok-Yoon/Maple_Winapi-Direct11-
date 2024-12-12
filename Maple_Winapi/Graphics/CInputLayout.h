#pragma once
#include "CGraphicsDevice_DX11.h"

class CInputLayout
{
public:
	CInputLayout();
	~CInputLayout();

	void CreateInputLayout(UINT _iVertexCount, D3D11_INPUT_ELEMENT_DESC* _layout,
		const void* pShaderBytecodeWithInputSignature, SIZE_T _bytecodeLength);
	void Bind() const;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_InputLayout;
};