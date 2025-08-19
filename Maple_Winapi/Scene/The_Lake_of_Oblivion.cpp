#include "../pch.h"
#include "The_Lake_of_Oblivion.h"
#include "../Manager/CTimeManager.h"
#include "../Component/CTransform.h"

The_Lake_of_Oblivion::The_Lake_of_Oblivion()
{
}

The_Lake_of_Oblivion::~The_Lake_of_Oblivion()
{
}

void The_Lake_of_Oblivion::Enter()
{
}

void The_Lake_of_Oblivion::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void The_Lake_of_Oblivion::Exit()
{
}

void The_Lake_of_Oblivion::Init()
{
	CScene::Init();
	AllMonster.reserve(20);
}

void The_Lake_of_Oblivion::Update()
{
	CScene::Update();
	respawnMonster();

	//if (m_pPlayer->GetTransform()->GetWorldPosition().y <= vMapScale.y)
	//{
	//	m_pPlayer->GetTransform()->SetLocalPosition(vTeleportPos);
	//	//m_pPlayer->MoveVectorForceReset();
	//}
}

void The_Lake_of_Oblivion::LateUpdate()
{
	CScene::LateUpdate();
}

void The_Lake_of_Oblivion::Render()
{
	CScene::Render();
}

void The_Lake_of_Oblivion::respawnMonster()
{
	for (FieldMonsterInfo& monInfo : AllMonster)
	{
		if (!monInfo.pMonster->IsActive()) // ���� ���¶��
		{
			monInfo.fRespawnTimer -= CTimeManager::GetfDeltaTime();

			if (monInfo.fRespawnTimer <= 0.0f)
			{
				monInfo.pMonster->Respawn(); // ���ο��� ���� �ʱ�ȭ
				monInfo.pMonster->GetTransform()->SetLocalPosition(monInfo.fRespawnPos);

				// ���� �ʱ�ȭ
				monInfo.pMonster->SetActive(true);        // �ٽ� Ȱ��ȭ
				monInfo.pMonster->SetObjectState(OBJECT_STATE::OS_Active);
				monInfo.pMonster->SetIsDead(false);
				monInfo.pMonster->SetIsUpdate(true);    // �̰� ���� �Լ� ������ִ� �� ����

				monInfo.fRespawnTimer = RESPAWN_TIME;
			}
		}
		else
		{
			monInfo.fRespawnTimer = RESPAWN_TIME;
		}
	}
}

void The_Lake_of_Oblivion::PlaceMonster(CMonster* _pMonster, Vector3 _vRespawnPos)
{
	_pMonster->GetTransform()->SetLocalPosition(_vRespawnPos);
}

// 2025-05-29 ������ ���� ����
//void The_Lake_of_Oblivion::respawnMonster()
//{
//	fRespawnTime -= CTimeManager::GetfDeltaTime();
//	if (fRespawnTime > 0.0f) return;
//
//	for (size_t i = 0; i < AllMonster.size(); i++)
//	{
//		if (AllMonster[i].pMonster->IsUpdate() == false)
//		{
//			AllMonster[i].pMonster->Respawn();
//			AllMonster[i].pMonster->GetTransform()->SetLocalPosition(AllMonster[i].fRespawnPos);
//		}
//	}
//
//	fRespawnTime = RESPAWN_TIME;
//}