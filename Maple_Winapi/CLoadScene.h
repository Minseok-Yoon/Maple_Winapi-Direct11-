#pragma once
#include "Manager/CSceneManager.h"
#include "Scene/CLoadingScene.h"
#include "Scene/CScene_Start.h"
#include "Scene/CScene_Stage01.h"

void LoadScenes()
{
	CSceneManager::CreateScene<CLoadingScene>(L"LoadingScene");

	CSceneManager::LoadScene(L"LoadingScene");

	const wchar_t* currentSceneName = CSceneManager::GetCurrentSceneName();

	std::wstringstream ss;
	ss << L"CurScene: " << currentSceneName << "\n";
	OutputDebugStringW(ss.str().c_str());
}