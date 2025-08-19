#include "../pch.h"
#include "CQuestManager.h"
#include "../Quest/CQuest.h"

CQuestManager::CQuestManager()
{
}

CQuestManager::~CQuestManager()
{
}

void CQuestManager::AcceptQuest(CQuest* pQuest)
{
	if (!pQuest || pQuest->GetQuestState() != QUEST_STATE::QS_NotAccepted)
		return;

	m_vecAcceptedQuests.push_back(pQuest);
	OutputDebugStringA("����Ʈ ������\n");
}

void CQuestManager::CompleteQuest(CQuest* pQuest)
{
	if (!pQuest || pQuest->GetQuestState() != QUEST_STATE::QS_Accepted)
		return;

	pQuest->CompleteQuest();
	OutputDebugStringA("����Ʈ �Ϸ��\n");
}

void CQuestManager::ClearAllQuests()
{
	m_vecAcceptedQuests.clear();
}

const vector<CQuest*>& CQuestManager::GetAcceptedQuests() const
{
	return m_vecAcceptedQuests;
}