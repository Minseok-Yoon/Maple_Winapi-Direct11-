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