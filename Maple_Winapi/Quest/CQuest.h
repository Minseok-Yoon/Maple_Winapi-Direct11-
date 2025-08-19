#pragma once

enum class QUEST_STATE
{
	QS_NotAccepted,
	QS_Accepted,
	QS_Completed
};

class CQuest
{
public:
	CQuest(const wstring& _title, const wstring& _description, const wstring& _targetMonsterName,
		int goal, int _rewardGold);
	~CQuest();

	void CompleteQuest();

	QUEST_STATE GetQuestState() const;

	const wstring& GetTitle() const;
	const wstring& GetDescription() const;
	int GetRewardGold() const;

	// 2025-06-26
	void TryProgress(const wstring& _killedMonsterName);
	bool IsQuestCompleted() const { return m_bCompleted; }

	const wstring& GetTargetMonsterName() const { return m_strTargetMonsterName; }
	int GetCurrentCount() const { return m_iCurrentCount; }
	int GetGoalCount() const { return m_iGoalCount; }

private:
	wstring m_strTitle;
	wstring m_strDescription;
	int m_iRewardGold;

	QUEST_STATE m_eQuestState;

	// 2025-06-26
	wstring m_strTargetMonsterName;
	int m_iGoalCount;
	int m_iCurrentCount;
	bool m_bCompleted;
};