#include "CBaseSkill.h"
#include "Trinity.h"

CBaseSkill::CBaseSkill()
{
}

CBaseSkill::~CBaseSkill()
{
}

void CBaseSkill::Init()
{
	CGameObject::Init();
	OutputDebugStringA("[DEBUG] CBaseSkill::Init start\n");

	if (!m_pTransform) {
		m_pTransform = this->AddComponent<CTransform>();
		if (!m_pTransform) {
			OutputDebugStringA("ERROR: Transform creation failed in CMonster::Init()\n");
		}
	}

	/*m_pSkillCollider = this->GetComponent<CCollider>();
	if (m_pSkillCollider == nullptr)
	{
		m_pSkillCollider = this->AddComponent<CCollider>();
	}*/

	m_pSkillScript = this->GetComponent<CBaseSkillScript>();
	if (!m_pSkillScript)
	{
		m_pSkillScript = this->AddComponent<CBaseSkillScript>();
		if (!m_pSkillScript)
		{
			OutputDebugStringA("ERROR: MonsterScript creation failed in CMonster::Init()\n");
		}
	}
	m_pSkillScript->SetSkillOwner(this); // 소유자 설정 (서로 연결)

	OutputDebugStringA("[DEBUG] CBaseSkill::Init end\n");
}

void CBaseSkill::Update()
{
	CGameObject::Update();
}

void CBaseSkill::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CBaseSkill::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

void CBaseSkill::Reset()
{
}

void CBaseSkill::ActiveSkill(SKILL_TYPE eSkillType)
{
}

void CBaseSkill::DeactiveSkill(SKILL_TYPE eSkillType)
{
}

void CBaseSkill::DeactivateAllSkills()
{
}
