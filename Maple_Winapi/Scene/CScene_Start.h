#pragma once
#include "CScene.h"

class CScene_Start : public CScene
{
public:
    CScene_Start();
    virtual ~CScene_Start();

    virtual void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
    virtual void Enter()override;
    virtual void Exit() override;

    virtual void Init();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();
};