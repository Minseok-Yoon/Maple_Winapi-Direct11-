#pragma once
#include "../Object/CGameObject.h"
#include "../Object/CMonster.h"
#include "../Quest/CQuest.h"
#include "../Resource/CTexture.h"

class Trinity;
class CQuest;
class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	/*bool CheckGround(Vector3 _fPlusCheckPos);

	float FindGroundForMovement(Vector3 currentPos, Vector3 prevPos);*/

	//float FindClosestGroundY(Vector3 _pos);

	// 2025-06-26
	void AddQuest(CQuest* _pQuest) 
	{ 
		wchar_t buf[256];
		swprintf_s(buf, 256, L"[Player] 퀘스트 추가됨: %s\n", _pQuest->GetTitle().c_str());
		OutputDebugStringW(buf);

		wchar_t bufa[256];
		swprintf_s(bufa, 256, L"[Player::AddQuest] this: %p\n", this);
		OutputDebugStringW(bufa);

		m_vecQuests.push_back(_pQuest); 

		wchar_t bufs[256];
		swprintf_s(bufs, 256, L"[DEBUG] Player 퀘스트 수: %d\n", (int)m_vecQuests.size());
		OutputDebugStringW(bufs);
	}
	void OnMonsterKilled(const wstring& _monsterName);
	const vector<CQuest*>& GetQuests() const { return m_vecQuests; }

public:
	Trinity* pTrinity = nullptr;

private:
	bool	m_bAttackCycle;
	float	m_fAttackDelayTime;		// 공격 후 대기 시간
	float	m_fElapsedTime;			// 경과 시간

	class CCollider* m_pCollider;

	// 지면 충돌 관련 변수 (2025-03-31 추가)
	set<float> NotGround;
	float SkipGround = 0.0f;
	bool IsGroundCheck = true;

	// 2025-05-24(플레이어 Hp, Mp 추가)
	static int PlayerHp;
	static int PlayerMp;

	// 2025-06-26
	vector<CQuest*> m_vecQuests;

	// 2025-06-30 디버깅 시각화 충돌 점
	Vector3 m_vLastGroundCheckPoint;
	bool m_bDebugShowGroundCheck = true;

	//CDebugDraw* m_pDebugDraw = nullptr;
	bool m_bOnGround;
};