#pragma once
#include "CScene.h"
#include "../Object/Jinhilla_Phase1.h"
#include "../Object/CObject.h"

class Jinhillah_Scene : public CScene
{
public:
	Jinhillah_Scene();
	virtual ~Jinhillah_Scene();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void Destroy();

	virtual void Enter();
	virtual void Exit();

	template <typename MonsterType>
	void CreateMonster(Vector3 _vRespawnPos)
	{
		FieldMonsterInfo MonInfo;

		CMonster* pMonster = Instantiate<MonsterType>(LAYER_TYPE::LT_Monster);
		PlaceMonster(pMonster, _vRespawnPos);

		MonInfo.pMonster = pMonster;
		MonInfo.fRespawnPos = _vRespawnPos;

		AllMonsterer.push_back(MonInfo);
	}

private:
	vector<FieldMonsterInfo> AllMonsterer;
	Jinhilla_Phase1* m_pBossJinhilla = nullptr;



	std::shared_ptr<class ContentBackGround> Back = nullptr;
	std::shared_ptr<class Boss_Lucid_Phase1> Boss = nullptr;
	std::shared_ptr<class Player> CurPlayer = nullptr;

	// Map Detail
	//void ObjectUpdate(float _Delta);
	//std::vector<std::shared_ptr<FlowObject>> MapObjects;
};