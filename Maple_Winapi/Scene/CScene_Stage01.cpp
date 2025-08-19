#include "../pch.h"
#include "CScene_Stage01.h"
#include "../Resource/CTexture.h"
#include "../Component/CCamera.h"
#include "../Core/CCore.h"
#include "../Manager/CKeyManager.h"
#include "../Scene/CScene.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"
#include "../Manager/CColliderManager.h"
#include "../Component/CPixelCollider.h"
#include "../Object/CBackGround.h"
#include "../Object/CPortal.h"

#include "../Component/CAnimator.h"
#include "../Object/CGameObject.h"
#include "../Component/CRenderer.h"
#include "../Object/CObject.h"
#include "../Component/CComponent.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CAudioListener.h"
#include "../Component/CAudioSource.h"
#include "../Component/CRigidBody.h"
#include "../Resource/CAudioClip.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CCameraScript.h"
#include "../Object/CPlayerScript.h"
#include "../Component/CBoxCollider2D.h"
#include "..\Object\TestMonster.h"

CScene_Stage01::CScene_Stage01() :
	m_pPlayer(nullptr)
{
	CResourceManager::Load<CTexture>(L"Stage01_BG", L"../Resources/Texture/Rehelleun.png");
	CResourceManager::Load<CTexture>(L"Stage01_BG_Collision", L"../Resources/Texture/Collision_Lacheln.png");
	CAudioClip* ac = CResourceManager::Load<CAudioClip>(L"Stage01_BGSound", L"../Resources/Sound/LachelntheIllusionCity.mp3");
	//CResourceManager::Load<CTexture>(L"PLAYER", L"../Resources/Texture/Player/Player.bmp");
}

CScene_Stage01::~CScene_Stage01()
{
}

void CScene_Stage01::Init()
{
	CScene::Init();
	AllMonster.reserve(20);

	// �ΰ��� ���� �� ĳ���� ����(����� �ΰ� ���� ������ ���� Ȱ��ȭ)
	if (CSceneManager::GetDontDestroyOnLoad()->FindObjectByName(L"Player") == nullptr)
	{
		CPlayer* player = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
		player->SetActive(false);
		DontDestroyOnLoad(player);
	}
}

void CScene_Stage01::Update()
{
	CScene::Update();
	//if (KEY_TAP(KEY_CODE::ENTER))
	//{
	//	ChangeScene(SCENE_TYPE::STAGE_02);
	//}

	//if(KEY_TAP(KEY_CODE::F1))
	//{
	//	CCollider::ToggleRenderColliders();
	//}

	//if (KEY_TAP(KEY_CODE::F2))
	//{
	//	// �� �ؽ�ó�� ��� ��ü
	//	CTexture* newTexture = CResourceManager::GetInst()->LoadTexture(L"InGameBackGround1", L"texture\\MainLoad.bmp");
	//	if (newTexture)
	//	{
	//		m_pBackGround = newTexture;
	//	}
	//}

	//if (KEY_TAP(KEY_CODE::F3))
	//{
	//	// �� �ؽ�ó�� ��� ��ü
	//	CTexture* newTexture = CResourceManager::GetInst()->LoadTexture(L"InGameBackGround", L"texture\\Main.bmp");
	//	if (newTexture)
	//	{
	//		m_pBackGround = newTexture;
	//	}
	//}

	//for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; ++i)
	//{
	//	const vector<CObject*>& vecObj = GetGroupObject((OBJECT_TYPE)i);

	//	for (size_t j = 0; j < vecObj.size(); ++j)
	//	{
	//		vecObj[j]->Update();
	//	}
	//}
	CScene::Update();
	respawnMonster();
}

void CScene_Stage01::LateUpdate()
{
	CScene::LateUpdate();
}

void CScene_Stage01::Render()
{
	CScene::Render();
}

