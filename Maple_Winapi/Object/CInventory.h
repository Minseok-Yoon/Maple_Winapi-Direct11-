#pragma once
#include "CItem.h"

class CItem;
class CInventory
{
	SINGLETON(CInventory);

public:
	void AddItem(const wstring& _itemName, int count = 1);
	bool RemoveItem(const wstring& _itemName, int count = 1);

	const vector<ItemSlot>& GetSlots() const { return m_vecSlots; }

private:
	vector<ItemSlot> m_vecSlots;

	// 2025-06-10
	function<void()> onInventoryChanged;
};