#include "../pch.h"
#include "CBoss.h"
#include "CBossScript.h"

CBoss::CBoss()
{
}

CBoss::~CBoss()
{
}

void CBoss::Init()
{
	CGameObject::Init();

	m_iHP = BOSS_MAX_HP;

	if (m_pBossRenderer == nullptr)
	{
		m_pBossRenderer = GetComponent<CSpriteRenderer>();
		//m_pBossRenderer
	}

	if (!m_pBossTransform)
	{
		m_pBossTransform = this->AddComponent<CTransform>();
	}

	if (!m_pBossCollider) {
		m_pBossCollider = this->AddComponent<CCollider>();
		if (m_pBossCollider) {
			m_pBossCollider->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
			m_pBossCollider->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));
		}
	}

	m_pBossScript = this->GetComponent<CBossScript>();
	if (!m_pBossScript)
	{
		m_pBossScript = this->AddComponent<CBossScript>();
	}
	m_pBossScript->SetBossOwner(this);
}

void CBoss::Update()
{
	CGameObject::Update();
}

void CBoss::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CBoss::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

//void CBoss::Release()
//{
//}
