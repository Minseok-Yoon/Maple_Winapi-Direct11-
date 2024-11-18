#pragma once
#include "CScene.h"

class CDontDestroyOnLoad : public CScene
{
public:
	CDontDestroyOnLoad();
	~CDontDestroyOnLoad();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void Enter() override;
	void Exit() override;
};