#pragma once
#include "../pch.h"

using namespace math;
class CEntity
{
public:
	CEntity();
	virtual ~CEntity();

	void SetName(const wstring& _strName) { m_strName = _strName; }
	wstring& GetName() { return m_strName; }

private:
	wstring m_strName;
};