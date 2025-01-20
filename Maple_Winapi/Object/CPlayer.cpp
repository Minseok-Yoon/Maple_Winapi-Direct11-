#include "CPlayer.h"
#include "../Component/CCollider.h"
#include "../Component/CBoxCollider2D.h"
#include "../Object/CGameObject.h"
#include "../Component/CComponent.h"

CPlayer::CPlayer() :
	m_fAttackDelayTime(1.2f),  // 공격 후 0.2초 동안 대기
	m_fElapsedTime(0.0f),
	m_bAttackCycle(false)
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
	CGameObject::Init();
}

void CPlayer::Update()
{
	CGameObject::Update();
}

void CPlayer::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CPlayer::Render()
{
	CGameObject::Render();
}