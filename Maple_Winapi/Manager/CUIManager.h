#pragma once
#include "../pch.h"
#include "../Object/CUI.h"

class CUIManager
{
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

	static CUI* GetUI(UI_TYPE _eUIType);

private:
	static unordered_map<UI_TYPE, CUI*>	m_mapUI;
	static stack<CUI*>					m_stUI;
	static queue<UI_TYPE>				m_quRequestUIQueue;
	static CUI* m_pActiveUI;
};