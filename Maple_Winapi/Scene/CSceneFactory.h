#pragma once

class CScene;
using SceneFactoryFunc = std::function<CScene*()>;

class CSceneFactory
{
public:
	static void RegisterScene(const wstring& strName, SceneFactoryFunc func);
	static CScene* CreateScene(const wstring& strName);

private:
	static unordered_map<wstring, SceneFactoryFunc>& GetRegistry();
};