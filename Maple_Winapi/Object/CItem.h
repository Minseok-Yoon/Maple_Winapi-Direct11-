#pragma once
#include "../Object/CGameObject.h"

class CCollider;

class CItem : public CGameObject
{
public:
    CItem();
    virtual ~CItem();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;

    void SetItemInfo(const Vector2& _vPos) { m_vPos = _vPos; };
    Vector2 GetItemInfo() const { return m_vPos; }

public:
    Vector2 m_vPos;
    class CGameObject* m_pItem;
};