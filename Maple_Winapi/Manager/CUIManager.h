#pragma once
#include "../pch.h"
#include "../Object/CUI.h"

class CQuest;
class CUIManager
{
	friend class CMentBox;

	SINGLETON(CUIManager);

public:
	static void Init();
	static void Update();
	static void LateUpdate();
	static void Render();
	static void OnLoad(UI_TYPE _eUIType);
	static void OnComplete(CUI* _pAddUI);
	static void OnFail();
	static void Release();

	static void Push(UI_TYPE _eUIType);
	static void Pop(UI_TYPE _eUIType);
	static bool IsActive(UI_TYPE _eUIType);

	static CUI* GetUI(UI_TYPE _eUIType);

	static void RegisterUI(UI_TYPE _eUIType, CUI* _pUI);

	// 2025-06-24
	static void PushWithText(UI_TYPE _eUIType, const wstring& _text, CQuest* _pQuest);

	// 2025-06-26
	static void PushWithQuest(UI_TYPE _eUIType, const wstring& _text, CQuest* _pQuest);

private:
	static unordered_map<UI_TYPE, CUI*>	m_mapUI;
	static stack<CUI*>					m_stUI;
	static queue<UI_TYPE>				m_quRequestUIQueue;
	static CUI* m_pActiveUI;

	// 2025-06-24
	static wstring m_strPendingText;

	// 2025-06-26
	static CQuest* m_pPendingQuest;
};