#pragma once
#include "../Object/CGameObject.h"

// ������ �⺻ ����
struct ItemData
{
    wstring     itemName;
    wstring     iconPath;
    int         maxStack;
};

// �κ��丮 ����
struct ItemSlot
{
    wstring     itemName;
    int         count;

    ItemSlot() : itemName(L""), count(0) {}
    ItemSlot(const wstring& _itemName, int _count) : itemName(_itemName), count(_count) {}
    bool IsEmpty() const { return itemName.empty(); }
};

class CCollider;
class CTexture;
class CItem : public CGameObject
{
public:
    CItem();
    virtual ~CItem();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;

    void SetItemInfo(const Vector2& _vPos) { m_vPos = _vPos; };
    Vector2 GetItemInfo() const { return m_vPos; }

    // 2025-05-30
    void SetDropItem(wstring _MapName);

    // 2025-06-02
    // ������ �̸� ��ȯ
    wstring GetItemName() const { return m_strItemName; }

    // ������ ���� �Լ�
    void OnPickUp();

public:
    Vector2 m_vPos;
    class CGameObject*  m_pItem;
    class CCollider*    m_pItemCollider;

    // 2025-05-30
    wstring             m_strItemName;
    CTexture*           m_pItemTexture;
};