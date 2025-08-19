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
	//SetPos(Vector2(100.0f, 100.0f));	// UI 위치
	//SetSize(Vector2(400.0f, 300.0f));	// UI 크기
}

CInventoryUI::~CInventoryUI()
{
	// 2025-06-12 활성화 비활성화 하는데 왜 슬릇을 정리하는지?(밑에 내용 확인)
	for (auto pSlot : m_vecSlotObjects)
	{
		if (pSlot)
		{
			Destroy(pSlot);
		}
	}
	m_vecSlotObjects.clear();
}

void CInventoryUI::OnInit()
{
	CTransform* parentTr = this->GetComponent<CTransform>();
	Vector3 basePos = parentTr->GetLocalPosition();

	CMesh* pRectMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	CMesh* pBorderMesh = CResourceManager::Find<CMesh>(L"RectMesh");
	CMaterial* pBorderMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Border-Material");
	CTexture* pDummyTex = CResourceManager::Find<CTexture>(L"DummyTexture");

	size_t maxSlots = m_iRows * m_iColumns;

	// 2025-06-12 기존 슬릇들이 있다면 모두 삭제?
	for (auto pSlot : m_vecSlotObjects)
	{
		if (pSlot)
		{
			Destroy(pSlot);
		}
	}
	m_vecSlotObjects.clear();

	for (size_t i = 0; i < maxSlots; ++i)
	{
		int row = static_cast<int>(i) / m_iColumns;
		int col = static_cast<int>(i) % m_iColumns;

		Vector3 slotPos = basePos + Vector3(
			col * (m_fSlotSize + m_fPadding),
			row * (m_fSlotSize + m_fPadding),
			0.0f
		);

		// 슬롯 오브젝트 생성
		CGameObject* pSlot = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
		pSlot->SetName(L"InventorySlot_" + std::to_wstring(i));
		pSlot->SetZOrder(1);

		// Transform 세팅
		auto slotTr = pSlot->AddComponent<CTransform>();
		slotTr->SetParent(parentTr);
		slotTr->SetLocalPosition(slotPos);
		slotTr->SetLocalScale(Vector3(m_fSlotSize, m_fSlotSize, 1.0f));

		// SpriteRenderer 세팅
		auto pRenderer = pSlot->AddComponent<CSpriteRenderer>();
		pRenderer->SetMesh(pBorderMesh);
		pRenderer->SetMaterial(pBorderMaterial);

		// 초기화 시 DummyTexture 설정
		if (pDummyTex)
		{
			pRenderer->SetTexture(pDummyTex);
		}

		pRenderer->Init();

		// 저장
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
	// 2025-06-09 텍스처 없이 기능만 구현
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
// 지금 보면 텍스처 없이 쉐이더에서 단색으로 처리(배경을 투명하게 그리고 테두리만 그리는)로 하고 있는데
// 왜 쓰레기 텍스처가 들어가는지 알아보기.(기존의 SRV에 텍스처가 남아 있어 그런 현상이 발생함)
// DummyTexture을 넣은 것으로 해결.

void CInventoryUI::RefreshUI()
{
	if (m_pInventory == nullptr) return;

	const auto& slots = m_pInventory->GetSlots();
	CMaterial* pDefaultMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");
	CMaterial* pBorderMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Border-Material");
	CTexture* pDummyTex = CResourceManager::Find<CTexture>(L"DummyTexture");

	for (size_t i = 0; i < m_vecSlotObjects.size(); ++i)
	{
		CGameObject* pSlotObj = m_vecSlotObjects[i];
		if (!pSlotObj) continue;

		auto pRenderer = pSlotObj->GetComponent<CSpriteRenderer>();
		if (!pRenderer) continue;

		// 항상 슬롯은 보이게 함
		pSlotObj->SetActive(true);

		// 먼저 기존 텍스처를 완전히 클리어
		pRenderer->SetTexture(nullptr);  // 또는 pRenderer->ClearTexture();

		// 아이템이 있는 슬롯인지 확인
		if (i < slots.size() && !slots[i].itemName.empty())
		{
			CTexture* pItemTex = CResourceManager::Find<CTexture>(slots[i].itemName);
			if (pItemTex)
			{
				wstring debugMsg = L"슬롯[" + to_wstring(i) + L"] 아이템 텍스처 설정: " + slots[i].itemName + L"\n";
				OutputDebugStringW(debugMsg.c_str());

				pRenderer->SetMaterial(pDefaultMaterial);
				pRenderer->SetTexture(pItemTex);
			}
		}
		else
		{
			wstring debugMsg = L"슬롯[" + to_wstring(i) + L"] 더미 텍스처 설정\n";
			OutputDebugStringW(debugMsg.c_str());

			pRenderer->SetMaterial(pBorderMaterial);
			pRenderer->SetTexture(pDummyTex);
		}

		// 현재 설정된 텍스처 확인
		CTexture* currentTex = pRenderer->GetTexture();
		if (currentTex)
		{
			wstring currentTexName = currentTex->GetName();
			wstring debugMsg2 = L"슬롯[" + to_wstring(i) + L"] 현재 텍스처: " + currentTexName + L"\n";
			OutputDebugStringW(debugMsg2.c_str());
		}
	}

	// 디버그: 인벤토리 상태 출력
	wstringstream ss;
	ss << L"=== 인벤토리 상태 ===\n";
	ss << L"총 슬롯 수: " << m_vecSlotObjects.size() << L"\n";
	ss << L"아이템 수: " << slots.size() << L"\n";
	for (size_t i = 0; i < slots.size(); ++i)
	{
		ss << L"슬롯[" << i << L"]: " << slots[i].itemName.c_str() << L")\n";
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
//	// 2025-06-09 텍스처 없이 기능만 구현
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
//		// 사각형 그리기
//		Renderer2D::DrawRect(slotPos, Vector2(m_fSlotSize, m_fSlotSize), Color::Gray);
//
//		// 텍스트 그리기
//		wstring text = slots[i].itemName + L" x" + to_wstring(slots[i].count);
//		Renderer2D::DrawText(slotPos + Vector2(5.0f, 5.0f), text, Color::White);
//	}
//}

// 2025-06-10 주석
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
//		// 슬롯 오브젝트 생성
//		CGameObject* pSlot = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//		pSlot->SetName(L"InventorySlot_" + std::to_wstring(i));
//		pSlot->SetZOrder(1);
//
//		// Transform 세팅
//		auto slotTr = pSlot->AddComponent<CTransform>();
//		slotTr->SetParent(parentTr);
//		slotTr->SetLocalPosition(slotPos);
//		slotTr->SetLocalScale(Vector3(m_fSlotSize, m_fSlotSize, 1.0f));
//
//		// SpriteRenderer 세팅
//		auto pRenderer = pSlot->AddComponent<CSpriteRenderer>();
//		pRenderer->SetMesh(pRectMesh);
//		pRenderer->SetMaterial(pMaterial);
//		pRenderer->Init();
//		//pRenderer->GetMaterial()->SetColor(Color::DarkGray);
//
//		//// 텍스트 UI 생성 및 슬롯 자식으로 붙임
//		//CTextUI* pText = Instantiate<CTextUI>(LAYER_TYPE::LT_UI);
//		//pText->SetText(L"", 12.0f, Color::White);
//		//pText->GetComponent<CTransform>()->SetParent(slotTr);
//		//pText->GetComponent<CTransform>()->SetLocalPosition(Vector3(5.0f, -5.0f, 0.0f));
//
//		// 저장
//		m_vecSlotObjects.push_back(pSlot);
//		//m_vSlotTexts.push_back(pText);
//	}
//}
//void CInventoryUI::OnRender()
//{
//	// 2025-06-09 텍스처 없이 기능만 구현
//	if (m_pInventory == nullptr) return;
//
//	const auto& slots = m_pInventory->GetSlots();
//	size_t maxSlots = m_iRows * m_iColumns;
//	size_t count = std::min(slots.size(), maxSlots);
//
//	// 아이템 있는 슬롯 텍스트 설정
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
//	//// 남은 슬롯 텍스트 클리어
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

// 2025-06-11 현재까지의 문제점
// 기쁨의 영토에서 다시 소멸의 여로로 돌아가면 배경과 음악이 새로 덮여 씌워지는 것 같음
// 기존의 것 위에 새로 덮여 씌워짐(캐릭터는 배경 사이에 존재, 음악은 들어져있는거 + 새로운 거)

// 2025-06-10 까지 사용한 RefreshUI()
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
//		// 아이템 이름으로 텍스처 찾기
//		CTexture* pTex = CResourceManager::Find<CTexture>(item.itemName);
//		if (pTex)
//		{
//			pRenderer->SetTexture(pTex);
//			pSlotObj->SetActive(true);
//		}
//		else
//		{
//			// 텍스처가 없는 경우 빈 슬롯 처리
//			pRenderer->SetTexture(nullptr);
//			pSlotObj->SetActive(true);
//		}
//	}
//
//	// 나머지 슬롯은 비활성화
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

// 2025-06-12 OnInit() 수정하기 전.
//void CInventoryUI::OnInit()
//{
//	CTransform* parentTr = this->GetComponent<CTransform>();
//	Vector3 basePos = parentTr->GetLocalPosition();
//
//	CMesh* pRectMesh = CResourceManager::Find<CMesh>(L"RectMesh");
//	CMaterial* pMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Default-Material");
//
//	CMesh* pBorderMesh = CResourceManager::Find<CMesh>(L"RectMesh");
//	CMaterial* pBorderMaterial = CResourceManager::Find<CMaterial>(L"Sprite-Border-Material");
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
//		// 슬롯 오브젝트 생성
//		CGameObject* pSlot = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//		pSlot->SetName(L"InventorySlot_" + std::to_wstring(i));
//		pSlot->SetZOrder(1);
//
//		// Transform 세팅
//		auto slotTr = pSlot->AddComponent<CTransform>();
//		slotTr->SetParent(parentTr);
//		slotTr->SetLocalPosition(slotPos);
//		slotTr->SetLocalScale(Vector3(m_fSlotSize, m_fSlotSize, 1.0f));
//
//		// SpriteRenderer 세팅
//		auto pRenderer = pSlot->AddComponent<CSpriteRenderer>();
//		pRenderer->SetMesh(pBorderMesh);
//		pRenderer->SetMaterial(pBorderMaterial);
//		pRenderer->Init();
//
//		// 저장
//		m_vecSlotObjects.push_back(pSlot);
//	}
//}

// 2025-06-12 문제점
// 1. 인벤토리가 처음엔 20개지만 i키를 눌러 활성화 할때마다 인벤토리 20칸씩 늘어남
// 1.1 다만 실제 렌더링 되는 인벤토리는 20칸으로만 보여짐.
// -> 활성화 비활성화 해도 슬릇 수는 똑같이 20칸으로 고정됨.
// -> 소멸자나 Init에서 Destroy를 하는 것이 왜 20칸으로 고정이 되는지?
// -> 기존 코드에서 i키로 인벤토리를 열 때마다 OnInit()이 매번 호출되었고, 기존 오브젝트는 삭제하지 않아
// 기존 오브젝트에 새로운 슬롯만 추가되어 갯수가 증가했던 것.
// 따라서 기존 슬릇을 삭제하면 OnInit을 호출할 때마다 똑같이 20칸으로 고정이 되는 것.
// 그렇다면 드는 의문? 아이템을 습득하면 인벤토리 슬릇에 저장되어 있을텐데 아이템도 같이 사라지는 것이 아니냐?
// 아이템 정보는 CInventory의 m_Slots에 보관되어 있고, CInventoryUI는 사실상 화면만 그릴 뿐이다.
// 간단하게 생각해서 CInventory = 데이터베이스 / CInventoryUI = 데이터베이스를 보여주는 웹페이지 이렇게 생각하면 편하다.
// 2. DummyTexture말고 기존 텍스처들이 번갈아 가면서 들어감.
// 2.1 아이템을 습득하고 습득한 맵에서는 안보이지만, 이전 맵으로 돌아오면 모든 텍스처가 그 아이템으로 바뀜
// -> 가장 멍청한 짓. ResourceManager의 Init에 추가하고 core에서 Init을 호출하지 않앗으니, 당연히 보이지 않음.
// core에서 ResourceManager::Init()호출로 인해 수정 완료.
// 2.2 Dummy 텍스처로 만드는 것은 좋은데 인벤토리 칸이 확실하게 보이게 하기 위해 Border을 사용했는데 보이지 않음.
// 
// 3. 여로에서 기쁨의 영토로 갔다가 다시 여로로 돌아오고 다시 기쁨의 영토로 돌아가면 화면이 2개씩 겹쳐져서 나옴.
// 3.1 이거 수정해야 할 것.