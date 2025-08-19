#pragma once
#include "../Entity/CEntity.h"
#include "../Object/CGameObject.h"
#include "../Layer/CLayer.h"
#include "../Object/CBackGround.h"
#include "../Component/CAudioSource.h"
#include "../Object/CPlayer.h"
#include "../Object/CNpc.h"
#include "..\Component\CAudioListener.h"
#include "../Object/CMonster.h"
#include "..\Object\Jinhilla_Phase1.h"

#define RESPAWN_TIME 6.0f;

class CCamera;
class CScene : public CEntity
{
public:
	CScene();
	virtual ~CScene();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void Destroy();

	virtual void Enter();
	virtual void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName);	// �ش� Scene�� ���� �� ȣ��
	virtual void Exit();	// �ش� Scene�� Ż�� �� ȣ��

	void DeleteAll();
	void AddGameObject(CGameObject* _pGameObj, const LAYER_TYPE _eLayerType);
	void EraseGameObject(CGameObject* _pGameObj);
	void AddCamera(CCamera* _pCamera);
	void RemoveCamera(CCamera* _pCamera);
	void CollectRenderables(CCamera* _pCamera, std::vector<CGameObject*>& opaqueList, std::vector<CGameObject*>& cutoutList
		, std::vector<CGameObject*>& transparentList) const;
	void SortByDistance(std::vector<CGameObject*>& renderList, 
		const Vector3& cameraPos, bool bAscending) const;
	void RenderRenderables(const std::vector<CGameObject*>& renderList
		, const Matrix& view, const Matrix& projection) const;


	CLayer* GetLayer(const LAYER_TYPE _eLayerType) const { return m_vecLayers[static_cast<UINT>(_eLayerType)]; }

	CBackGround* GetBackGround() const { return m_pBackGround; }

	vector<CGameObject*> GetGameObjects(const LAYER_TYPE _eLayerType)
	{
		if (_eLayerType >= LAYER_TYPE::LT_End)
			return {}; // �߸��� ���� ����

		return m_vecLayers[static_cast<UINT>(_eLayerType)]->GetGameObjects();
	}

	CPlayer* GetPlayer() { return m_pPlayer; }
	// 2025-06-27
	void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
	void SetBossMonster(Jinhilla_Phase1* _pBoss) { m_pBoss = _pBoss; }

	CGameObject* FindObjectByName(const wstring& _strName);

	// 2025-07-21
	CCamera* GetMainCamera() { return m_mapCamera[static_cast<int>(CAMERA_ORDER::CO_Main)]; }
	CCamera* GetCamera(int _iSelect) { return m_mapCamera[_iSelect]; }

	void CreateCamera(CGameObject* playerTarget);

	// 2025-06-14 NPC ����
	template <typename NpcType>
	void CreateNpc(wstring _NpcName, Vector3 _NpcPosition, bool _IsActive)
	{
		CNpc* npc = Instantiate<NpcType>(LAYER_TYPE::LT_Npc);
		placeNpc(npc, _NpcPosition);
		npc->SetActive(_IsActive);
	}

	// 2025-07-22 ���� ���� �Լ�.
	CAudioSource* CreateSceneAudio(const wstring& _SoundClipName, CGameObject*& _SoundObject);

	// 2025-07-23 ���� ���� ���ڴ� �ٸ��� ����
	template <typename MonsterType>
	void CreateRewardBox(Vector3 _vRespawnPos)
	{
		FieldMonsterInfo MonInfo;

		CMonster* pMonster = Instantiate<MonsterType>(LAYER_TYPE::LT_Monster);
		PlaceMonster(pMonster, _vRespawnPos);

		MonInfo.pMonster = pMonster;
		MonInfo.fRespawnPos = _vRespawnPos;

		AllMonsters.push_back(MonInfo);
	}

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

protected:
	void placeNpc(CNpc* _pNpc, Vector3 _vNpcPosition);
	void PlaceMonster(CMonster* _pMonster, Vector3 _vRespawnPos);

	void respawnMonster();

private:
	void createLayers();

public:
	class CBackGround* m_pBackGround;
	vector<CCamera*>	m_vecCameras;
	class CPlayer* m_pPlayer = nullptr;
	Jinhilla_Phase1* m_pBoss = nullptr;

protected:
	vector<FieldMonsterInfo> AllMonsters;
	CGameObject* camera = nullptr;
	class CAudioSource* m_pAudioSource;
	wstring PrevScene = L"";
	wstring NextScene = L"";


	// 205-07-22
	CGameObject* m_pSound = nullptr;

	float fRespawnTime = RESPAWN_TIME;
	vector<FieldMonsterInfo> AllMonster;

private:
	vector<CLayer*>		m_vecLayers;

	// 2025-07-21 ���� ���� ī�޶� ����
	unordered_map<int, CCamera*> m_mapCamera;

	CCamera* MainCamera = nullptr;
	CCamera* UICamera = nullptr;
};

// ���� ����: ��ü ���� ����� �������� ū �������� ������, �� ������ �ٽ� �������� ���� ��ҳ� �������� �����ȴ�.
// ū Ʋ�δ� Stage1Ŭ����, Stage2Ŭ������ ����� �۾�.

// Layer����: �ϳ��� Scene���� ��� Layer, ������Ʈ Layer, ĳ���� Layer, UI Layer ���� ���� �� �ֽ��ϴ�.
// �̷��� Layer ������ ���� ������ �������� ȿ�������� ������ �� ������, Ư�� Layer���� ������� �� ó���� ���������ϴ�(��: ����� ������ �ְ� ĳ���͸� �̵�)