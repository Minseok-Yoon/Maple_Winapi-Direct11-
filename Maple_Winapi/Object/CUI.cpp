#include "../pch.h"
#include "CUI.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CCamera.h"
#include "../Module/SelectGDI.h"
#include "../Scene/CScene.h"

CUI::CUI(UI_TYPE _eUIType) :
	m_eUIType(_eUIType),
	m_bMouseOn(false),
	m_bEnable(false)
{
}

CUI::~CUI()
{
}

void CUI::Init()
{
	OnInit();
}

void CUI::Update()
{
	if (m_bEnable)
		OnUpdate();
}

void CUI::LateUpdate()
{
	if (m_bEnable)
		OnLateUpdate();
}

void CUI::Render()
{
	if (m_bEnable)
		OnRender();
}

void CUI::Active()
{
	m_bEnable = true;
	OnActive();
}

void CUI::InActive()
{
	m_bEnable = false;
	OnInActive();
}

void CUI::UIClear()
{
	InActive();
	OnClear();
}

void CUI::OnInit()
{
}

void CUI::OnActive()
{
}

void CUI::OnInActive()
{
}

void CUI::OnUpdate()
{
}

void CUI::OnLateUpdate()
{
}

void CUI::OnRender()
{
}

void CUI::OnClear()
{
}