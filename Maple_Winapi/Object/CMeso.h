#pragma once
#include "CItem.h"

class CMeso : public CItem
{
public:
    CMeso();
    virtual ~CMeso();

private:
    int     m_iMoneyAmount;
    class CTexture* m_pTex;

public:
    void SetMoneyAmount(int _iAmount) { m_iMoneyAmount = _iAmount; }
    int GeMoneyAmount() const { return m_iMoneyAmount; }

public:
    virtual void Init();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render(HDC _hDC);
};