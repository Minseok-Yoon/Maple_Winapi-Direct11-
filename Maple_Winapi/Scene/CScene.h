#pragma once
#include "../Entity/CEntity.h"
#include "../Object/CGameObject.h"
#include "../Layer/CLayer.h"
#include "../Object/CGround.h"
#include "../Component/CAudioSource.h"

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
	virtual void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName);	// 해당 Scene에 진입 시 호출
	virtual void Exit();	// 해당 Scene에 탈출 시 호출

	void AddGameObject(CGameObject* _pGameObj, const LAYER_TYPE _eLayerType);
	void EraseGameObject(CGameObject* _pGameObj);

	CLayer* GetLayer(const LAYER_TYPE _eLayerType) { return m_vecLayers[static_cast<UINT>(_eLayerType)]; }

	//void SetMapSize(Vector2 _vMapSize) { m_vMapSize = _vMapSize; }
	//Vector2 GetMapSize() { return m_vMapSize; }

private:
	void createLayers();

protected:
	class CBackGround*	m_pBackGround;
	class CAudioSource* m_pAudioSource;

private:
	vector<CLayer*>		m_vecLayers;
	//Vector2		m_vMapSize;
};

// 세계 구성: 전체 게임 세계는 여러개의 큰 지역으로 나뉘고, 각 지역은 다시 여러개의 작은 장소나 던전으로 구성된다.
// 큰 틀로는 Stage1클래스, Stage2클래스로 나누어서 작업.

// Layer구성: 하나의 Scene에는 배경 Layer, 오브젝트 Layer, 캐릭터 Layer, UI Layer 등이 있을 수 있습니다.
// 이러한 Layer 구분을 통해 게임의 렌더링을 효율적으로 관리할 수 있으며, 특정 Layer만을 대상으로 한 처리가 가능해집니다(예: 배경은 정지해 있고 캐릭터만 이동)