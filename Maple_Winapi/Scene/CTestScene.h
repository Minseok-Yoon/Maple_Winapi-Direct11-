#pragma once
#include "CScene.h"

class CTestScene : public CScene
{
public:
    CTestScene();
    ~CTestScene();

    void Enter() override;
    void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
    void Exit() override;

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
};