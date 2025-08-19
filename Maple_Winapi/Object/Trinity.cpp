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

		// 플레이어와 같은 위치에서 테스트
		Vector3 trinityPos = playerPos; // 일시적으로 제거

		std::wstring posDebug = L"Trinity Position: " + std::to_wstring(trinityPos.x) +
			L", " + std::to_wstring(trinityPos.y) + L", " + std::to_wstring(trinityPos.z) + L"\n";
		OutputDebugString(posDebug.c_str());

		m_pTransform->SetLocalPosition(trinityPos);

		// 스케일도 일시적으로 작게 해서 테스트
		m_pTransform->SetLocalScale(Vector3(581.0f, 479.0f, 1.0f));
		/*m_pTransform = this->AddComponent<CTransform>();
		m_pTransform->SetLocalPosition(
			m_pOwnerPlayer->GetTransform()->GetWorldPosition() + Vector3(0.0f, 50.0f, 0.0f)
		);
		m_pTransform->SetLocalScale(Vector3(581.0f, 479.0f, 0.0f));*/
	}

	// 플레이어 객체만 가져올 수 있게하기
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
		//Vector3 trinityPos = playerPos; // 플레이어와 같은 위치 또는
		Vector3 trinityPos = playerPos + Vector3(200.0f, 0.0f, 1.0f); // 플레이어 위 50픽셀

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
	// 2. UseSkill() 호출되는지
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

	// 이미 활성화되어 있고 캐스팅 중이라면 연속 입력 대기
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

	// 렌더링 컴포넌트 비활성화
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
	// 3. startCombo가 실행되는지
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

// 애니메이션이 끝났을 때 n키가 눌러져 있으면 다시 0번 프레임부터 시작(스킬 객체 유지)
// 애니메이션이 끝났을 때 n키가 떨어져 있다면 (스킬 객체 비활성화)