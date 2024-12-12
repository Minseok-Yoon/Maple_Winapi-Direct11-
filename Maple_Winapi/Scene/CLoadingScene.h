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

	void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
	void Enter() override;
	void Exit() override;

private:
	void resourcesLoad(std::mutex& _pMutex);

private:
	bool			m_bLoadCompleted;
	std::thread* m_pResourcesLoadThread;
	std::mutex m_pMutualExclusion;
};