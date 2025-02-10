#pragma once
#include "CScene.h"
#include "../Component/CPixelCollider.h"

class CPixScene : public CScene
{
public:
    CPixScene();
    ~CPixScene();

    void Enter() override;
    void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
    void Exit() override;

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
};