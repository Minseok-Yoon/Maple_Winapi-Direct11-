#pragma once
#include "CGraphicsDevice_DX11.h"

// ConstantBuffer(상수버퍼) - 상수 데이터를 보내주는 버퍼로 
// 월드변환 행렬, 뷰 변환 행렬, 투영 변환 행렬을 셰이더로 넘겨주어 각 정점에 곱할 수 있게 해준다.
class CConstantBuffer : public GpuBuffer
{
public:
	CConstantBuffer(CB_TYPE _eCBType);
	virtual ~CConstantBuffer();

	bool Create(UINT _iSize, void* data = nullptr);

	void SetData(void* data) const;
	void Bind(SHADER_STAGE _eShaderStage) const;

private:
	UINT	m_iSize;
	CB_TYPE	m_eCBType;
};

