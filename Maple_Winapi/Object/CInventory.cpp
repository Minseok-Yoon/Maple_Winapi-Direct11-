#include "../pch.h"
#include "CInventory.h"
#include "CInventoryUI.h"
#include "../Manager/CUIManager.h"

CInventory::CInventory()
{
}

CInventory::~CInventory()
{
}

// 1. ���� �������� ������ ã�� -> �����ϸ� ���� ����, �������� ������ ���� �߰�
// 2. �ݹ� �� UI ����
void CInventory::AddItem(const wstring& _itemName, int count)
{
	for (auto& slot : m_vecSlots)
	{
		if (slot.itemName == _itemName)
		{
			slot.count += count;
			return;
		}
	}
	m_vecSlots.emplace_back(_itemName, count);

	if (onInventoryChanged)
		onInventoryChanged();

	CInventoryUI* pUI = dynamic_cast<CInventoryUI*>(CUIManager::GetInst()->GetUI(UI_TYPE::UI_Inventory));
	if (pUI)
		pUI->RefreshUI();
}

bool CInventory::RemoveItem(const wstring& _itemName, int count)
{
	for (auto iter = m_vecSlots.begin(); iter != m_vecSlots.end(); ++iter)
	{
		if (iter->itemName == _itemName)
		{
			iter->count -= count;
			if (iter->count <= 0)
				m_vecSlots.erase(iter);
			return true;
		}
	}
	return false;
}
