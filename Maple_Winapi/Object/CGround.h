#pragma once
#include "CGameObject.h"

class CColliderPixel;

class CBackGround : public CGameObject
{
public:
    CBackGround();
    virtual ~CBackGround();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;
};