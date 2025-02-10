#pragma once
#include "CScene.h"

class CLoadingScene : public CScene
{
public:
	CLoadingScene();
	virtual ~CLoadingScene();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

private:
	void resourcesLoad(std::mutex& _pMutex);

private:
	atomic<bool>	m_bLoadCompleted;
	std::thread* m_pResourcesLoadThread;
	std::mutex m_pMutualExclusion;
};