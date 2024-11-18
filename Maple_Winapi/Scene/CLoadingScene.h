#pragma once
#include "CScene.h"

class CLoadingScene : public CScene
{
public:
	CLoadingScene();
	~CLoadingScene();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void Enter() override;
	void Exit() override;

private:
	void resourcesLoad(std::mutex& _pMutex);
	void RenderLoadingImage();

private:
	bool			m_bLoadCompleted;
	std::thread* m_pResourcesLoadThread;
	std::mutex m_pMutualExclusion;
};