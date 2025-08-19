#pragma once

class CQuest;
class CQuestManager
{
	SINGLETON(CQuestManager);

public:
	void AcceptQuest(CQuest* pQuest);
	void CompleteQuest(CQuest* pQuest);
	void ClearAllQuests();

	const vector<CQuest*>& GetAcceptedQuests() const;

private:
	vector<CQuest*> m_vecAcceptedQuests;
};