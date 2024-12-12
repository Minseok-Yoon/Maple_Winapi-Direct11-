#include "CConstantBuffer.h"

CConstantBuffer::CConstantBuffer(CB_TYPE _eCBType) :
	m_iSize(0),
	m_eCBType(_eCBType)
{
}

CConstantBuffer::~CConstantBuffer()
{
}

// Render�� LoadConstantBuffers���� ȣ��
bool CConstantBuffer::Create(UINT _iSize, void* data)
{
	m_iSize = _iSize;

	desc.ByteWidth = _iSize;						// ������ ũ��
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// ��� ���۷� ����� ���� ���
	desc.Usage = D3D11_USAGE_DYNAMIC;				// �������� �����͸� ������ �� �ֵ��� ����
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU���� ���� �����ϵ��� ����

	// �ʱ� �����͸� ���� ����ü �ʱ�ȭ
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = data;

	bool succes = false;

	// �ʱ� �����Ͱ� NULL���� Ȯ��
	if (data == NULL)
		// �ʱ� ������ ���� ��� ���۸� ����
		succes = GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
	else
		// �ʱ� �����͸� �����Ͽ� ��� ���۸� ����
		succes = GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf());

	// ���� ���� �� �ߴ�
	if (!succes)
		assert(NULL && "�ʱ� ������ ������ �����߽��ϴ�.");

	return true;
}

void CConstantBuffer::SetData(void* data) const
{
	// GPU ��� ���ۿ� �����͸� ������Ʈ�ϴ� �Լ� ȣ��
	// buffer: GPU�� �Ҵ�� ����
	// data: ���ε��� ������
	// m_iSize: ������ ũ��
	GetDevice()->SetDataGpuBuffer(buffer.Get(), data, m_iSize);
}

void CConstantBuffer::Bind(SHADER_STAGE _eShaderStage) const
{
	// ��� ���۸� Ư�� ���̴� �ܰ迡 ���ε�
	// _eShaderStage: ���̴� �ܰ� (Vertex, Pixel ��)
	// m_eCBType: ��� ���� ���� (ex: ���� ����)
	// buffer: GPU�� �Ҵ�� ����
	GetDevice()->BindConstantBuffer(_eShaderStage, m_eCBType, buffer.Get());
}