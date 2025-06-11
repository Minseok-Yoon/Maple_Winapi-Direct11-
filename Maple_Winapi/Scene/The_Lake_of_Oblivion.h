#pragma once
#include "CScene.h"
#include "../Object/CMonster.h"

#define RESPAWN_TIME 6.0f

class The_Lake_of_Oblivion : public CScene
{
public:
    The_Lake_of_Oblivion();
    ~The_Lake_of_Oblivion();

    void Enter() override;
    void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;
    void Exit() override;

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

    template <typename MonsterType>
    void CreateMonster(Vector3 _vRespawnPos)
    {
        FieldMonsterInfo MonInfo;

        CMonster* pMonster = Instantiate<MonsterType>(LAYER_TYPE::LT_Monster);
        PlaceMonster(pMonster, _vRespawnPos);

        MonInfo.pMonster = pMonster;
        MonInfo.fRespawnPos = _vRespawnPos;

        AllMonster.push_back(MonInfo);
    }

private:
    void respawnMonster();
    void PlaceMonster(CMonster* _pMonster, Vector3 _vRespawnPos);

protected:
    float fRespawnTime = RESPAWN_TIME;
    Vector3 vMapScale = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 vTeleportPos = Vector3(0.0f, 0.0f, 0.0f);
    vector<FieldMonsterInfo> AllMonster;

    CPlayer* m_pPlayer = nullptr;
};