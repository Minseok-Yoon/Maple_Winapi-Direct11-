#include "../pch.h"
#include "CInventoryUI.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CUIManager.h"
#include "CInventory.h"
#include "../Component/CTransform.h"
#include "../Object/CObject.h"
#include "../Resource/CTexture.h"
#include "../Component/CSpriteRenderer.h"

CInventoryUI::CInventoryUI() :
	CUI(UI_TYPE::UI_Inventory)
{
	SetFullScreen(false);
	SetZOrder(10);
	SetPos(Vector2(100.0f, 100.0f));	// UI ��ġ
	SetSize(Vector2(400.0f, 300.0f));	// UI ũ��
}

CInventoryUI::~CInventoryUI()
{
}

void CInventoryUI::OnInit()
{
	CTransform* parentTr = this->GetComponent<CTransform>();
	Vector3 basePos = parentTr->GetLocalPosition();

	CMesh* pRectMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	CMaterial* pMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");

	CMesh* pBorderMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	CMaterial* pBorderMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Border-Material");

	size_t maxSlots = m_iRows * m_iColumns;

	for (size_t i = 0; i < maxSlots; ++i)
	{
		int row = static_cast<int>(i) / m_iColumns;
		int col = static_cast<int>(i) % m_iColumns;

		Vector3 slotPos = basePos + Vector3(
			col * (m_fSlotSize + m_fPadding),
			row * (m_fSlotSize + m_fPadding),
			0.0f
		);

		// ���� ������Ʈ ����
		CGameObject* pSlot = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
		pSlot->SetName(L"InventorySlot_" + std::to_wstring(i));
		pSlot->SetZOrder(1);

		// Transform ����
		auto slotTr = pSlot->AddComponent<CTransform>();
		slotTr->SetParent(parentTr);
		slotTr->SetLocalPosition(slotPos);
		slotTr->SetLocalScale(Vector3(m_fSlotSize, m_fSlotSize, 1.0f));

		// SpriteRenderer ����
		auto pRenderer = pSlot->AddComponent<CSpriteRenderer>();
		pRenderer->SetMesh(pBorderMesh);
		pRenderer->SetMaterial(pBorderMaterial);
		pRenderer->Init();

		// ����
		m_vecSlotObjects.push_back(pSlot);
	}
}

void CInventoryUI::OnActive()
{
	for (auto& pSlot : m_vecSlotObjects)
	{
		if (pSlot)
			pSlot->SetActive(true);
	}

	RefreshUI();
}

void CInventoryUI::OnInActive()
{
	for (auto& pSlot : m_vecSlotObjects)
	{
		if (pSlot)
			pSlot->SetActive(false);
	}
}

void CInventoryUI::OnUpdate()
{
}

void CInventoryUI::OnLateUpdate()
{
}

void CInventoryUI::OnRender()
{
	// 2025-06-09 �ؽ�ó ���� ��ɸ� ����
	if (m_pInventory == nullptr) return;

	const auto& slots = m_pInventory->GetSlots();
	size_t maxSlots = m_iRows * m_iColumns;
	size_t count = std::min(slots.size(), maxSlots);
}

void CInventoryUI::OnClear()
{
	InActive();
}

// 2025-06-11
// ���� ���� �ؽ�ó ���� ���̴����� �ܻ����� ó��(����� �����ϰ� �׸��� �׵θ��� �׸���)�� �ϰ� �ִµ�
// �� ������ �ؽ�ó�� ������ �˾ƺ���.

