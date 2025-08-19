#include "../pch.h"
#include "Legato.h"
#include "CMentBox.h"
#include "CObject.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CSpriteRenderer.h"
#include "../Core/CCore.h"
#include "../Manager/CUIManager.h"
#include "../Quest/CQuest.h"

extern CCore core;

Legato::Legato()
{
	SetName(L"Legato");
}

Legato::~Legato()
{
}

void Legato::MentBox()
{
	CNpc::MentBox();

	// 1. ����Ʈ�� ���� ���� ���� ���� (�ߺ� ����)
	if (m_pQuest == nullptr)
	{
		GiveQuest();  // ���⼭ ����Ʈ ���� �� ���
	}

	CUIManager::PushWithText(UI_TYPE::UT_MentBox, m_pQuest->GetDescription(), m_pQuest);
	CUIManager::PushWithQuest(UI_TYPE::UT_MentBox, m_pQuest->GetDescription(), m_pQuest);
}

void Legato::CreateBtnMent(wstring& _CancelBtnName)
{
}

void Legato::Init()
{
	CNpc::Init();

	m_pTransform = this->GetComponent<CTransform>();
	if (m_pTransform) {
		m_pTransform->SetLocalScale(Vector3(231.0f, 134.0f, -1.0f));
	}
}

void Legato::Update()
{
	CNpc::Update();
}

void Legato::LateUpdate()
{
	CNpc::LateUpdate();
}

void Legato::Render(const Matrix& view, const Matrix& projection)
{
	CNpc::Render(view, projection);
}

void Legato::GiveQuest()
{
	// ����Ʈ ����
	m_pQuest = new CQuest(L"����� �����ٽ� óġ",
		L"����� ���信�� ����� �����ٽ��� 5���� óġ�ϼ���.",
		L"Gladness_of_Erdas", // <- ���� �̸� or ID
		5, 100);

	CPlayer* pPlayer = CSceneManager::GetCurScene()->GetPlayer();
	if (pPlayer)
	{
		OutputDebugStringW(L"[Legato] Player�� ã��, AddQuest ȣ����\n");
		pPlayer->AddQuest(m_pQuest);
	}
	else
	{
		OutputDebugStringW(L"[Legato] Player�� ã�� ���� (nullptr)\n");
	}
}

// 2025-06-24 MentBox() ���� ��
//void Legato::MentBox()
//{
//	CNpc::MentBox();
//
//	CMentBox* pMentBox = Instantiate<CMentBox>(LAYER_TYPE::LT_UI); // LT_UI �߿�
//	pMentBox->SetText(L"�ȳ��ϼ���, ���谡��. ���õ� ���� �Ϸ� �Ǽ���!");
//	CUIManager::RegisterUI(UI_TYPE::UT_MentBox, pMentBox);
//	CUIManager::OnComplete(pMentBox);
//
//	// 2. �ؽ�ó �ε�
//	CTexture* pTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//
//	// 3. ��ġ �� ������ ����
//	UINT screenWidth = core.GetWidth();
//	UINT screenHeight = core.GetHeight();
//
//	CTransform* tr = pMentBox->AddComponent<CTransform>();
//	Vector3 pos(static_cast<float>(screenWidth) / 4.0f,
//		static_cast<float>(screenHeight) / 4.0f,
//		0.0f);
//	tr->SetLocalPosition(pos);
//
//	if (pTex)
//	{
//		CTexture::TextureSize size = pTex->GetTextureSize();
//		tr->SetLocalScale(Vector3(size.width, size.height, 0.0f));
//	}
//
//	// 4. SpriteRenderer �߰� �� �ؽ�ó ����
//	CSpriteRenderer* sr = pMentBox->AddComponent<CSpriteRenderer>();
//	sr->SetTexture(pTex);
//}

// 2025-06-25
	// UIManager�� ���� MentBox ���� ��û
	//CUIManager::Push(UI_TYPE::UT_MentBox);

	//// ������ MentBox�� �ؽ�Ʈ ����
	//CMentBox* pMentBox = static_cast<CMentBox*>(CUIManager::GetUI(UI_TYPE::UT_MentBox));
	//if (pMentBox)
	//{
	//	pMentBox->SetText(L"�ȳ��ϼ���, ���谡��. ���õ� ���� �Ϸ� �Ǽ���!");
	//	OutputDebugStringA("MentBox text set successfully\n");
	//}
	//else
	//{
	//	OutputDebugStringA("ERROR: Failed to get MentBox UI\n");
	//}

// 2025-06-26
// npc�� Legato �Ѵ� mentBox, UIManager�� �߸������� �Ǿ� �ִ� ����.
// m_pQuest ������ �Ѱ����� ����.