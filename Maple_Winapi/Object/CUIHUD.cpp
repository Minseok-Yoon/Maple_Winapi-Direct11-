#include "CUIHUD.h"
#include "../Manager/CResourceManager.h"

CUIHUD::CUIHUD() :
	CUI(UI_TYPE::UT_HpBar)
{
}

CUIHUD::~CUIHUD()
{
}

void CUIHUD::OnInit()
{
	//m_pTexture = CResourceManager::Find<CTexture>(L"HPBAR");
}

void CUIHUD::OnActive()
{
}

void CUIHUD::OnInActive()
{
}

void CUIHUD::OnUpdate()
{
}

void CUIHUD::OnLateUpdate()
{
}

void CUIHUD::OnRender()
{
	/*TransparentBlt(_Hdc,
		0, 0,
		m_pTexture->GetWidth(),
		m_pTexture->GetHeight(),
		m_pTexture->GetHdc(),
		0, 0,
		m_pTexture->GetWidth() / 2.0f,
		m_pTexture->GetHeight(),
		RGB(255, 0, 255));*/
}

void CUIHUD::OnClear()
{
}
