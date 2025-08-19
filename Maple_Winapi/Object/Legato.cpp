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

	// 1. 퀘스트가 아직 없을 때만 생성 (중복 방지)
	if (m_pQuest == nullptr)
	{
		GiveQuest();  // 여기서 퀘스트 생성 및 등록
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
	// 퀘스트 생성
	m_pQuest = new CQuest(L"기쁨의 에르다스 처치",
		L"기쁨의 영토에서 기쁨의 에르다스를 5마리 처치하세요.",
		L"Gladness_of_Erdas", // <- 몬스터 이름 or ID
		5, 100);

	CPlayer* pPlayer = CSceneManager::GetCurScene()->GetPlayer();
	if (pPlayer)
	{
		OutputDebugStringW(L"[Legato] Player를 찾음, AddQuest 호출함\n");
		pPlayer->AddQuest(m_pQuest);
	}
	else
	{
		OutputDebugStringW(L"[Legato] Player를 찾지 못함 (nullptr)\n");
	}
}

// 2025-06-24 MentBox() 수정 전
//void Legato::MentBox()
//{
//	CNpc::MentBox();
//
//	CMentBox* pMentBox = Instantiate<CMentBox>(LAYER_TYPE::LT_UI); // LT_UI 중요
//	pMentBox->SetText(L"안녕하세요, 모험가님. 오늘도 힘찬 하루 되세요!");
//	CUIManager::RegisterUI(UI_TYPE::UT_MentBox, pMentBox);
//	CUIManager::OnComplete(pMentBox);
//
//	// 2. 텍스처 로드
//	CTexture* pTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//
//	// 3. 위치 및 사이즈 설정
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
//	// 4. SpriteRenderer 추가 및 텍스처 지정
//	CSpriteRenderer* sr = pMentBox->AddComponent<CSpriteRenderer>();
//	sr->SetTexture(pTex);
//}

// 2025-06-25
	// UIManager를 통해 MentBox 생성 요청
	//CUIManager::Push(UI_TYPE::UT_MentBox);

	//// 생성된 MentBox에 텍스트 설정
	//CMentBox* pMentBox = static_cast<CMentBox*>(CUIManager::GetUI(UI_TYPE::UT_MentBox));
	//if (pMentBox)
	//{
	//	pMentBox->SetText(L"안녕하세요, 모험가님. 오늘도 힘찬 하루 되세요!");
	//	OutputDebugStringA("MentBox text set successfully\n");
	//}
	//else
	//{
	//	OutputDebugStringA("ERROR: Failed to get MentBox UI\n");
	//}

// 2025-06-26
// npc와 Legato 둘다 mentBox, UIManager와 잘못연동이 되어 있는 듯함.
// m_pQuest 정보를 넘겨주지 못함.