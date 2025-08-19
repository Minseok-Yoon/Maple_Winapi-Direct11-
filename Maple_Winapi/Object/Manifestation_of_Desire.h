#pragma once
#include "CMonster.h"

class Manifestation_of_Desire : public CMonster
{
public:
    Manifestation_of_Desire();
    virtual ~Manifestation_of_Desire();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;
};