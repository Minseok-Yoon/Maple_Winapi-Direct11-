#pragma once
#include "CScene.h"
#include "../Object/CMonster.h"

class CBoss_Clear_Room : public CScene
{
public:
	CBoss_Clear_Room();
	~CBoss_Clear_Room();

	// delete function
	CBoss_Clear_Room(const CBoss_Clear_Room& _Other) = delete;
	CBoss_Clear_Room(CBoss_Clear_Room&& _Other) noexcept = delete;
	CBoss_Clear_Room& operator=(const CBoss_Clear_Room& _Other) = delete;
	CBoss_Clear_Room& operator=(CBoss_Clear_Room&& _Other) noexcept = delete;

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
	void Destroy() override;

	void Enter() override;
	void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName) override;	// 해당 Scene에 진입 시 호출
	void Exit() override;

	void PlaceMonster(CMonster* _pMonster, Vector3 _vRespawnPos);

protected:
	vector<FieldMonsterInfo> AllMonster;
};