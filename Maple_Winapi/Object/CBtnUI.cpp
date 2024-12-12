#include "CBtnUI.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CSceneManager.h"
#include "../Component/CTransform.h"
#include "../Object/CGameObject.h"
#include "../Component/CRenderer.h"
#include "../Component/CComponent.h"
#include "../Scene/CScene.h"

CBtnUI::CBtnUI() :
	CUI(UI_TYPE::UT_Button)
{
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::OnInit()
{
	SetPos(Vector2(200.0f, 200.0f));
	SetSize(Vector2(200.0f, 200.0f));

	m_tOnClick = bind(&CBtnUI::ButtonClick, this);
}

void CBtnUI::OnActive()
{
}

void CBtnUI::OnInActive()
{
}

void CBtnUI::OnUpdate()
{
	Vector2 mousePos = CKeyManager::GetMousePos();

	if (m_vPosition.x <= mousePos.x && mousePos.x <= m_vPosition.x + m_vSize.x
		&& m_vPosition.y <= mousePos.y && mousePos.y <= m_vPosition.y + m_vSize.y)
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}

	if (KEY_TAP(KEY_CODE::LBTN))
	{
		if (m_bMouseOn)
			m_tOnClick();
	}
}

void CBtnUI::OnLateUpdate()
{
}

void CBtnUI::OnRender()
{
	/*if (m_pTexture == nullptr)
		assert(false);

	int width = m_pTexture->GetWidth();
	int height = m_pTexture->GetHeight();
	Vector2 vPos = GetBtnPosition();

	if (m_pTexture->GetTextureState() == TEXTURE_STATE::TS_Bmp)
	{
		TransparentBlt(_Hdc, vPos.x, vPos.y,
			width,
			height,
			m_pTexture->GetHdc(),
			0, 0,
			m_pTexture->GetWidth(),
			m_pTexture->GetHeight(),
			RGB(255, 0, 255));
	}
	else if (m_pTexture->GetTextureState() == TEXTURE_STATE::TS_Png)
	{
		Gdiplus::Graphics graphcis(_Hdc);
		graphcis.DrawImage(m_pTexture->GetImage(),
			Gdiplus::Rect(vPos.x, vPos.y,
				m_pTexture->GetWidth(),
				m_pTexture->GetHeight()));
	}
	Rectangle(_Hdc,
		(int)m_vPosition.x, (int)m_vPosition.y,
		m_vPosition.x + m_vSize.x,
		m_vPosition.y + m_vSize.y);*/
}

void CBtnUI::OnClear()
{
}

void CBtnUI::ButtonClick()
{
	CScene* curScene = CSceneManager::GetCurScene();
	if (curScene != nullptr && curScene->GetName() == L"StartScene")
	{
		CSceneManager::ChangeScene(L"Stage01", L"Stage01_BG", L"BGSound1");
	}
}