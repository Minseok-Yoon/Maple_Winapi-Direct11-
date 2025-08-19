#include "../pch.h"
#include "../Object/CObject.h"
#include "CMentBox.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CFontManager.h"
#include "../Component/CUITransform.h"
#include "../Component/CSpriteRenderer.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CQuestManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Quest/CQuest.h"
#include "../Object/CPlayer.h"
#include "../Core/CCore.h"
#include "CUIText.h"
#include "CBtnUI.h"

extern CCore core;

CMentBox::CMentBox() :
	CUI(UI_TYPE::UT_MentBox)
{
	CTexture* pMentBox = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
	CTexture* pBtnOK = CResourceManager::Load<CTexture>(L"BtnOK", L"../Resources/Texture/NPC/Quest_BtOK.png");

	float screenWidth = (float)core.GetWidth();
	float screenHeight = (float)core.GetHeight();
	SetUILocalPos(Vector2(screenWidth * 0.5f, screenHeight * 0.5f));// �̷��� �������� �� ���� ������ ���� �����Ѵٴ� ���� ���� ��ũ�� ��ǥ�� �ƴ� ���� ��ǥ��� ��.
	SetUILocalScale(Vector2(600.0f, 200.0f));
}

CMentBox::~CMentBox()
{
}

void CMentBox::OnInit()
{
	CUI::OnInit();

	// �����: Transform ���� �� �� Ȯ��
	Vector2 setPos = GetUILocalPos();
	Vector3 worldPos = GetTransform()->GetWorldPosition();
	char dbg[128];
	sprintf_s(dbg, "Width: %d, Height: %d\n", core.GetWidth(), core.GetHeight());
	OutputDebugStringA(dbg);

	char buf[128];
	sprintf_s(buf, "Actual LocalPos: (%.2f, %.2f)\n", setPos.x, setPos.y);
	OutputDebugStringA(buf);
	sprintf_s(buf, "Actual WorldPos: (%.2f, %.2f)\n", worldPos.x, worldPos.y);
	OutputDebugStringA(buf);

	// SpriteRenderer ����
	CSpriteRenderer* renderer = GetComponent<CSpriteRenderer>();
	if (!renderer)
		renderer = AddComponent<CSpriteRenderer>();

	// �ؽ�ó �ε�
	CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
	if (!pMentBoxTex)
		pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");

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

	// �ݶ��̴� (���û���)
	if (!m_pMentBoxCol) {
		m_pMentBoxCol = AddComponent<CCollider>();
		if (m_pMentBoxCol) {
			m_pMentBoxCol->SetBottomLeft(Vector3(-0.5f, -0.5f, -1.0f));
			m_pMentBoxCol->SetTopRight(Vector3(0.5f, 0.5f, -1.0f));
		}
	}
	
	// 2025-07-17
	// �ؽ�Ʈ UI �ڽ� ����
	if (CUIManager::GetUI(UI_TYPE::UT_Text) == nullptr)
	{
		m_pTextUI = Instantiate<CUIText>(LAYER_TYPE::LT_UI);
		m_pTextUI->SetRelativePos(Vector2(-200.0f, -10.0f));
		m_pTextUI->Init();

		AddChild(m_pTextUI);
		CUIManager::RegisterUI(UI_TYPE::UT_Text, m_pTextUI);
		CUIManager::OnComplete(m_pTextUI);
	}

	if (CUIManager::GetUI(UI_TYPE::UT_Button) == nullptr)
	{
		m_pBtnUI = Instantiate<CBtnUI>(LAYER_TYPE::LT_UI);

		AddChild(m_pBtnUI); // �ڽ����� ���� �߰��ؾ� transform ����� ����� �۵���

		// Transform �� ��ġ ������ AddChild �Ŀ� �ؾ� �ݿ���
		m_pBtnUI->SetRelativePos(Vector2(200.0f, -60.0f));
		//m_pBtnUI->GetTransform()->SetWorldPosition(Vector3(0.0f, 0.0f, -0.5f));
		m_pBtnUI->Init();

		// ��ư�� SpriteRenderer ����
		CSpriteRenderer* BtnSr = m_pBtnUI->GetComponent<CSpriteRenderer>();
		if (!BtnSr)
			BtnSr = m_pBtnUI->AddComponent<CSpriteRenderer>();

		CTexture* pBtnTex = CResourceManager::Find<CTexture>(L"BtnOK");
		if (!pBtnTex)
			pBtnTex = CResourceManager::Load<CTexture>(L"BtnOK", L"../Resources/Texture/NPC/Quest_BtOK.png");

		if (pBtnTex)
		{
			BtnSr->SetTexture(pBtnTex);
			BtnSr->SetScreenSpace(true);
		}
		else
		{
			OutputDebugStringA("ERROR: BtnOK texture could not be loaded\n");
		}

		BtnSr->Init();
		m_pBtnUI->SetActive(true);

		CUIManager::RegisterUI(UI_TYPE::UT_Button, m_pBtnUI);
		CUIManager::OnComplete(m_pBtnUI);

		/*m_pBtnUI->SetButtonClickEndEvent([&]()
		{
			OnInActive();
		});*/
	}
}

