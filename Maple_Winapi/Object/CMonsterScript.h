#pragma once
#include "../Component/CScript.h"
#include "../Component/CTransform.h"
#include "../Object/CBackGround.h"
#include "../Resource/CTexture.h"

class CMonster;
class CMonsterScript : public CScript
{
public:
	CMonsterScript();
	virtual ~CMonsterScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	void AttackEffect();

	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);

	void RandomChangeDirTime(float _fMinTime, float _fMaxTime);
	void RandomChangeDir();

	//void OnDeath();

public:
	void SetMonOwner(CMonster* _pMonOwner) { m_pMonOwner = _pMonOwner; }

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
	void move();
	void dead();

private:
	//MON_STATE			m_eMonState;

	class CAnimator*	m_pAnimator;
	int					m_iDir;

	wstring				m_strCurAnimName; // 현재 재생 중인 애니메이션 이름

	// 2025-05-26
	CMonster* m_pMonOwner = nullptr;

private:
	float m_fChangeDirTime = 3.0f;
	CTransform* m_pTransform = nullptr;
	CBackGround* m_pBackGround = nullptr;
};