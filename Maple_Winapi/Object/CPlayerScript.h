#pragma once
#include "../Component/CScript.h"
#include "../Resource/CTexture.h"

class CPlayerScript : public CScript
{
public:
	CPlayerScript();
	~CPlayerScript();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void OnCollisionEnter(class CCollider* _pOther) override;
	void OnCollisionStay(class CCollider* _pOther) override;
	void OnCollisionExit(class CCollider* _pOther) override;

private:
	void idle();
	void move();
	void jump();
	void attack();

private:
	PLAYER_STATE		m_ePlayerState;
	class CAnimator* m_pAnimator;
};