void CMentBox::OnActive()
{
	SetActive(true);
}

void CMentBox::OnInActive()
{
	SetActive(false);
}

void CMentBox::OnUpdate()
{
	Vector3 setPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(Vector3(setPos.x, setPos.y, 0.0f));

	if (m_charIndex < m_strFullText.length())
	{
		m_fAccTime += CTimeManager::GetfDeltaTime();
		if (m_fAccTime >= m_fTypingSpeed)
		{
			m_strCurrentText += m_strFullText[m_charIndex++];
			m_fAccTime = 0.0f;

			if (m_pTextUI)
				m_pTextUI->SetText(m_strCurrentText);
		}
	}

	if (KEY_TAP(KEY_CODE::SPACE)) OnAcceptQuest();
	if (KEY_TAP(KEY_CODE::ESC)) CUIManager::Pop(UI_TYPE::UT_MentBox);

	// �ؽ�Ʈ ��� ó��
	//if (m_charIndex < m_strFullText.length())
	//{
	//	m_fAccTime += CTimeManager::GetfDeltaTime();

	//	if (m_fAccTime >= m_fTypingSpeed)
	//	{
	//		m_strCurrentText += m_strFullText[m_charIndex];
	//		m_charIndex++;
	//		m_fAccTime = 0.0f;
	//	}
	//}
	//else if (KEY_TAP(KEY_CODE::SPACE))
	//{
	//	OutputDebugStringA("����Ʈ space�� ����\n");
	//	OnAcceptQuest();
	//}

	//// ESC ������ �� UI �ݱ�
	//if (KEY_TAP(KEY_CODE::ESC))
	//{
	//	if (CUIManager::IsActive(UI_TYPE::UT_MentBox))
	//	{
	//		CUIManager::Pop(UI_TYPE::UT_MentBox);
	//	}
	//}
}

void CMentBox::OnLateUpdate()
{
}

void CMentBox::OnRender()
{
	CUI::OnRender();

	ID3D11DeviceContext* context = GetDeviceContext();
	if (!context) return;

	// Scissor ���� ����
	D3D11_RECT scissor;
	Vector3 pos = GetTransform()->GetWorldPosition();
	Vector3 size = GetTransform()->GetLocalScale();

	scissor.left = static_cast<LONG>(pos.x);
	scissor.top = static_cast<LONG>(pos.y);
	scissor.right = static_cast<LONG>(pos.x + size.x);
	scissor.bottom = static_cast<LONG>(pos.y + size.y);

	context->RSSetScissorRects(1, &scissor);

	// �ڽ� ��� ����
	for (CUI* child : GetChildren())
	{
		if (child && child->IsEnable())
			child->Render();
	}
}

void CMentBox::OnClear() 
{
}

void CMentBox::SetText(const wstring& _strText)
{
	if (m_pTextUI)
	{
		m_pTextUI->SetFullText(_strText); // ���� ����
	}
}

