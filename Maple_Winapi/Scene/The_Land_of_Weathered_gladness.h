#pragma once
#include "The_Lake_of_Oblivion.h"

class The_Land_of_Weathered_gladness : public The_Lake_of_Oblivion
{
public:
    The_Land_of_Weathered_gladness();
    ~The_Land_of_Weathered_gladness();

    void Enter() override;
    void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
    void Exit() override;

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
};