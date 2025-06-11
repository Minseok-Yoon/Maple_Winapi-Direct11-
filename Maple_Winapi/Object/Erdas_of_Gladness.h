#pragma once
#include "CMonster.h"

class Erdas_of_Gladness : public CMonster
{
public:
    Erdas_of_Gladness();
    virtual ~Erdas_of_Gladness();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;
    
private:

};