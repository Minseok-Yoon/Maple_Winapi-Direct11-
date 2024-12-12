#include "CUIManager.h"
#include "../Object/CUIHUD.h"
#include "../Object/CBtnUI.h"

unordered_map<UI_TYPE, CUI*> CUIManager::m_mapUI = {};
stack<CUI*> CUIManager::m_stUI = {};
queue<UI_TYPE> CUIManager::m_quRequestUIQueue = {};
CUI* CUIManager::m_pActiveUI = nullptr;

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

void CUIManager::Init()
{
	if (m_mapUI.find(UI_TYPE::UT_Button) == m_mapUI.end())
	{
		// UI 객체 생성해주기
		CBtnUI* uiBtn = new CBtnUI();
		m_mapUI.insert(make_pair(UI_TYPE::UT_Button, uiBtn));
	}
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

	reverse(buff.begin(), buff.end());

	for (CUI* ui : buff)
	{
		ui->Render();
		m_stUI.push(ui);
	}
}

void CUIManager::OnLoad(UI_TYPE _eUIType)
{
	unordered_map<UI_TYPE, CUI*>::iterator iter = m_mapUI.find(_eUIType);

	if (iter == m_mapUI.end())
	{
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
	if (m_stUI.size() <= 0)
		return;

	// 해당 ui 한개만 스택에서
	stack<CUI*> tempStack;

	CUI* uiBase = nullptr;
	while (m_stUI.size() > 0)
	{
		uiBase = m_stUI.top();
		m_stUI.pop();

		if (uiBase->GetUIType() != _eUIType)
		{
			tempStack.push(uiBase);
			continue;
		}

		if (uiBase->IsFullScreen())
		{
			stack<CUI*> uiBases = m_stUI;
			while (!uiBases.empty())
			{
				CUI* uiBase = uiBases.top();
				uiBases.pop();
				if (uiBase)
				{
					uiBase->Active();
					break;
				}
			}
		}
		uiBase->UIClear();
	}

	while (tempStack.size() > 0)
	{
		uiBase = tempStack.top();
		tempStack.pop();
		m_stUI.push(uiBase);
	}
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