void CMentBox::OnAcceptQuest()
{
	if (m_pQuest)
	{
		CQuestManager::GetInst()->AcceptQuest(m_pQuest);
		OutputDebugStringW((L"������ ����Ʈ: " + m_pQuest->GetTitle() + L"\n").c_str());
		if (CUIManager::IsActive(UI_TYPE::UT_MentBox))
		{
			CUIManager::Pop(UI_TYPE::UT_MentBox);
		}
	}
}

// 2025-06-24
//CFontManager::GetInst()->DrawText(
		//	GetDeviceContext(),  // ����̽� ���ؽ�Ʈ
		//	m_strCurrentText,                  // ����� �ؽ�Ʈ
		//	100.0f, 100.0f,                    // ��� ��ġ
		//	24.0f,                             // ��Ʈ ũ��
		//	0x00000000,                        // ���� (���)
		//	L"Arial"
		//);

//// ȭ�� �߾ӿ� �׽�Ʈ �ؽ�Ʈ ���
		//CFontManager::GetInst()->DrawText(
		//	deviceContext,
		//	m_strCurrentText,
		//	-100.0f, -300.0f,  // ���� ��ġ
		//	15.0f,            // ū ��Ʈ ũ��
		//	0xFFFF0000,       // ������
		//	L"Arial"
		//);

	//// �ؽ�Ʈ ��¿� �Լ� ȣ��
	//if (!m_strCurrentText.empty())
	//{
	//	Vector3 pos = GetComponent<CTransform>()->GetWorldPosition();
	//	ID3D11DeviceContext* deviceContext = GetDeviceContext();

	//	if (deviceContext)
	//	{
	//		CFontManager::GetInst()->DrawText(
	//			deviceContext,
	//			m_strCurrentText,
	//			pos.x + 20.0f, 
	//			pos.y + 20.0f,
	//			24.0f,
	//			0xFFFFFFFF,
	//			L"Arial"
	//		);
	//	}
	//}
//if (m_charIndex < m_strFullText.length())
	//{
	//	m_fAccTime += CTimeManager::GetfDeltaTime();

	//	if (m_fAccTime >= m_fTypingSpeed)
	//	{
	//		m_strCurrentText += m_strFullText[m_charIndex];
	//		m_charIndex++;
	//		m_fAccTime = 0.0f;
	//	}
	//}
	//else
	//{
	//	// ��� �ؽ�Ʈ ��� �Ŀ��� �����̽��� �Է� ����
	//	if (KEY_TAP(KEY_CODE::SPACE))
	//	{
	//		//OnAcceptQuest();
	//	}
	//}

	//CTransform* tr = GetComponent<CTransform>();
	//if(!tr) 
	//	return;

	//Vector3 pos = tr->GetLocalPosition();
	//Vector3 scale = tr->GetLocalScale();

	//Vector2 mousePos = CKeyManager::GetInst()->GetMousePos(); // ���� ���콺 ��ġ
	//Vector2 mouse(mousePos.x, mousePos.y);

	//// ���콺 Ŭ�� üũ
	//if (KEY_TAP(KEY_CODE::LBTN) && KEY_HOLD(KEY_CODE::LBTN))
	//{
	//	// ���콺�� UI ���� �ȿ� �ִ��� Ȯ��
	//	if (mouse.x >= pos.x && mouse.x <= pos.x + scale.x &&
	//		mouse.y >= pos.y && mouse.y <= pos.y + scale.y)
	//	{
	//		m_bDragging = true;
	//		m_vDragOffset = Vector2(mouse.x - pos.x, mouse.y - pos.y);
	//	}
	//}

	//if (KEY_HOLD(KEY_CODE::LBTN) && m_bDragging)
	//{
	//	// �巡�� �� �� ��ġ ����
	//	tr->SetLocalPosition(Vector3(mouse.x - m_vDragOffset.x,
	//		mouse.y - m_vDragOffset.y,
	//		pos.z));
	//}

	//if (KEY_AWAY(KEY_CODE::LBTN))
	//{
	//	m_bDragging = false;
	//}

