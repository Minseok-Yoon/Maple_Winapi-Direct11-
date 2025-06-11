#pragma once
#include "CResource.h"
#include "CTexture.h"
#include "CShader.h"

class CMaterial : public CResource
{
public:
	struct Data	// Texture names
	{
		wstring albedo;	// diffuse
	};

	CMaterial();
	virtual ~CMaterial();

	virtual HRESULT Load(const wstring& _strPath) override;

	void Bind();
	void BindShader();
	void BindTextures();
	void SetRenderingMode(const RENDERING_MODE _eRenderingMode);

	void SetShader(CShader* _pShader) { m_pShader = _pShader; }
	void SetAlbedoTexture(CTexture* _pAlbedoTexture) { m_pAlbedoTexture = _pAlbedoTexture; m_Data.albedo = _pAlbedoTexture->GetName(); }

	RENDERING_MODE GetRenderingMode() const { return m_eRenderingMode; }

private:
	RENDERING_MODE	m_eRenderingMode;
	CMaterial::Data	m_Data;

	CTexture*		m_pAlbedoTexture;
	CShader*		m_pShader;
};

// 2025-06-11 Material이란?
// Material은 단어의 의미 그대로 '소재, 재질'을 의미한다. 약간 비유를 하자면 Geometry가 뼈대라면, Material은 살로 비유가 가능하다.
// Material의 종류에 따라 재질이 달라지고, 재질은 얼마나 빛을 반사하는지, 빛에 반응하여 어떤 느낌을 내는 지를 주로 결정한다.
