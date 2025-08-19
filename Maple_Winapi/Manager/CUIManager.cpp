#include "../pch.h"
#include "CUIManager.h"
#include "../Object/CUIHUD.h"
#include "../Object/CBtnUI.h"
#include "../Object/CObject.h"
#include "../Object/CMiniMap.h"
#include "../Object/CInventoryUI.h"
#include "../Object/CMentBox.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Core/CCore.h"
#include "../Component/CSpriteRenderer.h"

extern CCore core;

unordered_map<UI_TYPE, CUI*> CUIManager::m_mapUI = {};
stack<CUI*> CUIManager::m_stUI = {};
queue<UI_TYPE> CUIManager::m_quRequestUIQueue = {};
CUI* CUIManager::m_pActiveUI = nullptr;
wstring CUIManager::m_strPendingText = L"";
CQuest* CUIManager::m_pPendingQuest = nullptr;

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::Init()
{
	//if (m_mapUI.find(UI_TYPE::UT_Button) == m_mapUI.end())
	//{
	//	// UI 객체 생성해주기
	//	CBtnUI* uiBtn = new CBtnUI();
	//	m_mapUI.insert(make_pair(UI_TYPE::UT_Button, uiBtn));
	//}

	//if (m_mapUI.find(UI_TYPE::UT_MiniMap) == m_mapUI.end())
	//{
	//	// UI 객체 생성해주기
	//	CMiniMap* uiMiniMap = new CMiniMap();
	//	m_mapUI.insert(make_pair(UI_TYPE::UT_MiniMap, uiMiniMap));
	//}
}

void CUIManager::Update()
{
	stack<CUI*> ui = m_stUI;

	while (!ui.empty())
	{
		CUI* uiBase = ui.top();
		if (uiBase)
		{
			uiBase->Update();
			ui.pop();
		}
	}

	if (m_quRequestUIQueue.size() > 0)
	{
		UI_TYPE requestUI = m_quRequestUIQueue.front();
		m_quRequestUIQueue.pop();
		OnLoad(requestUI);
	}

	if (KEY_TAP(KEY_CODE::I))
	{
		if (IsActive(UI_TYPE::UI_Inventory))
		{
			Pop(UI_TYPE::UI_Inventory);
		}
		else
		{
			Push(UI_TYPE::UI_Inventory);
		}
	}
}

void CUIManager::LateUpdate()
{
	stack<CUI*> ui = m_stUI;

	while (!ui.empty())
	{
		CUI* uiBase = ui.top();
		if (uiBase)
		{
			uiBase->LateUpdate();
			ui.pop();
		}
	}
}

void CUIManager::Render()
{
	if (m_stUI.size() <= 0)
		return;

	vector<CUI*> buff;

	CUI* ui = nullptr;
	while (m_stUI.size() > 0)
	{
		ui = m_stUI.top();
		m_stUI.pop();

		buff.push_back(ui);
	}

	sort(buff.begin(), buff.end(),
		[](CUI* a, CUI* b) {
		return a->GetZOrder() < b->GetZOrder(); // 낮은 zOrder 먼저 → 나중에 높은 zOrder 위로 렌더
	});

	//reverse(buff.begin(), buff.end());

	for (CUI* ui : buff)
	{
		if (ui->IsEnable()) // 이 조건 추가
		{
			ui->Render();
			m_stUI.push(ui);
		}
	}
}

void CUIManager::OnLoad(UI_TYPE _eUIType)
{
	unordered_map<UI_TYPE, CUI*>::iterator iter = m_mapUI.find(_eUIType);

	if (iter == m_mapUI.end())
	{
		// 만약 등록 안된 경우 여기서 인스턴스를 생성해도 됨
		if (_eUIType == UI_TYPE::UI_Inventory)
		{
			CInventoryUI* pInventoryUI = Instantiate<CInventoryUI>(LAYER_TYPE::LT_UI);
			pInventoryUI->SetInventory(CInventory::GetInst());
			RegisterUI(_eUIType, pInventoryUI);
			OnComplete(pInventoryUI);
			return;
		}
		// 2025-06-24 Npc MentBox 추가
		else if (_eUIType == UI_TYPE::UT_MentBox)
		{
			CMentBox* pMentBox = Instantiate<CMentBox>(LAYER_TYPE::LT_UI);
			pMentBox->SetName(L"NpcMentBox"); // 2025-07-09 나중에 해당 Npc를 가져와 그 Npc의 이름과 합쳐보기
			pMentBox->OnInit();

			// 저장된 텍스트 설정
			if (!m_strPendingText.empty())
			{
				pMentBox->SetText(m_strPendingText);
				m_strPendingText.clear(); // 사용 후 초기화
				OutputDebugStringA("MentBox text set from pending text\n");
			}
			else
			{
				// 기본 텍스트 설정
				pMentBox->SetText(L"안녕하세요!");
				OutputDebugStringA("MentBox set with default text\n");
			}

			// 2025-06-26
			if (m_pPendingQuest != nullptr)
			{
				pMentBox->SetQuest(m_pPendingQuest);
				m_pPendingQuest = nullptr;
				OutputDebugStringA("MentBox quest set from pending quest\n");
			}

			//// 최종 Transform 상태 확인
			//Vector3 finalPos = tr->GetLocalPosition();
			//Vector3 finalScale = tr->GetLocalScale();
			//wchar_t finalMsg[256];
			//swprintf_s(finalMsg, 256, L"UIManager - Final MentBox Transform: Pos(%.2f, %.2f), Scale(%.2f, %.2f)\n",
			//	finalPos.x, finalPos.y, finalScale.x, finalScale.y);
			//OutputDebugString(finalMsg);

			RegisterUI(_eUIType, pMentBox);
			OnComplete(pMentBox);
			return;
		}

		OnFail();
		return;
	}

	OnComplete(iter->second);
}

