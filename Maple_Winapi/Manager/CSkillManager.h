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

	// 스킬 가져오기
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

		// 새 스킬 생성
		T* newSkill = new T();
		newSkill->SetLayerType(LAYER_TYPE::LT_Skill);

		// 현재 씬에 등록
		CScene* activeScene = CSceneManager::GetCurScene();
		CLayer* layer = activeScene->GetLayer(LAYER_TYPE::LT_Skill);
		layer->AddGameObject(newSkill);

		// 소유자 설정 및 초기화
		newSkill->SetOwnerPlayer(m_pOwnerPlayer);
		newSkill->Init();

		// 초기에는 비활성화
		newSkill->SetActive(false);

		// 스킬 맵에 추가
		m_Skills[eSkillType] = newSkill;

		return newSkill;
	}

private:
	std::map<SKILL_TYPE, CBaseSkill*> m_Skills;
	CPlayer* m_pOwnerPlayer = nullptr;
};