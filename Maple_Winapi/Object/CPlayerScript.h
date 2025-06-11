#pragma once
#include "../Component/CScript.h"
#include "../Resource/CTexture.h"
#include "../Component/CPixelCollider.h"

class CMonster;
class CPlayerScript : public CScript
{
public:
	CPlayerScript();
	virtual ~CPlayerScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	void OnCollisionEnter(class CCollider* _pOther) override;
	void OnCollisionStay(class CCollider* _pOther) override;
	void OnCollisionExit(class CCollider* _pOther) override;

	/*vector<Vector3>GetCollisionPoints(const Vector3& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf);
	bool CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag);
	void UpdateCollisionState(bool& _bIsColiding, bool _bCollisionDetected, const string& _strColTag, void (CPlayerScript::* onEnter)(), void (CPlayerScript::* onExit)());
	void CheckPixelColor();*/

	/*void OnStageCollisionEnter();
	void OnStageCollisionExit();

	void OnStageCollisionEnter(CCollider* _pOther);
	void OnStageCollision(CCollider* _pOther);
	void OnStageCollisionExit(CCollider* _pOther);*/

	void SetPixelCollider(CPixelCollider* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

	void PlayerAttack(CMonster* _pMonster);

private:
	void idle();
	void move();
	void prone();
	void jump();
	void attack();

private:
	PLAYER_STATE		m_ePlayerState;
	class CAnimator*	m_pAnimator;
	int					m_iDir;

	CPixelCollider*		m_pPixelCollider;
	vector<Vector3>		m_vecCollisionPoint;

	// 2025-05-26 
	bool				m_bIsNormalAttack = false;
	bool				m_bHasDealtDamage = false;
	CGameObject*		m_pColliderMonster = nullptr;

	// 2025-06-06
	CGameObject* m_pColliderItem = nullptr;
	int m_iItemCnt = 0;
	int m_iItemTarget = 10;
};