void CUIManager::OnComplete(CUI* _pAddUI)
{
	if (_pAddUI == nullptr)
		return;

	_pAddUI->Init();
	_pAddUI->Active();
	_pAddUI->Update();

	// 만약에 현재 추가된 ui가 전체화면이라면
		// 전체화면인 ui 말고 나머지를 전부 비활성화
	if (_pAddUI->IsFullScreen())
	{
		std::stack<CUI*> uiBases = m_stUI;
		while (!uiBases.empty())
		{
			CUI* uiBase = uiBases.top();
			uiBases.pop();
			if (uiBase)
			{
				uiBase->InActive();
			}
		}
	}
	m_stUI.push(_pAddUI);
	m_pActiveUI = nullptr;
}

void CUIManager::OnFail()
{
	m_pActiveUI = nullptr;
}

void CUIManager::Release()
{
	for (auto iter : m_mapUI)
	{
		delete iter.second;
		iter.second = nullptr;
	}
}

void CUIManager::Push(UI_TYPE _eUIType)
{
	m_quRequestUIQueue.push(_eUIType);
}

void CUIManager::Pop(UI_TYPE _eUIType)
{
	wchar_t szLog[256];
	swprintf_s(szLog, L"Pop request: %d\n", static_cast<int>(_eUIType));
	OutputDebugStringW(szLog);

	if (m_stUI.empty())
		return;

	stack<CUI*> tempStack;
	CUI* targetUI = nullptr;

	while (!m_stUI.empty())
	{
		CUI* ui = m_stUI.top();
		m_stUI.pop();

		swprintf_s(szLog, L"Checking UI in stack: %d\n", static_cast<int>(ui->GetUIType()));
		OutputDebugStringW(szLog);

		if (ui->GetUIType() == _eUIType && !targetUI)
		{
			//OutputDebugStringW(L"Found target UI. Clearing it.\n");
			targetUI = ui;
			continue;
		}

		tempStack.push(ui);
	}

	if (targetUI)
	{
		targetUI->UIClear();

		if (targetUI->IsFullScreen())
		{
			stack<CUI*> tempRestore = tempStack;
			while (!tempRestore.empty())
			{
				CUI* ui = tempRestore.top();
				tempRestore.pop();

				if (ui)
				{
					//OutputDebugStringW(L"Re-activating previous UI below fullscreen UI.\n");
					ui->InActive();
					break;
				}
			}
		}
	}

	while (!tempStack.empty())
	{
		m_stUI.push(tempStack.top());
		tempStack.pop();
	}

	//OutputDebugStringW(L"Pop() complete.\n");
}

bool CUIManager::IsActive(UI_TYPE _eUIType)
{
	stack<CUI*> tempStack = m_stUI;

	while (!tempStack.empty())
	{
		CUI* pUI = tempStack.top();
		tempStack.pop();

		if (pUI && pUI->GetUIType() == _eUIType)
			return true;
	}

	return false;
}

CUI* CUIManager::GetUI(UI_TYPE _eUIType)
{
	auto iter = m_mapUI.find(_eUIType);
	if (iter != m_mapUI.end())
	{
		return iter->second;
	}

	return nullptr;
}

void CUIManager::RegisterUI(UI_TYPE _eUIType, CUI* _pUI)
{
	if (m_mapUI.find(_eUIType) == m_mapUI.end())
		m_mapUI.insert(make_pair(_eUIType, _pUI));
}

void CUIManager::PushWithText(UI_TYPE _eUIType, const wstring& _text, CQuest* _pQuest)
{
	m_strPendingText = _text;
	m_pPendingQuest = _pQuest;
	m_quRequestUIQueue.push(_eUIType);
}

void CUIManager::PushWithQuest(UI_TYPE _eUIType, const wstring& _text, CQuest* _pQuest)
{
	CMentBox* pMentBox = dynamic_cast<CMentBox*>(GetUI(_eUIType));
	if (pMentBox)
	{
		pMentBox->SetText(_text);
		pMentBox->SetQuest(_pQuest);
		Push(_eUIType);
	}
}

// 2025-06-10 Pop함수 원본
//void CUIManager::Pop(UI_TYPE _eUIType)
//{
//	//if (m_stUI.size() <= 0)
//	//	return;
//
//	//// 해당 ui 한개만 스택에서
//	//stack<CUI*> tempStack;
//
//	//CUI* uiBase = nullptr;
//	//while (m_stUI.size() > 0)
//	//{
//	//	uiBase = m_stUI.top();
//	//	m_stUI.pop();
//
//	//	if (uiBase->GetUIType() != _eUIType)
//	//	{
//	//		tempStack.push(uiBase);
//	//		continue;
//	//	}
//
//	//	if (uiBase->IsFullScreen())
//	//	{
//	//		stack<CUI*> uiBases = m_stUI;
//	//		while (!uiBases.empty())
//	//		{
//	//			CUI* uiBase = uiBases.top();
//	//			uiBases.pop();
//	//			if (uiBase)
//	//			{
//	//				uiBase->Active();
//	//				break;
//	//			}
//	//		}
//	//	}
//	//	uiBase->UIClear();
//	//}
//
//	//while (tempStack.size() > 0)
//	//{
//	//	uiBase = tempStack.top();
//	//	tempStack.pop();
//	//	m_stUI.push(uiBase);
//	//}
//}