// 2025-07-11
//void CMentBox::OnInit()
//{
//	CUI::OnInit();
//
//	// Transform ����
//	CTransform* tr = GetComponent<CTransform>();
//	if (!tr)
//		tr = AddComponent<CTransform>();
//
//	tr->SetLocalPosition(Vector3(m_vPosition.x, m_vPosition.y, 0.0f));
//	tr->SetLocalScale(Vector3(m_vSize.x, m_vSize.y, 1.0f));
//
//	// SpriteRenderer ����
//	CSpriteRenderer* renderer = GetComponent<CSpriteRenderer>();
//	if (!renderer)
//		renderer = AddComponent<CSpriteRenderer>();
//
//	// �ؽ�ó �ε�
//	CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//	if (!pMentBoxTex)
//		pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
//
//	if (pMentBoxTex)
//	{
//		renderer->SetTexture(pMentBoxTex);
//	}
//	else
//	{
//		OutputDebugStringA("ERROR: MentBox texture could not be loaded\n");
//	}
//
//	renderer->Init();
//
//	// �ݶ��̴� (���û���)
//	if (!m_pMentBoxCol) {
//		m_pMentBoxCol = AddComponent<CCollider>();
//		if (m_pMentBoxCol) {
//			m_pMentBoxCol->SetBottomLeft(Vector3(-0.5f, -0.5f, -1.0f));
//			m_pMentBoxCol->SetTopRight(Vector3(0.5f, 0.5f, -1.0f));
//		}
//	}
//
//	// ����� ����
//	OutputDebugStringA("MentBox::OnInit completed - Renderer and Transform set\n");
//	//CUI::OnInit();
//
//	////CUITransform* pMentBoxTr = GetComponent<CUITransform>();
//	////if (!pMentBoxTr)
//	////	pMentBoxTr = AddComponent<CUITransform>();
//
//	////CSpriteRenderer* pMentBoxSr = GetComponent<CSpriteRenderer>();
//	////if (!pMentBoxSr)
//	////	pMentBoxSr = AddComponent<CSpriteRenderer>();
//
//	////CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//	////if(!pMentBoxTex)
//	////	pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
//
//	////if (pMentBoxTex)
//	////{
//	////	pMentBoxSr->SetTexture(pMentBoxTex);
//
//	////	// ȭ�� ������ ���� ��ġ ����
//	////	UINT screenWidth = core.GetWidth();
//	////	UINT screenHeight = core.GetHeight();
//
//	////	Vector2 size(300.f, 200.f);
//	////	//Vector2 position(static_cast<float>(screenWidth) * 0.5f, static_cast<float>(screenHeight) * 0.5f);
//	////	Vector2 pivot(0.5f, 0.5f); // �߽� ����
//	////	Vector2 position(1200.0f, 0.0f);
//
//	////	pMentBoxTr->SetSize(size);
//	////	pMentBoxTr->SetPivot(pivot);
//	////	pMentBoxTr->SetPosition(position);
//	////}
//
//	////Vector2 uiPos = pMentBoxTr->GetWorldPosition();
//	////wchar_t buf[128];
//	////swprintf_s(buf, 128, L"Screen Size: %dx%d, UI Pos = (%.2f, %.2f)\n",
//	////	core.GetWidth(), core.GetHeight(), uiPos.x, uiPos.y);
//	////OutputDebugStringW(buf);
//
//	////// ���� ��� Ȯ��
//	////Matrix worldMat = pMentBoxTr->GetWorldMatrix();
//	////wchar_t bufs[256];
//	////swprintf_s(bufs, 256, L"World Matrix: [%.2f, %.2f, %.2f, %.2f]\n",
//	////	worldMat.m[3][0], worldMat.m[3][1], worldMat.m[3][2], worldMat.m[3][3]);
//	////OutputDebugStringW(bufs);
//
//	//CTransform* parentTr = this->GetComponent<CTransform>();
//	//Vector3 basePos = parentTr->GetLocalPosition();
//
//	//Vector3 pMentPos = Vector3(0.0f, 0.0f, 0.0f);
//
//	//CGameObject* pMentBox = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//
//	//// Transform ����
//	//auto slotTr = this->AddComponent<CTransform>();
//	//slotTr->SetParent(parentTr);
//	//slotTr->SetLocalPosition(pMentPos);
//	//slotTr->SetLocalScale(Vector3(600.0f, 200.0f, 1.0f));
//
//	//wchar_t buf[128];
//	//swprintf_s(buf, 128, L"Screen Size: %dx%d, UI Pos = (%.2f, %.2f)\n",
//	//	core.GetWidth(), core.GetHeight(), basePos.x, basePos.y);
//	//OutputDebugStringW(buf);
//
//	//CSpriteRenderer* pMentBoxSr = GetComponent<CSpriteRenderer>();
//	//if (!pMentBoxSr)
//	//	pMentBoxSr = AddComponent<CSpriteRenderer>();
//
//	//// �ؽ�ó �ε�
//	//CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//	//if (!pMentBoxTex)
//	//	pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
//
//	//if (pMentBoxTex)
//	//{
//	//	pMentBoxSr->SetTexture(pMentBoxTex);
//	//}
//
//	//pMentBoxSr->Init();
//
//	//if (!m_pMentBoxCol) {
//	//	m_pMentBoxCol = this->AddComponent<CCollider>();
//	//	if (m_pMentBoxCol) {
//	//		m_pMentBoxCol->SetBottomLeft(Vector3(-0.5f, -0.5f, -1.0f));
//	//		m_pMentBoxCol->SetTopRight(Vector3(0.5f, 0.5f, -1.0f));
//	//		OutputDebugStringA("INFO: Collider successfully added to CMentbox\n");
//	//	}
//	//	else {
//	//		OutputDebugStringA("ERROR: Collider creation failed in CMonster::Init()\n");
//	//	}
//	//}
//
//	//// ������ ���� Ȯ��
//	//OutputDebugStringA("=== MentBox Init Debug ===\n");
//
//	//if (pMentBoxSr) {
//	//	OutputDebugStringA("SpriteRenderer exists\n");
//	//	if (pMentBoxTex) {
//	//		OutputDebugStringA("Texture loaded successfully\n");
//	//	}
//	//	else {
//	//		OutputDebugStringA("ERROR: Texture is NULL\n");
//	//	}
//	//}
//	//else {
//	//	OutputDebugStringA("ERROR: SpriteRenderer is NULL\n");
//	//}
//
//	//// Ȱ��ȭ ���� Ȯ��
//	//if (IsActive()) {
//	//	OutputDebugStringA("MentBox is Active\n");
//	//}
//	//else {
//	//	OutputDebugStringA("WARNING: MentBox is not Active\n");
//	//}
//}

