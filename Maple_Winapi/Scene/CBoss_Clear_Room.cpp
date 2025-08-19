#include "CBoss_Clear_Room.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"
#include "../Object/CObject.h"
#include "../Object/CBoss_Reward_Box.h"
#include "../Scene/The_Lake_of_Oblivion.h"
#include "../Component/CCameraScript.h"
#include "../Component/CGravity.h"

CBoss_Clear_Room::CBoss_Clear_Room()
{
	CResourceManager::Load<CTexture>(L"Altar_of_Desire_ClearRoom", L"../Resources/Texture/Map/Labyrinth_of_Pain/Altar_of_Desire/Altar_of_Desire.png");
	CResourceManager::Load<CTexture>(L"Collision_Altar_of_Desire_ClearRoom", L"../Resources/Texture/Map/Labyrinth_of_Pain/Altar_of_Desire/Altar_of_Desire_Collision.png");
	CResourceManager::Load<CTexture>(L"Altar_of_Desire_ClearRoom_Box", L"../Resources/Texture/Monster/Altar_of_Desire/ClearRoom/Stand/stand.0.png");
	CAudioClip* ac = CResourceManager::Load<CAudioClip>(L"Secret_Labyrinth", L"../Resources/Sound/SecretLabyrinth.mp3");
}

CBoss_Clear_Room::~CBoss_Clear_Room()
{
}

void CBoss_Clear_Room::Init()
{
	PrevScene = L"TestScene";

	CScene::Init();

	if (CSceneManager::GetDontDestroyOnLoad()->FindObjectByName(L"Player") == nullptr)
	{
		CPlayer* player = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
		player->SetActive(false);
		DontDestroyOnLoad(player);
	}
}

void CBoss_Clear_Room::Update()
{
	CScene::Update();
}

void CBoss_Clear_Room::LateUpdate()
{
	CScene::LateUpdate();
}

void CBoss_Clear_Room::Render()
{
	CScene::Render();
}

void CBoss_Clear_Room::Destroy()
{
	CScene::Destroy();
}

void CBoss_Clear_Room::Enter()
{
	CScene::Enter();

	if (m_pBackGround == nullptr)
	{
		m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
		m_pBackGround->CreateCollisionMap(L"Collision_Altar_of_Desire_ClearRoom");
		m_pBackGround->CreateMap(L"Altar_of_Desire_ClearRoom");
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

	CreateRewardBox<CBoss_Reward_Box>({ -400.0f, 0.0f, -1.0f });

	// 카메라 설정
	CreateCamera(m_pPlayer);

	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Monster, true);
	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_Item, true);
}

void CBoss_Clear_Room::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CBoss_Clear_Room::Exit()
{
	CScene::Exit();

	if (m_pPlayer)
		m_pPlayer->SetActive(false);
}

void CBoss_Clear_Room::PlaceMonster(CMonster* _pMonster, Vector3 _vRespawnPos)
{
	_pMonster->GetTransform()->SetLocalPosition(_vRespawnPos);
}