void CInventoryUI::RefreshUI()
{
	if (m_pInventory == nullptr) return;

	const auto& slots = m_pInventory->GetSlots();
	CMaterial* pDefaultMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");
	CMaterial* pBorderMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Border-Material");

	for (size_t i = 0; i < m_vecSlotObjects.size(); ++i)
	{
		CGameObject* pSlotObj = m_vecSlotObjects[i];
		if (!pSlotObj) continue;

		auto pRenderer = pSlotObj->GetComponent<CSpriteRenderer>();
		if (!pRenderer) continue;

		// �׻� ������ ���̰� ��
		pSlotObj->SetActive(true);

		// �������� �ִ� ����
		if (i < slots.size() && !slots[i].itemName.empty())
		{
			CTexture* pTex = CResourceManager::Find<CTexture>(slots[i].itemName);
			if (pTex)
			{
				// ������ �ؽ�ó�� ������ �Ϲ� ��Ƽ����� ��ü
				pRenderer->SetMaterial(pDefaultMaterial);
				pRenderer->SetTexture(pTex);
			}
			else
			{
				// �ؽ�ó�� ��� �׵θ��� ������
				pRenderer->SetMaterial(pBorderMaterial);
				pRenderer->SetTexture(nullptr);
			}
		}
		else
		{
			// �� ������ �׵θ��� ������
			pRenderer->SetMaterial(pBorderMaterial);
			pRenderer->SetTexture(nullptr);
		}
	}

	// �����: �κ��丮 ���� ���
	wstringstream ss;
	ss << L"=== �κ��丮 ���� ===\n";
	ss << L"�� ���� ��: " << m_vecSlotObjects.size() << L"\n";
	ss << L"������ ��: " << slots.size() << L"\n";
	for (size_t i = 0; i < slots.size(); ++i)
	{
		ss << L"����[" << i << L"]: " << slots[i].itemName.c_str() << L")\n";
	}
	OutputDebugStringW(ss.str().c_str());
}

void CInventoryUI::RegisterItemTexture(const wstring& _strItemName, const wstring& _strTextureKey)
{
	CTexture* tex = CResourceManager::Find<CTexture>(_strTextureKey);
	if (tex)
	{
		m_mapItemTextures[_strItemName] = tex;
	}
}

//void CInventoryUI::OnRender()
//{
//	// 2025-06-09 �ؽ�ó ���� ��ɸ� ����
//
//	const auto& slots = m_pCInventory->GetSlots();
//	Vector2 basePos = GetLocalPosition();
//
//	for (size_t i = 0; i < slots.size(); ++i)
//	{
//		int row = i / m_iColumns;
//		int col = i % m_iColumns;
//
//		Vector2 slotPos = basePos + Vector2(
//			col * (m_fSlotSize + m_fPadding),
//			row * (m_fSlotSize + m_fPadding)
//		);
//
//		// �簢�� �׸���
//		Renderer2D::DrawRect(slotPos, Vector2(m_fSlotSize, m_fSlotSize), Color::Gray);
//
//		// �ؽ�Ʈ �׸���
//		wstring text = slots[i].itemName + L" x" + to_wstring(slots[i].count);
//		Renderer2D::DrawText(slotPos + Vector2(5.0f, 5.0f), text, Color::White);
//	}
//}

