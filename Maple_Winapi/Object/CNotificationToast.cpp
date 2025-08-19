#include "CNotificationToast.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CUIManager.h"

CNotificationToast::CNotificationToast() :
	CUI(UI_TYPE::UT_NotificationToast)
{
	// �˸� ������ Quest��� Quest �̹��� ���
}

CNotificationToast::~CNotificationToast()
{
}

void CNotificationToast::OnInit()
{
}

void CNotificationToast::OnActive()
{
}

void CNotificationToast::OnInActive()
{
}

void CNotificationToast::OnUpdate()
{
	m_fElapsed += CTimeManager::GetfDeltaTime();

	if (m_fElapsed >= m_fLifeTime)
	{
		SetActive(false);
		CUIManager::Pop(UI_TYPE::UT_NotificationToast);
	}

	CUI::OnUpdate();
}

void CNotificationToast::OnLateUpdate()
{
}

void CNotificationToast::OnRender()
{
	CUI::OnRender();
}

void CNotificationToast::OnClear()
{
}
