#include "../pch.h"
#include "CNpc.h"
#include "../Core/CCore.h"
#include "../Object/CObject.h"
#include "../Resource/CTexture.h"
#include "../Object/CGameObject.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CAnimator.h"
#include "CMentBox.h"
#include "../Component/CCamera.h"

extern CCore core;

CNpc::CNpc()
{
	CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
}

CNpc::~CNpc()
{
}

void CNpc::NpcClick()
{
	// 1. 만약 Npc가 퀘스트를 주는 npc나 상점 같은 ui를 호출하는 npc일때
	// 2. 그냥 일반 멘트만 나올 때.
	MentBox();
}

void CNpc::MentBox()
{
	OutputDebugStringA("MentBox is Active!");
}

void CNpc::CreateBtnMent(wstring& _CancelBtnName)
{
	/*m_pMent = Instantiate<CUI>(LAYER_TYPE::LT_UI);
	m_pMent->Create*/
}

void CNpc::Init()
{
	CGameObject::Init();

	if (!m_pTransform) {
		m_pTransform = this->AddComponent<CTransform>();
		if (!m_pTransform) {
			OutputDebugStringA("ERROR: Transform creation failed in CMonster::Init()\n");
		}
	}

	// Collider 컴포넌트 확인 및 추가
	/*if (!m_pCollider) {
		m_pCollider = this->AddComponent<CCollider>();
		if (m_pCollider) {
			m_pCollider->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
			m_pCollider->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));
			OutputDebugStringA("INFO: Collider successfully added to Monster\n");
		}
		else {
			OutputDebugStringA("ERROR: Collider creation failed in CMonster::Init()\n");
		}
	}*/

	m_pNpcScript = this->GetComponent<CNpcScript>();
	if (!m_pNpcScript)
	{
		m_pNpcScript = this->AddComponent<CNpcScript>();
		if (!m_pNpcScript)
		{
			OutputDebugStringA("ERROR: MonsterScript creation failed in CMonster::Init()\n");
		}
	}
	m_pNpcScript->SetNpcOwner(this); // 소유자 설정 (서로 연결)

	m_tNpcOnClick = bind(&CNpc::NpcClick, this);
}

void CNpc::Update()
{
	CGameObject::Update();

	if (!m_pAnimator)
		m_pAnimator = this->GetComponent<CAnimator>();

	if (!m_pAnimator)
		m_pAnimator = this->AddComponent<CAnimator>();

	CSpriteRenderer* sr = this->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = this->AddComponent<CSpriteRenderer>();
	}
	m_pAnimator->SetSpriteRenderer(sr);

	CCamera* cam = CCamera::GetMainCam();
	if (cam)
	{
		Matrix view = cam->GetViewMatrix();
		Matrix proj = cam->GetProjectionMatrix();

		CTransform* tr = GetComponent<CTransform>();
		//if (tr)
		//{
		//	Vector3 npcPos = tr->GetWorldPosition();
		//	bool vis = cam->IsWorldPosVisible(npcPos, view, proj);
		//	//OutputDebugStringA(vis ? "[NPC] Visible\n" : "[NPC] NOT Visible\n");
		//}

		// MentBox 위치 가시 여부 확인
		if (m_pMent) // MentBox가 생성되어 있으면
		{
			CTransform* mentTr = m_pMent->GetComponent<CTransform>();
			if (mentTr)
			{
				Vector3 mentPos = mentTr->GetWorldPosition();
				bool mentVis = cam->IsWorldPosVisible(mentPos, view, proj);

				char buf[128];
				sprintf_s(buf, "[MentBox] %s\n", mentVis ? "Visible" : "NOT Visible");
				OutputDebugStringA(buf);
			}
		}
	}

	// 2025-06-18
	if (KEY_TAP(KEY_CODE::LBTN))
	{
		Vector2 mousePos = CKeyManager::GetInst()->GetMousePos();
		Vector3 worldPos = CCamera::GetMainCam()->ScreenToWorld(mousePos);

		CTransform* tr = this->GetComponent<CTransform>();
		Vector3 npcTr = tr->GetWorldPosition();
		Vector3 npcSize = tr->GetWorldScale();

		if (npcTr.x <= worldPos.x && worldPos.x <= npcTr.x + npcSize.x
			&& npcTr.y <= -worldPos.y && -worldPos.y <= npcTr.y + npcSize.y)
		{
			m_bMouseOn = true;
		}
		else
		{
			m_bMouseOn = false;
		}

		if (m_bMouseOn)
		{
			NpcClick(); // 자기 자신 클릭됨
		}
	}
}

