#include "CBtnUI.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CSceneManager.h"
#include "../Component/CTransform.h"
#include "../Object/CGameObject.h"
#include "../Component/CRenderer.h"
#include "../Component/CComponent.h"
#include "../Scene/CScene.h"
#include "../Manager/CResourceManager.h"
#include "../Core/CCore.h"
#include "../Component/CSpriteRenderer.h"

extern CCore core;

CBtnUI::CBtnUI() :
	CUI(UI_TYPE::UT_Button)
{
	CTexture* pBtnOK = CResourceManager::Load<CTexture>(L"BtnOK", L"../Resources/Texture/NPC/Quest_BtOK.png");
	GetTransform()->SetLocalPosition(Vector3(200.0f, 100.0f, -1.5f));
	GetTransform()->SetLocalScale(Vector3(57.0f, 17.0f, -1.5f));
}

CBtnUI::~CBtnUI()
{
}

void CBtnUI::OnInit()
{
	//SetPos(Vector2(200.0f, 200.0f));
	//SetSize(Vector2(200.0f, 200.0f));

	CUI::OnInit();

	Vector2 setPos = GetUILocalPos();
	Vector3 worldPos = GetTransform()->GetWorldPosition();
	Vector3 worldSclae = GetTransform()->GetWorldScale();
	char dbg[128];
	sprintf_s(dbg, "Width: %d, Height: %d\n", core.GetWidth(), core.GetHeight());
	OutputDebugStringA(dbg);

	char buf[128];
	sprintf_s(buf, "Actual Btn LocalPos: (%.2f, %.2f)\n", setPos.x, setPos.y);
	OutputDebugStringA(buf);
	sprintf_s(buf, "Actual Btn WorldPos: (%.2f, %.2f)\n", worldPos.x, worldPos.y);
	OutputDebugStringA(buf);
	sprintf_s(buf, "Actual Btn WorldScale: (%.2f, %.2f)\n", worldSclae.x, worldSclae.y);
	OutputDebugStringA(buf);

	CSpriteRenderer* renderer = GetComponent<CSpriteRenderer>();
	if (!renderer)
		renderer = AddComponent<CSpriteRenderer>();

	// 텍스처 로드
	CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"BtnOK");
	if (!pMentBoxTex)
		pMentBoxTex = CResourceManager::Load<CTexture>(L"BtnOK", L"../Resources/Texture/NPC/Quest_BtOK.png");

	if (pMentBoxTex)
	{
		renderer->SetTexture(pMentBoxTex);
		renderer->SetScreenSpace(true);
	}
	else
	{
		OutputDebugStringA("ERROR: MentBox texture could not be loaded\n");
	}

	renderer->Init();

	m_tOnClick = bind(&CBtnUI::ButtonClick, this);
}

void CBtnUI::OnActive()
{
	SetActive(true);
}

void CBtnUI::OnInActive()
{
	SetActive(false);
}

void CBtnUI::OnUpdate()
{
	Vector3 setPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(Vector3(setPos.x, setPos.y, 0.0f));

	ClickUpdate();

	Vector2 mousePos = CKeyManager::GetMousePos();

	/*if (m_vPosition.x <= mousePos.x && mousePos.x <= m_vPosition.x + m_vSize.x
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
	}*/
}

void CBtnUI::OnLateUpdate()
{
}

void CBtnUI::OnRender()
{
	CUI::OnRender();
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

void CBtnUI::ClickUpdate()
{
	if (KEY_TAP(KEY_CODE::LBTN))
	{
		if (ButtonClickEndEvent != nullptr)
		{
			ButtonClickEndEvent();
		}
	}
}

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