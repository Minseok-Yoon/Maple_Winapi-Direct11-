#pragma once
#include "CMonster.h"

class TestMonster : public CMonster
{
public:
    TestMonster();
    virtual ~TestMonster();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;
};