void CScene_Stage01::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
	CScene::Enter(L"Stage01_BG", L"Stage01_BGSound");
	m_pBackGround->CreateCollisionMap(L"Stage01_BG_Collision");

	// �÷��̾� �����
	CPlayer* player = dynamic_cast<CPlayer*>(
		CSceneManager::GetDontDestroyOnLoad()->FindObjectByName(L"Player")
		);

	if (player)
	{
		m_pPlayer = player;
		m_pPlayer->SetActive(true);

		// ���� ���� ����
		GetLayer(LAYER_TYPE::LT_Player)->AddGameObject(m_pPlayer);
		CSceneManager::GetCurScene()->SetPlayer(m_pPlayer);
	}

	CreateMonster<TestMonster>({ 0.0f, 0.0f, -1.0f });

	// ī�޶� ����
	CreateCamera(m_pPlayer);

	//// �÷��̾� ����
	//m_pPlayer = Instantiate<CPlayer>(LAYER_TYPE::Player, Vector2(760.0f, 777.0f));
	//DontDestroyOnLoad(m_pPlayer);
	//cameraComp->SetTarget(m_pPlayer);

	//CPlayerScript* plScript = m_pPlayer->AddComponent<CPlayerScript>();

	//// �÷��̾� �浹ü ����
	//CBoxCollider2D* plCollider = m_pPlayer->AddComponent<CBoxCollider2D>();
	//plCollider->SetOffsetPos(Vector2(0.0f, 0.0f));

	//CSpriteRenderer* sr = m_pPlayer->AddComponent<CSpriteRenderer>();

	//sr->SetTexture(CResourceManager::Find<CTexture>(L"Player"));

	//m_pPlayer->GetComponent<CTransform>()->SetPosition(Vector2(760.0f, 777.0f));
	//m_pPlayer->AddComponent<CRigidBody>();

	//// ��Ż ��ü �ʱ�ȭ
	//CPortal* pPortal = new CPortal("Portal 0");
	//pPortal->SetObjName(L"Portal");
	//pPortal->SetPos(Vector2(150.f, 830.f));
	//AddObject(pPortal, OBJECT_TYPE::PORTAL);

	//// �÷��̾� ��ü �ʱ�ȭ
	//CPlayer* pPlayer = new CPlayer();
	//pPlayer->SetObjName(L"Player");
	//pPlayer->SetPos(Vector2(760.f, 777.f));
	//pPlayer->SetScale(Vector2(100.f, 100.f));
	//pPlayer->SetPixelCollider(pBackGround->GetPixelCollider());
	//AddObject(pPlayer, OBJECT_TYPE::PLAYER);

	//RegisterPlayer(pPlayer);

	//CCamera::GetInst()->SetTarget(pPlayer);

	//// ���� ��ġ
	//Vector2 vResolution = CCore::GetInst()->GetResolution();

	//// ���� ������ ��ġ ����Ʈ
	//vector<Vector2> monsterPositions = {
	//	Vector2(350.f, 415.f),
	//	Vector2(150.f, 820.f),
	//};

	//// ���� ���� �ݺ���
	//for (const auto& pos : monsterPositions) {
	//	CMonster* pMonster = CMonFactory::CreateMonster(MON_TYPE::NORMAL, pos);
	//	pMonster->SetObjName(L"Monster");
	//	pMonster->SetScale(Vector2(100.f, 100.f));
	//	pMonster->SetPixelCollider(pBackGround->GetPixelCollider());
	//	AddObject(pMonster, OBJECT_TYPE::MONSTER);
	//}

	//CCamera::GetInst()->FadeIn(1.f);
	//CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	//CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::MONSTER);
	//CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PORTAL);
	//CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::ITEM);
	//CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::MESO);
	//CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PIXEL_BACKGROUND);
	//CColliderManager::GetInst()->CheckGroup(OBJECT_TYPE::MONSTER, OBJECT_TYPE::PIXEL_BACKGROUND);

	//FastUpdate();

	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Monster, true);
	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Skill, LAYER_TYPE::LT_Monster, true);
}

void CScene_Stage01::Exit()
{
	CScene::Exit();

	if (m_pPlayer)
		m_pPlayer->SetActive(false);
	//DeleteAll();

	//// �浹 ���� ���� �ʱ�ȭ
	//CColliderManager::GetInst()->Reset();
}

// ���� ��������� Init�Լ��� �ƴ� Enter�� ����غ���