#pragma once
#include "CScene.h"
#include "../Component/CLineRenderer.h"

class CRectDrawScene : public CScene
{
public:
    CRectDrawScene();
    ~CRectDrawScene();

    void Enter() override;
    void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
    void Exit() override;

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

private:
    CLineRenderer* m_pLineRenderer;
};