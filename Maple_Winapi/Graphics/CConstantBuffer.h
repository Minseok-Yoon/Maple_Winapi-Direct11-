#pragma once
#include "CGraphicsDevice_DX11.h"

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

