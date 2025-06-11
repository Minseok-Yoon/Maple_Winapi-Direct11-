#pragma once
#include "CUI.h"
#include "CInventory.h"

class CInventoryUI :
    public CUI
{
public:
    CInventoryUI();
    virtual ~CInventoryUI();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender() override;
    virtual void OnClear() override;

    // 2025-06-09
    void SetInventory(CInventory* _pInventory) { m_pInventory = _pInventory; }

    // 2025-06-10
    void RefreshUI();
    void RegisterItemTexture(const wstring& _strItemName, const wstring& _strTextureKey);

private:
    int     m_iColumns = 5;
    int     m_iRows = 4;
    float   m_fSlotSize = 50.0f;
    float   m_fPadding = 5.0f;

    // 2025-06-09
    CInventory* m_pInventory = nullptr;

    // 2025-06-09 슬릇 오브젝트 및 텍스트 관리용 벡터 추가
    vector<CGameObject*>    m_vecSlotObjects;
    //vector<CTextUI*>        m_vecSlotTexts;

    // 2025-06-10
    unordered_map<wstring, CTexture*> m_mapItemTextures;
};

