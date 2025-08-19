#include "../pch.h"
#include "CSkillManager.h"
#include "../Object/Trinity.h"

CSkillManager::CSkillManager()
{
}

CSkillManager::~CSkillManager()
{
	for (auto& pair : m_Skills)
	{
		if (pair.second)
			delete pair.second;
	}
	m_Skills.clear();
}

void CSkillManager::Init()
{
	CreateSkill<Trinity>(SKILL_TYPE::ST_Trinity);
}

void CSkillManager::Update()
{
    // Ȱ��ȭ�� ��ų�� ������Ʈ
    for (auto& pair : m_Skills)
    {
        if (pair.second && pair.second->GetActive())
        {
            pair.second->Update();
        }
    }
}

void CSkillManager::ActiveSkill(SKILL_TYPE eSkillType)
{
    auto it = m_Skills.find(eSkillType);
    if (it != m_Skills.end() && it->second)
    {
        for (auto& pair : m_Skills)
        {
            if (pair.first != eSkillType && pair.second && pair.second->GetActive())
            {
                pair.second->EndSkill();
            }
        }
        it->second->UseSkill();

        //// �ٸ� ��� ��ų ��Ȱ��ȭ
        //DeactivateAllSkills();

        //// ���õ� ��ų Ȱ��ȭ
        //it->second->SetActive(true);
        //it->second->UseSkill();
    }
}

void CSkillManager::DeactiveSkill(SKILL_TYPE eSkillType)
{
    auto it = m_Skills.find(eSkillType);
    if (it != m_Skills.end() && it->second)
    {
        it->second->SetActive(false);
        it->second->EndSkill();
    }
}

void CSkillManager::DeactivateAllSkills()
{
    for (auto& pair : m_Skills)
    {
        if (pair.second && pair.second->GetActive())
        {
            pair.second->EndSkill();
        }
    }
}
