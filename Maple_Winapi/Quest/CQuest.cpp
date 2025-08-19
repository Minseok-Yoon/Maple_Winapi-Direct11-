#include "../pch.h"
#include "CQuest.h"

CQuest::CQuest(const wstring& _title, const wstring& _description, const wstring& _targetMonsterName, 
	int _goal, int _rewardGold) :
	m_strTitle(_title),
	m_strDescription(_description),
	m_strTargetMonsterName(_targetMonsterName),
	m_iGoalCount(_goal),
	m_iCurrentCount(0),
	m_bCompleted(false),
	m_iRewardGold(_rewardGold)
{
}

CQuest::~CQuest()
{
}

void CQuest::CompleteQuest()
{
	if (m_eQuestState == QUEST_STATE::QS_Accepted)
		m_eQuestState = QUEST_STATE::QS_Completed;
}

QUEST_STATE CQuest::GetQuestState() const
{
	return m_eQuestState;
}

const wstring& CQuest::GetTitle() const
{
	return m_strTitle;
}

const wstring& CQuest::GetDescription() const
{
	return m_strDescription;
}

int CQuest::GetRewardGold() const
{
	return m_iRewardGold;
}

void CQuest::TryProgress(const wstring& _killedMonsterName)
{
	if (m_bCompleted)
		return;

	if (_killedMonsterName == m_strTargetMonsterName)
	{
		m_iCurrentCount++;
		if (m_iCurrentCount >= m_iGoalCount)
		{
			m_bCompleted = true;
			OutputDebugStringW((L"[퀘스트 완료] " + m_strTitle + L"\n").c_str());
		}
	}
}
