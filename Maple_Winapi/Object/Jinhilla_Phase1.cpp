#include "../pch.h"
#include "Jinhilla_Phase1.h"

Jinhilla_Phase1::Jinhilla_Phase1()
{
	SetName(L"Jinhilla");
}

Jinhilla_Phase1::~Jinhilla_Phase1()
{
}

void Jinhilla_Phase1::Init()
{
	CBoss::Init();

	m_bIsCoolDownUpdate = true;

	m_pBossTransform = this->GetComponent<CTransform>();
	if (m_pBossTransform)
		m_pBossTransform->SetLocalScale(Vector3(240.f, 305.f, -1.0f));

	//m_pJinhillaGui = CGameEngineGUI::CreateGUIWindow< Jinhilla_Phase1_GUI>(L"JinhillaState");
	//m_pJinhillaGui->m_pCurBoss = this;

	CBoss::Init();
	//m_vecSkillInfo.resize(2);
	//m_vecSkillInfo[0] = { ATTACK1_COOLDOWN, Jinhilla_State::JS_Attack1 };

	//if(CResourceManager::Find)

	//// Jinhilla Sound 배경 음악은 Scene에서 사용하기.
	//if (m_pSound == nullptr)
	//{
	//	m_pAudioSource = CreateSceneAudio(L"Depth_Of_Pain", m_pSound);
	//}

	//m_pBossRenderer->CreateAnimation(L"Idle", L"Jinhilla_Phase1_Idle");
	//idleStart();

	//CBossHpBar* BossHpBar = CSceneManager::GetCurScene()->
	//_HpBar->LinkBossHP(&HP);*/

}

void Jinhilla_Phase1::Update()
{
	CBoss::Update();
}

void Jinhilla_Phase1::LateUpdate()
{
	CBoss::LateUpdate();
}

void Jinhilla_Phase1::Render(const Matrix& view, const Matrix& projection)
{
	CBoss::Render(view, projection);
}

//void Jinhilla_Phase1::Release()
//{
//	CBoss::Release();
//}
