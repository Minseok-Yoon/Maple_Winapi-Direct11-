#pragma once
#include "CGraphicsDevice_DX11.h"

class CConstantBuffer : public GpuBuffer
{
public:
	CConstantBuffer(CB_TYPE _eCBType);
	~CConstantBuffer();

	bool Create(UINT _iSize, void* data = NULL);

	void SetData(void* data) const;
	void Bind(SHADER_STAGE _eShaderStage) const;

private:
	UINT	m_iSize;
	CB_TYPE	m_eCBType;
};

