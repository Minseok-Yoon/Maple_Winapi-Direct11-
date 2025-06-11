#pragma once
#include "CItem.h"

class CEtcItem : public CItem
{
public:
    CEtcItem();
    virtual ~CEtcItem();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;
};