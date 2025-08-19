#pragma once
#include "../Component/CScript.h"
#include "CBackGround.h"

class CBoss;
class CBossScript : public CScript
{
public:
	CBossScript();
	virtual ~CBossScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	void AttackEffect();

	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);

	//void RandomChangeDirTime(float _fMinTime, float _fMaxTime);
	//void RandomChangeDir();

	//void OnDeath();

	CBoss* GetBossOwner() { return m_pBossOwner; }
	void SetBossOwner(CBoss* _pBossOwner) { m_pBossOwner = _pBossOwner; }
	
	void CheckSummonCondition();
	void SummonMonster();

protected:
	int iUpYPivot = 1;
	int iDownYPivot = 0;
	Vector3 vMoveVectorForce = Vector3(0.0f, 0.0f, 0.0f);
	bool bWallCheck = true;

private:
	TextureColor checkGroundColor(const Vector3& _vPos);
	void changeDir();

	void moveUpdate();

	void idle();
	void summon1();
	void move();
	void dead();

private:
	//MON_STATE			m_eMonState;

	//class CAnimator* m_pAnimator;
	int					m_iDir;

	wstring				m_strCurAnimName; // 현재 재생 중인 애니메이션 이름

	float m_fChangeDirTime = 3.0f;
	CTransform* m_pTransform = nullptr;
	class CBackGround* m_pBackGround = nullptr;

	CBoss* m_pBossOwner = nullptr;

	float m_fSummonTimer = 0.0f;		// 소환 타이머
	bool	m_bSummonPending = false;	// 소환 대기 상태

};