//void CMentBox::OnUpdate()
//{
//	//OutputDebugStringA("MentBox OnUpdate ���� ��\n");
//
//	//GetTransform()->SetLocalPosition(Vector3(m_vLocalPosition.x, m_vPosition.y, 0.f));
//
//	// �ؽ�Ʈ ��� ó��
//	if (m_charIndex < m_strFullText.length())
//	{
//		m_fAccTime += CTimeManager::GetfDeltaTime();
//
//		if (m_fAccTime >= m_fTypingSpeed)
//		{
//			m_strCurrentText += m_strFullText[m_charIndex];
//			m_charIndex++;
//			m_fAccTime = 0.0f;
//		}
//	}
//	else if (KEY_TAP(KEY_CODE::SPACE))
//	{
//		OutputDebugStringA("����Ʈ space�� ����\n");
//		OnAcceptQuest();
//	}
//
//	//// �巡�� ó��
//	//CTransform* tr = GetComponent<CTransform>();
//	//if (!tr) return;
//
//	//Vector3 pos = tr->GetLocalPosition(); // UI ��ǥ (Ŭ���̾�Ʈ ����)
//	//Vector3 scale = tr->GetLocalScale();  // UI ũ�� (Ŭ���̾�Ʈ ����)
//
//	//Vector2 mousePos = CKeyManager::GetInst()->GetMousePos(); // Ŭ���̾�Ʈ ���� ��ǥ
//
//	//// Ŭ�� �� �巡�� ���� �Ǻ�
//	//if (KEY_TAP(KEY_CODE::LBTN))
//	//{
//	//	if (mousePos.x >= pos.x && mousePos.x <= pos.x + scale.x &&
//	//		mousePos.y >= pos.y && mousePos.y <= pos.y + scale.y)
//	//	{
//	//		m_bDragging = true;
//	//		m_vDragOffset = Vector2(mousePos.x - pos.x, mousePos.y - pos.y);
//	//		OutputDebugStringA("Drag ���۵�\n");
//	//	}
//	//}
//
//	//// �巡�� ���̸� ��ġ �̵�
//	//if (KEY_HOLD(KEY_CODE::LBTN) && m_bDragging)
//	//{
//	//	Vector3 newPos(mousePos.x - m_vDragOffset.x, mousePos.y - m_vDragOffset.y, pos.z);
//	//	tr->SetLocalPosition(newPos);
//	//	//SetPos(Vector2(newPos.x, newPos.y)); // UI ����ȭ
//
//	//	OutputDebugStringA("�巡�� ��...\n");
//	//}
//
//	//// �巡�� ����
//	//if (KEY_AWAY(KEY_CODE::LBTN))
//	//{
//	//	if (m_bDragging)
//	//		OutputDebugStringA("�巡�� �����\n");
//	//	m_bDragging = false;
//	//}
//
//	// ESC ������ �� UI �ݱ�
//	if (KEY_TAP(KEY_CODE::ESC))
//	{
//		if (CUIManager::IsActive(UI_TYPE::UT_MentBox))
//		{
//			CUIManager::Pop(UI_TYPE::UT_MentBox);
//		}
//	}
//}

