#pragma once
#include "CMonster.h"

class CBoss_Reward_Box : public CMonster
{
public:
    CBoss_Reward_Box();
    virtual ~CBoss_Reward_Box();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;
};