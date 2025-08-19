#pragma once
#include "../Object/CBaseSkill.h"
#include "../Scene/CScene.h"
#include "../Layer/CLayer.h"
#include "../Manager/CSceneManager.h"

class CSkillManager
{
	SINGLETON(CSkillManager);

public:
	void Init();
	void Update();

	void ActiveSkill(SKILL_TYPE eSkillType);
	void DeactiveSkill(SKILL_TYPE eSkillType);
	void DeactivateAllSkills();

	// ��ų ��������
	CBaseSkill* GetSkill(SKILL_TYPE eSkillType)
	{
		auto it = m_Skills.find(eSkillType);
		return (it != m_Skills.end()) ? it->second : nullptr;
	}

	void SetOwnerPlayer(CPlayer* player) { m_pOwnerPlayer = player; }

	template<typename T>
	T* CreateSkill(SKILL_TYPE eSkillType)
	{
		if (m_Skills.find(eSkillType) != m_Skills.end())
		{
			return dynamic_cast<T*>(m_Skills[eSkillType]);
		}

		// �� ��ų ����
		T* newSkill = new T();
		newSkill->SetLayerType(LAYER_TYPE::LT_Skill);

		// ���� ���� ���
		CScene* activeScene = CSceneManager::GetCurScene();
		CLayer* layer = activeScene->GetLayer(LAYER_TYPE::LT_Skill);
		layer->AddGameObject(newSkill);

		// ������ ���� �� �ʱ�ȭ
		newSkill->SetOwnerPlayer(m_pOwnerPlayer);
		newSkill->Init();

		// �ʱ⿡�� ��Ȱ��ȭ
		newSkill->SetActive(false);

		// ��ų �ʿ� �߰�
		m_Skills[eSkillType] = newSkill;

		return newSkill;
	}

private:
	std::map<SKILL_TYPE, CBaseSkill*> m_Skills;
	CPlayer* m_pOwnerPlayer = nullptr;
};