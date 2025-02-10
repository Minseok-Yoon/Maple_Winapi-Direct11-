#pragma once
#include "Manager/CSceneManager.h"
#include "Scene/CLoadingScene.h"
#include "Scene/CScene_Start.h"
#include "Scene/CScene_Stage01.h"

void LoadScenes()
{
	CSceneManager::CreateScene<CLoadingScene>(L"LoadingScene");

	CSceneManager::LoadScene(L"LoadingScene");
}