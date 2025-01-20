#pragma once
#include "../Component/CScript.h"
#include "../Resource/CTexture.h"
#include "../Component/CPixelCollider.h"

class CPlayerScript : public CScript
{
public:
	CPlayerScript();
	virtual ~CPlayerScript();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void OnCollisionEnter(class CCollider* _pOther) override;
	void OnCollisionStay(class CCollider* _pOther) override;
	void OnCollisionExit(class CCollider* _pOther) override;

	vector<Vector3>GetCollisionPoints(const Vector3& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf);
	bool CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag);
	void UpdateCollisionState(bool& _bIsColiding, bool _bCollisionDetected, const string& _strColTag, void (CPlayerScript::* onEnter)(), void (CPlayerScript::* onExit)());
	void CheckPixelColor();

	void OnStageCollisionEnter();
	void OnStageCollisionExit();

	void OnStageCollisionEnter(CCollider* _pOther);
	void OnStageCollision(CCollider* _pOther);
	void OnStageCollisionExit(CCollider* _pOther);

	void SetPixelCollider(CPixelCollider* _pPixelCollider) { m_pPixelCollider = _pPixelCollider; }

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
};