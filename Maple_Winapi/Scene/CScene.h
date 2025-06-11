#pragma once
#include "../Entity/CEntity.h"
#include "../Object/CGameObject.h"
#include "../Layer/CLayer.h"
#include "../Object/CBackGround.h"
#include "../Component/CAudioSource.h"
#include "../Object/CMiniMap.h"
#include "../Object/CPlayer.h"

class CMiniMap;
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

	CBackGround* GetCurBackGround() { return m_pBackGround; }
	CPlayer* GetPlayer() { return m_pPlayer; }

	CGameObject* FindObjectByName(const wstring& _strName);

private:
	void createLayers();

public:
	class CBackGround* m_pBackGround;
	vector<CCamera*>	m_vecCameras;
	class CPlayer* m_pPlayer = nullptr;

protected:
	class CAudioSource* m_pAudioSource;
	wstring PrevScene = L"";
	wstring NextScene = L"";

private:
	vector<CLayer*>		m_vecLayers;
};

// ���� ����: ��ü ���� ����� �������� ū �������� ������, �� ������ �ٽ� �������� ���� ��ҳ� �������� �����ȴ�.
// ū Ʋ�δ� Stage1Ŭ����, Stage2Ŭ������ ����� �۾�.

// Layer����: �ϳ��� Scene���� ��� Layer, ������Ʈ Layer, ĳ���� Layer, UI Layer ���� ���� �� �ֽ��ϴ�.
// �̷��� Layer ������ ���� ������ �������� ȿ�������� ������ �� ������, Ư�� Layer���� ������� �� ó���� ���������ϴ�(��: ����� ������ �ְ� ĳ���͸� �̵�)