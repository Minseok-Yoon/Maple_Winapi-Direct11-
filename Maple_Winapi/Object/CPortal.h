#pragma once
#include "CGameObject.h"
#include "../Component/CSpriteRenderer.h"

class CPortal : public CGameObject
{
public:
	CPortal();
	virtual ~CPortal();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(const Matrix& view, const Matrix& projection);

	void OnPlayerEnter();
	void OnPlayerStay();
	void OnPlayerExit();

	void SetMoveMap(wstring _strMapName) { m_strMoveMap = _strMapName; }
	wstring GetMoveMap() { return m_strMoveMap; }

private:
	wstring m_strMoveMap;
	CSpriteRenderer* m_pSpriteRenderer = nullptr;
	CCollider* m_pPortalCollision = nullptr;
};

//public:
//    CPortal(const string& portalTag);
//    virtual ~CPortal();
//
//public:
//    unordered_map<string, vector<pair<pair<float, float>, string>>> m_stagePortals;
//    unordered_map<string, wstring> m_stageFiles;
//    unordered_map<string, string>  m_portalToStageMap;
//
//
//private:
//    pair<float, float> m_tPosID;    // ��ġ �ĺ���
//    static int  m_iPortalCount;
//    string      m_portalId;         // ��Ż�� ������ �ĺ� ��
//    string      m_sTargetStage;     // ��ǥ ���������� �̸��� ����
//    string     m_strPortalTag; // ��Ż �±׸� �����ϴ� ��� ����
//
//
//public:
//    void SetPosID(pair<float, float> posID) { m_tPosID = posID; }
//    pair<float, float> GetPosID()   const { return m_tPosID; }
//
//    void SetTargetStage(const string& sTargetStage)
//    {
//        m_sTargetStage = sTargetStage;
//    }
//
//    const string& GetTargetStage()  const
//    {
//        return m_sTargetStage;
//    }
//
//    void SetPortalID(string id)
//    {
//        m_portalId = id;
//    }
//
//    string GetPortalID() const
//    {
//        return m_portalId;
//    }
//
//    void SetPortalTag(const string& _strPortalTag) { m_strPortalTag = _strPortalTag; }
//    const string& GetPortalTag() const { return m_strPortalTag; }
//
//public:
//    void StagePortals();
//    void PortalAnimationClip();
//
//    void AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY, float _fOffsetX, float _fOffsetY);
//
//public:
//    virtual void FastUpdate() {}; 	// Scene�� ���۵Ǳ� ������ ȣ��Ǵ� �Լ�
//    virtual void Update();
//    virtual void Render();