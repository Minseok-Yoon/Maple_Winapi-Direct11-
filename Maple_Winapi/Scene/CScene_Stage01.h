#pragma once
#include "CScene.h"

class CScene_Stage01 : public CScene
{
public:
    CScene_Stage01();
    ~CScene_Stage01();

    void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
    void Exit() override;

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

private:
    class CPlayer* m_pPlayer;
};