void CNpc::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CNpc::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

// 2025-06-18
// 마우스가 해당 오브젝트 안에서 클릭이 되었는가?
	/*Vector2 mousePos = CKeyManager::GetMousePos();
	Vector3 worldMousePos = m_pCamera->ScreenToWorld(mousePos);
	CTransform* npcTr = this->GetComponent<CTransform>();
	Vector3 npcPos = npcTr->GetLocalPosition();
	Vector3 npcSize = npcTr->GetLocalScale();

	if (npcPos.x <= worldMousePos.x && worldMousePos.x <= npcPos.x + npcSize.x
		&& npcPos.y <= worldMousePos.y && worldMousePos.y <= npcPos.y + npcSize.y)
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
			CNpc::m_tNpcOnClick();
	}*/

// 2025-06-20
// npc멘트는 UI전용으로 만들어야 함. 지금은 npc 자체에서 Sprite를 사용해서 덮어씌워지고 있음.
// 근데 나중 것이 덮어씌워지면 기존 애니메이션이 아닌 NpcMent박스가 렌더링이 되어야 하는 것이 아닌가?
// UI전용으로 만들어서 다시 구현해보기.

// 2025-06-23
/*UINT screenWidth = core.GetWidth();
	UINT screenHeight = core.GetHeight();

	m_pNpcMentBox = CResourceManager::Find<CTexture>(L"NpcMent_BG");

	CSpriteRenderer* sr = AddComponent<CSpriteRenderer>();
	sr->SetTexture(m_pNpcMentBox);

	Vector3 localPos(
		static_cast<float>(screenWidth) / 2.0f,
		static_cast<float>(screenHeight) / 2.0f,
		-1.0f
	);

	CTransform* bgTr = GetComponent<CTransform>();
	bgTr->SetLocalPosition(localPos);

	if (m_pNpcMentBox != nullptr)
	{
		CTexture::TextureSize textureSize = m_pNpcMentBox->GetTextureSize();
		bgTr->SetLocalScale(Vector3(textureSize.width, textureSize.height, 0.0f));
	}*/

// 2025-06-23
//void CNpc::MentBox()
//{
//	//CMentBox* pMentBox = Instantiate<CMentBox>(LAYER_TYPE::LT_UI); // LT_UI 중요
//
// //  // 2. 텍스처 로드
//	//CTexture* pTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//
//	//// 3. 위치 및 사이즈 설정
//	//UINT screenWidth = core.GetWidth();
//	//UINT screenHeight = core.GetHeight();
//
//	//CTransform* tr = pMentBox->AddComponent<CTransform>();
//	//Vector3 pos(static_cast<float>(screenWidth) / 4.0f,
//	//	static_cast<float>(screenHeight) / 4.0f,
//	//	0.0f);
//	//tr->SetWorldPosition(pos);
//
//	//if (pTex)
//	//{
//	//	CTexture::TextureSize size = pTex->GetTextureSize();
//	//	tr->SetLocalScale(Vector3(size.width, size.height, 0.0f));
//	//}
//
//	//// 4. SpriteRenderer 추가 및 텍스처 지정
//	//CSpriteRenderer* sr = pMentBox->AddComponent<CSpriteRenderer>();
//	//sr->SetTexture(pTex);
//}