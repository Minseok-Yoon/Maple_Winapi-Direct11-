#include "../pch.h"
#include "CPathManager.h"
#include "../Core/CCore.h"

CPathManager::CPathManager() :
	m_szContentPath{}
{
}

CPathManager::~CPathManager()
{
}

void CPathManager::Init()
{
	// ���� �۾� ���͸� ��θ� m_szContentPath�� �����Ѵ�.
	GetCurrentDirectory(255, m_szContentPath);
	// ���� �۾� ���͸� ����� ���̸� ���Ѵ�.
	int iLen = (int)wcslen(m_szContentPath);

	// ���������� �̵��Ͽ� Content��θ� �����Ѵ�.
	for (int i = iLen - 1; i >= 0; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\Resources");

	// ���� �޽����� ���ڿ��� ��ȯ
	std::wstringstream ss;
	ss << L"file: " << m_szContentPath << std::endl;

	// ��ȯ�� ���ڿ��� ���â�� ���
	OutputDebugString(ss.str().c_str());
}

wstring CPathManager::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;

	// ���� ����� ���̸� ���Ѵ�.
	size_t iAbsLen = wcslen(m_szContentPath);
	// ��ü ������ ����� ���̸� ���Ѵ�.
	size_t iFullLen = strFilePath.length();

	// ���� ��θ� �������� ��� ��θ� �����Ѵ�.
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);
	// ��� ��θ� ��ȯ�Ѵ�.
	return strRelativePath;
}

wstring CPathManager::GetDesktopPath()
{
	PWSTR path = nullptr;

	// FOLDERID_Desktop�� ����� ����ũž ��θ� ������
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path)))
	{
		std::wstring desktopPath(path);
		CoTaskMemFree(path); // �޸� ����
		return desktopPath;
	}
	return L""; // ������ ��� �� ���ڿ� ��ȯ

	return wstring();
}

// ���� ��� ��ȯ �Լ�
wstring CPathManager::GetCorrectFilePath(const wstring& _strRelativePath)
{
	wstring desktopPath = GetDesktopPath();
	return desktopPath + _strRelativePath;
}
