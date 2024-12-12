#include "CConstantBuffer.h"

CConstantBuffer::CConstantBuffer(CB_TYPE _eCBType) :
	m_iSize(0),
	m_eCBType(_eCBType)
{
}

CConstantBuffer::~CConstantBuffer()
{
}

// Render의 LoadConstantBuffers에서 호출
bool CConstantBuffer::Create(UINT _iSize, void* data)
{
	m_iSize = _iSize;

	desc.ByteWidth = _iSize;						// 버퍼의 크기
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 상수 버퍼로 사용할 것을 명시
	desc.Usage = D3D11_USAGE_DYNAMIC;				// 동적으로 데이터를 갱신할 수 있도록 설정
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU에서 쓰기 가능하도록 설정

	// 초기 데이터를 위한 구조체 초기화
	D3D11_SUBRESOURCE_DATA sub = {};
	sub.pSysMem = data;

	bool succes = false;

	// 초기 데이터가 NULL인지 확인
	if (data == NULL)
		// 초기 데이터 없이 상수 버퍼를 생성
		succes = GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
	else
		// 초기 데이터를 포함하여 상수 버퍼를 생성
		succes = GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf());

	// 생성 실패 시 중단
	if (!succes)
		assert(NULL && "초기 데이터 생성에 실패했습니다.");

	return true;
}

void CConstantBuffer::SetData(void* data) const
{
	// GPU 상수 버퍼에 데이터를 업데이트하는 함수 호출
	// buffer: GPU에 할당된 버퍼
	// data: 업로드할 데이터
	// m_iSize: 데이터 크기
	GetDevice()->SetDataGpuBuffer(buffer.Get(), data, m_iSize);
}

void CConstantBuffer::Bind(SHADER_STAGE _eShaderStage) const
{
	// 상수 버퍼를 특정 셰이더 단계에 바인딩
	// _eShaderStage: 셰이더 단계 (Vertex, Pixel 등)
	// m_eCBType: 상수 버퍼 유형 (ex: 버퍼 슬롯)
	// buffer: GPU에 할당된 버퍼
	GetDevice()->BindConstantBuffer(_eShaderStage, m_eCBType, buffer.Get());
}