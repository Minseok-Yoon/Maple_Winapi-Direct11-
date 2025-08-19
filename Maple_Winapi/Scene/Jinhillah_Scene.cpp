#include "Jinhillah_Scene.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CSkillManager.h"
#include "../Object/CObject.h"

Jinhillah_Scene::Jinhillah_Scene()
{
	CResourceManager::Load<CTexture>(L"Altar_of_Desire", L"../Resources/Texture/Map/Labyrinth_of_Pain/Altar_of_Desire/Altar_of_Desire.png");
	CResourceManager::Load<CTexture>(L"Collision_Altar_of_Desire", L"../Resources/Texture/Map/Labyrinth_of_Pain/Altar_of_Desire/Altar_of_Desire_Collision.png");
	CAudioClip* ac = CResourceManager::Load<CAudioClip>(L"Secret_Labyrinth", L"../Resources/Sound/SecretLabyrinth.mp3");
}

Jinhillah_Scene::~Jinhillah_Scene()
{
}

void Jinhillah_Scene::Init()
{
	CScene::Init();

	if (CSceneManager::GetDontDestroyOnLoad()->FindObjectByName(L"Player") == nullptr)
	{
		CPlayer* player = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
		player->SetActive(false);
		DontDestroyOnLoad(player);
	}

	/*if (CSceneManager::GetDontDestroyOnLoad()->FindObjectByName(L"Jinhilla") == nullptr)
	{
		Jinhilla_Phase1* bossMonster = Instantiate<Jinhilla_Phase1>(LAYER_TYPE::LT_BossMonster);
		bossMonster->SetActive(false);
		DontDestroyOnLoad(bossMonster);
	}*/
}

void Jinhillah_Scene::Update()
{
	CScene::Update();
}

void Jinhillah_Scene::LateUpdate()
{
	CScene::LateUpdate();
}

void Jinhillah_Scene::Render()
{
	CScene::Render();
}

void Jinhillah_Scene::Destroy()
{
	CScene::Destroy();
}

void Jinhillah_Scene::Enter()
{
	CScene::Enter();

	if (m_pBackGround == nullptr)
	{
		m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
		//m_pBackGround->CreateCollisionMap(L"Collision_Altar_of_Desire");
		//m_pBackGround->CreateMap(L"Altar_of_Desire");
	}

	if (m_pSound == nullptr)
	{
		m_pAudioSource = CreateSceneAudio(L"Secret_Labyrinth", m_pSound);
	}

	CPlayer* player = dynamic_cast<CPlayer*>(CSceneManager::GetDontDestroyOnLoad()->FindObjectByName(L"Player"));

	if (player)
	{
		m_pPlayer = player;
		m_pPlayer->SetActive(true);
		m_pPlayer->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.0f, 0.0f, -1.0f));

		// 현재 씬에 재등록
		GetLayer(LAYER_TYPE::LT_Player)->AddGameObject(m_pPlayer);
		CSceneManager::GetCurScene()->SetPlayer(m_pPlayer);
	}

	Jinhilla_Phase1* boss = dynamic_cast<Jinhilla_Phase1*>(CSceneManager::GetDontDestroyOnLoad()->FindObjectByName(L"Jinhilla"));

	if (boss)
	{
		m_pBossJinhilla = boss;
		m_pBossJinhilla->SetActive(true);
		m_pBossJinhilla->GetComponent<CTransform>()->SetLocalPosition(Vector3(0.0f, 0.0f, -1.0f));

		// 현재 씬에 재등록
		GetLayer(LAYER_TYPE::LT_BossMonster)->AddGameObject(m_pBossJinhilla);
		CSceneManager::GetCurScene()->SetBossMonster(m_pBossJinhilla);
	}

	// 카메라 설정
	CreateCamera(m_pPlayer);

	auto skill = CSkillManager::GetInst()->GetSkill(SKILL_TYPE::ST_Trinity);
	if (skill)
	{
		GetLayer(LAYER_TYPE::LT_Skill)->AddGameObject(skill);
	}

	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Monster, true);
}

void Jinhillah_Scene::Exit()
{
	CScene::Exit();

	if (m_pPlayer)
		m_pPlayer->SetActive(false);
}
