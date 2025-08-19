#include "../pch.h"
#include "CSceneFactory.h"

void CSceneFactory::RegisterScene(const wstring& strName, SceneFactoryFunc func)
{
	GetRegistry()[strName] = func;
}

CScene* CSceneFactory::CreateScene(const wstring& strName)
{
	/*auto it = GetRegistry().find(strName);

	OutputDebugStringW((L"[SceneFactory] ��ϵ��� ���� �� ��û: " + strName + L"\n").c_str());
	return (it != GetRegistry().end()) ? it->second() : nullptr;*/
	auto& reg = GetRegistry();
	auto it = reg.find(strName);
	if (it != reg.end())
		return it->second();

	OutputDebugStringW((L"[SceneFactory] ��ϵ��� ���� �� ��û: " + strName + L"\n").c_str());
	return nullptr;
}

unordered_map<wstring, SceneFactoryFunc>& CSceneFactory::GetRegistry()
{
	static unordered_map<wstring, SceneFactoryFunc> registry;
	return registry;
}