//void CMentBox::OnRender()
//{
//	CUI::OnRender();
//
//	ID3D11DeviceContext* deviceContext = GetDeviceContext();
//	if (!deviceContext || !CFontManager::GetInst()) return;
//
//	// Step 1: ���� Transform ��ġ�� ��������
//	CTransform* tr = GetComponent<CTransform>();
//	if (!tr) return;
//
//	Vector3 worldPos = tr->GetWorldPosition();
//	Vector3 scale = tr->GetWorldScale(); // UI ũ��
//
//	// Step 2: Transform ��ǥ (NDC) �� ��ũ�� �ȼ� ��ǥ ��ȯ
//	Vector2 screenPos;
//	UINT screenWidth = core.GetWidth();
//	UINT screenHeight = core.GetHeight();
//
//	//char dbg[128];
//	//sprintf_s(dbg, "[MentBox::OnRender] MentBox Pos: %.1f, %.1f\n", m_vPosition.x, m_vPosition.y);
//	////sprintf_s(dbg, "[MentBox::OnRender] Core instance: %p\n", CCore::GetInst());
//	//OutputDebugStringA(dbg);
//
//	screenPos.x = (worldPos.x + 1.0f) * 0.5f * screenWidth;
//	screenPos.y = (1.0f - worldPos.y) * 0.5f * screenHeight;
//
//	// Step 3: �ؽ�Ʈ�� �ȼ� ��ǥ ��� (�е� 20px)
//	float textX = screenPos.x + 0.0f;
//	float textY = screenPos.y + 0.0f;
//
//	// ����� ���
//	/*char dbgs[256];
//	sprintf_s(dbgs, "MentBox Screen Pos: (%.2f, %.2f), Text Pos: (%.2f, %.2f)\n",
//		screenPos.x, screenPos.y, textX, textY);
//	OutputDebugStringA(dbgs);*/
//
//	// Step 4: �ؽ�Ʈ ������
//	CFontManager::GetInst()->DrawText(
//		deviceContext,
//		m_strCurrentText,
//		30.0f,
//		textX,
//		textY,
//		0xFF0000FF,  // �Ķ���
//		L"Arial"
//	);
//
//	//ID3D11DeviceContext* deviceContext = GetDeviceContext();
//	//if (!deviceContext || !CFontManager::GetInst()) return;
//
//	///*CTransform* tr = GetComponent<CTransform>();
//	//if (!tr) return;
//
//	//Vector3 pos = tr->GetWorldPosition();
//	//Vector3 size = tr->GetLocalScale();*/
//
//	//Vector3 worldPos = GetTransform()->GetWorldPosition();
//	//Vector3 scale = GetTransform()->GetWorldScale();
//
//	////float offsetX = 80.0f;
//	////float offsetY = 40.0f;
//
//	////// ���� ���޵Ǵ� ��ǥ�� Ȯ��
//	////float finalX = pos.x + offsetX;
//	////float finalY = pos.y + offsetY;
//	//float textX = worldPos.x + m_vTextOffset.x * scale.x;
//	//float textY = worldPos.y + m_vTextOffset.y * scale.y;
//
//	//// OutputDebugString�� ���ڿ� ������
//	///*wchar_t debugMsg[256];
//	//swprintf_s(debugMsg, 256, L"Box pos: (%.2f, %.2f, %.2f), Final text pos: (%.2f, %.2f)\n",
//	//	pos.x, pos.y, pos.z, finalX, finalY);
//	//OutputDebugString(debugMsg);*/
//
//	//CFontManager::GetInst()->DrawText(
//	//	deviceContext,
//	//	m_strCurrentText,
//	//	textX,
//	//	textY,  // offsetY�� ����
//	//	30.0f,
//	//	0xFFFF0000,  // ���
//	//	L"Arial"
//	//);
//}

