#pragma once
#include "../Entity/CEntity.h"

class CResource abstract : public CEntity
{
public:
	CResource(RESOURCE_TYPE _eResourceType);
	CResource();
	virtual ~CResource();

	virtual HRESULT Load(const wstring& _strPath) = 0;

public:
	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }
	const wstring& GetRelativePath() { return m_strRelativePath; }

private:
	wstring			m_strRelativePath;
	RESOURCE_TYPE	m_eResourceType;
};