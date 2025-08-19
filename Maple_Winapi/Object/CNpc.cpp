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
	// 1. ���� Npc�� ����Ʈ�� �ִ� npc�� ���� ���� ui�� ȣ���ϴ� npc�϶�
	// 2. �׳� �Ϲ� ��Ʈ�� ���� ��.
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

	// Collider ������Ʈ Ȯ�� �� �߰�
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
	m_pNpcScript->SetNpcOwner(this); // ������ ���� (���� ����)

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

		// MentBox ��ġ ���� ���� Ȯ��
		if (m_pMent) // MentBox�� �����Ǿ� ������
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
			NpcClick(); // �ڱ� �ڽ� Ŭ����
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
// ���콺�� �ش� ������Ʈ �ȿ��� Ŭ���� �Ǿ��°�?
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
// npc��Ʈ�� UI�������� ������ ��. ������ npc ��ü���� Sprite�� ����ؼ� ��������� ����.
// �ٵ� ���� ���� ��������� ���� �ִϸ��̼��� �ƴ� NpcMent�ڽ��� �������� �Ǿ�� �ϴ� ���� �ƴѰ�?
// UI�������� ���� �ٽ� �����غ���.

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
//	//CMentBox* pMentBox = Instantiate<CMentBox>(LAYER_TYPE::LT_UI); // LT_UI �߿�
//
// //  // 2. �ؽ�ó �ε�
//	//CTexture* pTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//
//	//// 3. ��ġ �� ������ ����
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
//	//// 4. SpriteRenderer �߰� �� �ؽ�ó ����
//	//CSpriteRenderer* sr = pMentBox->AddComponent<CSpriteRenderer>();
//	//sr->SetTexture(pTex);
//}