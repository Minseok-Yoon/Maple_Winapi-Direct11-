#include "../pch.h"
#include "CResourceManager.h"
#include "CPathManager.h"
#include "../Resource/CTexture.h"
#include "../Resource/CResource.h"

map<wstring, CResource*> CResourceManager::m_mapResources = {};

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
}

void CResourceManager::Insert(const wstring& _strKey, CResource* _pResource)
{
    if (_strKey == L"")
        return;

    if (_pResource == nullptr)
        return;

    m_mapResources.insert(make_pair(_strKey, _pResource));
}

void CResourceManager::Release()
{
    for (auto& iter : m_mapResources)
    {
        delete iter.second;
        iter.second = nullptr;
    }
}