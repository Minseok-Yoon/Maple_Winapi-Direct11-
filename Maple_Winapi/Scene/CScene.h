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
	virtual void Enter(const wstring& _strBackGroundName, const wstring& _strAudioName);	// �ش� Scene�� ���� �� ȣ��
	virtual void Exit();	// �ش� Scene�� Ż�� �� ȣ��

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

// ���� ����: ��ü ���� ����� �������� ū �������� ������, �� ������ �ٽ� �������� ���� ��ҳ� �������� �����ȴ�.
// ū Ʋ�δ� Stage1Ŭ����, Stage2Ŭ������ ����� �۾�.

// Layer����: �ϳ��� Scene���� ��� Layer, ������Ʈ Layer, ĳ���� Layer, UI Layer ���� ���� �� �ֽ��ϴ�.
// �̷��� Layer ������ ���� ������ �������� ȿ�������� ������ �� ������, Ư�� Layer���� ������� �� ó���� ���������ϴ�(��: ����� ������ �ְ� ĳ���͸� �̵�)