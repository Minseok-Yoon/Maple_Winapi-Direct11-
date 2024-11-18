#include "CConstantBuffer.h"

CConstantBuffer::CConstantBuffer(CB_TYPE _eCBType) :
	m_iSize(0),
	m_eCBType(_eCBType)
{
}

CConstantBuffer::~CConstantBuffer()
{
}

bool CConstantBuffer::Create(UINT _iSize, void* data)
{
	m_iSize = _iSize;
	desc.ByteWidth = _iSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = data;

	bool succes = false;
	if (data == NULL)
		succes = GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
	else
		succes = GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf());

	if (!succes)
		assert(NULL);

	return true;
}

void CConstantBuffer::SetData(void* data) const
{
	GetDevice()->SetDataGpuBuffer(buffer.Get(), data, m_iSize);
}

void CConstantBuffer::Bind(SHADER_STAGE _eShaderStage) const
{
	GetDevice()->BindConstantBuffer(_eShaderStage, m_eCBType, buffer.Get());
}
