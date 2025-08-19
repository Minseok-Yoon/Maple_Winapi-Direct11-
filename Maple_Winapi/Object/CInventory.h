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

// 2025-06-12
// 내부적으로는 1차원 배열로 데이터를 저장하되, 논리적으로는 2차원 그리드로 관리해야 함.