// 2025-06-10 �ּ�
//void CInventoryUI::OnInit()
//{
//	CTransform* parentTr = this->GetComponent<CTransform>();
//	Vector3 basePos = parentTr->GetLocalPosition();
//
//	CMesh* pRectMesh = CResourceManager::Find<CMesh>(L"RectMesh");
//	CMaterial* pMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");
//
//	size_t maxSlots = m_iRows * m_iColumns;
//
//	for (size_t i = 0; i < maxSlots; ++i)
//	{
//		int row = static_cast<int>(i) / m_iColumns;
//		int col = static_cast<int>(i) % m_iColumns;
//
//		Vector3 slotPos = basePos + Vector3(
//			col * (m_fSlotSize + m_fPadding),
//			row * (m_fSlotSize + m_fPadding),
//			0.0f
//		);
//
//		// ���� ������Ʈ ����
//		CGameObject* pSlot = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//		pSlot->SetName(L"InventorySlot_" + std::to_wstring(i));
//		pSlot->SetZOrder(1);
//
//		// Transform ����
//		auto slotTr = pSlot->AddComponent<CTransform>();
//		slotTr->SetParent(parentTr);
//		slotTr->SetLocalPosition(slotPos);
//		slotTr->SetLocalScale(Vector3(m_fSlotSize, m_fSlotSize, 1.0f));
//
//		// SpriteRenderer ����
//		auto pRenderer = pSlot->AddComponent<CSpriteRenderer>();
//		pRenderer->SetMesh(pRectMesh);
//		pRenderer->SetMaterial(pMaterial);
//		pRenderer->Init();
//		//pRenderer->GetMaterial()->SetColor(Color::DarkGray);
//
//		//// �ؽ�Ʈ UI ���� �� ���� �ڽ����� ����
//		//CTextUI* pText = Instantiate<CTextUI>(LAYER_TYPE::LT_UI);
//		//pText->SetText(L"", 12.0f, Color::White);
//		//pText->GetComponent<CTransform>()->SetParent(slotTr);
//		//pText->GetComponent<CTransform>()->SetLocalPosition(Vector3(5.0f, -5.0f, 0.0f));
//
//		// ����
//		m_vecSlotObjects.push_back(pSlot);
//		//m_vSlotTexts.push_back(pText);
//	}
//}
//void CInventoryUI::OnRender()
//{
//	// 2025-06-09 �ؽ�ó ���� ��ɸ� ����
//	if (m_pInventory == nullptr) return;
//
//	const auto& slots = m_pInventory->GetSlots();
//	size_t maxSlots = m_iRows * m_iColumns;
//	size_t count = std::min(slots.size(), maxSlots);
//
//	// ������ �ִ� ���� �ؽ�Ʈ ����
//	//for (size_t i = 0; i < count; ++i)
//	//{
//	//	const auto& slot = slots[i];
//	//	if (!slot.itemName.empty())
//	//	{
//	//		wstring text = slot.itemName + L" x" + std::to_wstring(slot.count);
//	//		m_vSlotTexts[i]->SetText(text, 12.0f, Color::White);
//	//	}
//	//	else
//	//	{
//	//		m_vSlotTexts[i]->SetText(L"", 12.0f, Color::White);
//	//	}
//	//}
//
//	//// ���� ���� �ؽ�Ʈ Ŭ����
//	//for (size_t i = count; i < maxSlots; ++i)
//	//{
//	//	m_vSlotTexts[i]->SetText(L"", 12.0f, Color::White);
//	//}
//}
//void CInventoryUI::OnClear()
//{
//	InActive();
//	/*for (auto pSlot : m_vecSlotObjects)
//		SAFE_RELEASE(pSlot);
//	m_vecSlotObjects.clear();*/
//
//	/*for (auto pText : m_vSlotTexts)
//		SafeRelease(pText);
//	m_vSlotTexts.clear();*/
//}

// 2025-06-11 ��������� ������
// ����� ���信�� �ٽ� �Ҹ��� ���η� ���ư��� ���� ������ ���� ���� �������� �� ����
// ������ �� ���� ���� ���� ������(ĳ���ʹ� ��� ���̿� ����, ������ ������ִ°� + ���ο� ��)

// 2025-06-10 ���� ����� RefreshUI()
//void CInventoryUI::RefreshUI()
//{
//	if (m_pInventory == nullptr) return;
//
//	const auto& slots = m_pInventory->GetSlots();
//	size_t count = std::min(slots.size(), m_vecSlotObjects.size());
//
//	for (size_t i = 0; i < slots.size(); ++i)
//	{
//		wstringstream ss;
//		ss << L"Slot[" << i << L"]: " << slots[i].itemName.c_str() << L"\n";
//		OutputDebugStringW(ss.str().c_str());
//	}
//
//	for (size_t i = 0; i < count; ++i)
//	{
//		const auto& item = slots[i];
//		CGameObject* pSlotObj = m_vecSlotObjects[i];
//		if (!pSlotObj) continue;
//
//		auto pRenderer = pSlotObj->GetComponent<CSpriteRenderer>();
//		if (!pRenderer) continue;
//
//		// ������ �̸����� �ؽ�ó ã��
//		CTexture* pTex = CResourceManager::Find<CTexture>(item.itemName);
//		if (pTex)
//		{
//			pRenderer->SetTexture(pTex);
//			pSlotObj->SetActive(true);
//		}
//		else
//		{
//			// �ؽ�ó�� ���� ��� �� ���� ó��
//			pRenderer->SetTexture(nullptr);
//			pSlotObj->SetActive(true);
//		}
//	}
//
//	// ������ ������ ��Ȱ��ȭ
//	for (size_t i = count; i < m_vecSlotObjects.size(); ++i)
//	{
//		CGameObject* pSlotObj = m_vecSlotObjects[i];
//		if (!pSlotObj) continue;
//
//		auto pRenderer = pSlotObj->GetComponent<CSpriteRenderer>();
//		if (pRenderer)
//			pRenderer->SetTexture(nullptr);
//
//		pSlotObj->SetActive(true);
//	}
//}