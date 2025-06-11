#pragma once
#include "CRigidBody.h"
#include "../Object/CBackGround.h"
#include "../Object/CPlayer.h"
#include "../Object/CMonster.h"

class CPlayer;
class CMonster;
class CBackGround;
class CGravity : public CRigidBody
{
public:
	CGravity();
	virtual ~CGravity();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	void Jump(float _fJumpForce);

	void SetPlayerGround(bool _bPlayerGround) { m_bPlayerGround = _bPlayerGround; }
	void SetMonsterGround(bool _bMonsterGround) { m_bMonsterGround = _bMonsterGround; }
	/*bool IsOnGround() const { return m_bGround; }*/

	//bool CheckGround(CBackGround* pGround, Vector3 _fPlusCheckPos);

	void SetGravityEnabled(bool _bEnable)
	{
		m_bIsGravity = _bEnable;

		if (!_bEnable)
		{
			// 중력이 꺼지면 수직 속도를 0으로 설정하여 멈춤
			m_vVelocity.y = 0.0f;
		}
	}

	void Gravity();

	void SetPlayer(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }
	void SetMonster(CMonster* _pMonster) 
	{ 
		m_pMonster = _pMonster; 

		char buffer[256];
		sprintf_s(buffer, "[SetMonster] 받음: %p\n", m_pMonster);
		OutputDebugStringA(buffer);
	}

private:
	bool		m_bIsGravity;
	bool		m_bPlayerGround;
	bool		m_bMonsterGround;

	float		m_fGravityForce;
	float		m_fGravitySpeed;
	Vector2		m_vecMoveForce;

	float		m_fMaxGravity;

	Vector2		m_vGravity;

	CTransform* m_pTransform;

	//bool IsGroundCheck = true;
	//float SkipGround = 0.0f;
	//set<float> NotGround;

	bool m_bPrevGround;

	CPlayer* m_pPlayer = nullptr;
	CMonster* m_pMonster = nullptr;
};

// 몬스터의 배경 투명한거 왜 검게 나오는지 확인하기
// 몬스터와 플레이어 공격, 죽었을 때 아이템 나오게 하기