#include "Trinity.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CSceneManager.h"
#include "TrinityScript.h"
#include "CPlayer.h"
#include "../Component/CAnimator.h"
#include "CBaseSkillScript.h"

Trinity::Trinity()
{
	SetName(L"Trinity");
}

Trinity::~Trinity()
{
}

void Trinity::Init()
{
	CBaseSkill::Init();

	if (m_pOwnerPlayer)
	{
		m_pTransform = this->AddComponent<CTransform>();
		Vector3 playerPos = m_pOwnerPlayer->GetTransform()->GetWorldPosition();

		// �÷��̾�� ���� ��ġ���� �׽�Ʈ
		Vector3 trinityPos = playerPos; // �Ͻ������� ����

		std::wstring posDebug = L"Trinity Position: " + std::to_wstring(trinityPos.x) +
			L", " + std::to_wstring(trinityPos.y) + L", " + std::to_wstring(trinityPos.z) + L"\n";
		OutputDebugString(posDebug.c_str());

		m_pTransform->SetLocalPosition(trinityPos);

		// �����ϵ� �Ͻ������� �۰� �ؼ� �׽�Ʈ
		m_pTransform->SetLocalScale(Vector3(581.0f, 479.0f, 1.0f));
		/*m_pTransform = this->AddComponent<CTransform>();
		m_pTransform->SetLocalPosition(
			m_pOwnerPlayer->GetTransform()->GetWorldPosition() + Vector3(0.0f, 50.0f, 0.0f)
		);
		m_pTransform->SetLocalScale(Vector3(581.0f, 479.0f, 0.0f));*/
	}

	// �÷��̾� ��ü�� ������ �� �ְ��ϱ�
	if (!m_pDetectedCollision) {
		m_pDetectedCollision = this->AddComponent<CCollider>();
		if (m_pDetectedCollision) {
			m_pDetectedCollision->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
			m_pDetectedCollision->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));
		}
	}

	if (!m_pHitCollision) {
		m_pHitCollision = this->AddComponent<CCollider>();
		if (m_pHitCollision) {
			m_pHitCollision->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
			m_pHitCollision->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));
		}
	}

	CBaseSkillScript* pScript = GetComponent<CBaseSkillScript>();
	if (pScript)
	{
		OutputDebugStringA("[DEBUG] Trinity::Init found CBaseSkillScript\n");
		/*SkillAnimInfo idleData = {
			L"../Resources/Texture/Player/Skill/Trinity/%s/Trinity_%d.png",
			18, 1.0f, Vector2(581.f, 479.f)
		};
		pScript->SetAnimationData(Skill_State::SS_Idle, idleData);
		OutputDebugStringA("[DEBUG] Trinity::Init called SetAnimationData\n");*/

		SkillAnimInfo castData = {
			L"../Resources/Texture/Player/Skill/Trinity/%s/Trinity_%d.png",
			18, 0.1f, Vector2(581.f, 479.f)
		};
		pScript->SetAnimationData(Skill_State::SS_Casting, castData);
		OutputDebugStringA("[DEBUG] Trinity::Init called SetAnimationData\n");
	}
}

void Trinity::Update()
{
	CBaseSkill::Update();

	if (m_pOwnerPlayer && m_pTransform)
	{
		Vector3 playerPos = m_pOwnerPlayer->GetTransform()->GetWorldPosition();
		//Vector3 trinityPos = playerPos; // �÷��̾�� ���� ��ġ �Ǵ�
		Vector3 trinityPos = playerPos + Vector3(200.0f, 0.0f, 1.0f); // �÷��̾� �� 50�ȼ�

		m_pTransform->SetLocalPosition(trinityPos);
	}

	if (state == Skill_State::SS_Cooldown)
	{
		currentCooldown -= CTimeManager::GetfDeltaTime();
		if (currentCooldown <= 0.0f)
		{
			state = Skill_State::SS_Idle;
		}
	}

	if (comboTimer > 0.0f) {
		comboTimer -= CTimeManager::GetfDeltaTime();
	}
	else if (comboTimer <= 0.0f && currentStage > 0) {
		resetcombo();
	}
}

void Trinity::LateUpdate()
{
	CBaseSkill::LateUpdate();
}

void Trinity::Render(const Matrix& view, const Matrix& projection)
{
	CBaseSkill::Render(view, projection);
}

void Trinity::Reset()
{
}

void Trinity::UseSkill()
{
	// 2. UseSkill() ȣ��Ǵ���
	//OutputDebugStringA("[DEBUG] Trinity::UseSkill called\n");

	if (state == Skill_State::SS_Cooldown) return;

	if (state == Skill_State::SS_Idle)
	{
		SetActive(true);

		auto renderer = GetComponent<CSpriteRenderer>();
		if (renderer) {
			renderer->SetEnabled(true);
		}

		startcombo();
		comboTimer = maxComboTime;
		return;
	}

	// �̹� Ȱ��ȭ�Ǿ� �ְ� ĳ���� ���̶�� ���� �Է� ���
	if (state == Skill_State::SS_Casting && GetActive())
	{
		return;
	}

	startcombo();

	comboTimer = maxComboTime;

	/*if (currentStage < 3 && comboTimer > 0.0f) {
		nextstage();
	}
	else if (state == Skill_State::SS_Idle) {
		startcombo();
	}

	comboTimer = maxComboTime;*/
}

void Trinity::EndSkill()
{
	state = Skill_State::SS_Idle;
	resetcombo();
	SetActive(false);

	// ������ ������Ʈ ��Ȱ��ȭ
	auto renderer = GetComponent<CSpriteRenderer>();
	if (renderer) {
		renderer->SetEnabled(false);
	}

	CBaseSkillScript* script = GetComponent<CBaseSkillScript>();
	if (script) {
		script->FinishAndHide();
	}
}

void Trinity::startcombo()
{
	// 3. startCombo�� ����Ǵ���
	//OutputDebugStringA("[DEBUG] startcombo()\n");

	currentStage = 1;
	state = Skill_State::SS_Casting;

	CBaseSkillScript* script = GetComponent<CBaseSkillScript>();
	if (script) {
		script->attack(Skill_State::SS_Casting);
	}
}

void Trinity::nextstage()
{
	currentStage++;
	if (currentStage >= 3) {
		state = Skill_State::SS_Cooldown;
		currentCooldown = cooldown;
	}
}

void Trinity::resetcombo()
{
	currentStage = 0;
	state = Skill_State::SS_Idle;
}

// �ִϸ��̼��� ������ �� nŰ�� ������ ������ �ٽ� 0�� �����Ӻ��� ����(��ų ��ü ����)
// �ִϸ��̼��� ������ �� nŰ�� ������ �ִٸ� (��ų ��ü ��Ȱ��ȭ)