// 2025-07-17
	//ID3D11DeviceContext* deviceContext = GetDeviceContext();
	//if (!deviceContext || !CFontManager::GetInst()) return;

	//// 1. Transform ��������
	//CTransform* tr = GetComponent<CTransform>();
	//if (!tr) return;

	//Vector3 screenPos = tr->GetLocalPosition(); // �̹� �ȼ� ��ǥ
	//Vector3 scale = tr->GetLocalScale();        // ũ�⵵ �ȼ� ����

	//// 2. �ؽ�Ʈ�� ��ġ = MentBox ���� ��� + �е�
	//float paddingX = 20.0f;
	//float paddingY = 20.0f;

	//float textX = screenPos.x;
	//float textY = screenPos.y;

	//// 3. �ؽ�Ʈ ������
	//CFontManager::GetInst()->DrawText(
	//	deviceContext,
	//	m_strCurrentText,
	//	25.0f,
	//	textX,
	//	textY,
	//	0xFF0000FF,  // �Ķ���
	//	L"Arial"
	//);

	//CUI::OnRender();

	//ID3D11DeviceContext* deviceContext = GetDeviceContext();
	//if (!deviceContext || !CFontManager::GetInst()) return;

	//// Step 1: ���� Transform ��ġ�� ��������
	//CTransform* tr = GetComponent<CTransform>();
	//if (!tr) return;

	//Vector3 worldPos = tr->GetWorldPosition();
	//Vector3 scale = tr->GetWorldScale(); // UI ũ��

	//// Step 2: Transform ��ǥ (NDC) �� ��ũ�� �ȼ� ��ǥ ��ȯ
	//Vector2 screenPos;
	//UINT screenWidth = core.GetWidth();
	//UINT screenHeight = core.GetHeight();

	//screenPos.x = (worldPos.x + 1.0f) * 0.5f * screenWidth;
	//screenPos.y = (1.0f - worldPos.y) * 0.5f * screenHeight;

	//// Step 3: �ؽ�Ʈ�� �ȼ� ��ǥ ��� (�е� 20px)
	//float textX = screenPos.x + 0.0f;
	//float textY = screenPos.y + 0.0f;

	//// Step 4: �ؽ�Ʈ ������
	//CFontManager::GetInst()->DrawText(
	//	deviceContext,
	//	m_strCurrentText,
	//	30.0f,
	//	textX,
	//	textY,
	//	0xFF0000FF,  // �Ķ���
	//	L"Arial"
	//);