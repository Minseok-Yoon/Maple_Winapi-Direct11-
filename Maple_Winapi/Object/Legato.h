#pragma once
#include "CNpc.h"

class CQuest;
class Legato : public CNpc
{
public:
	Legato();
	virtual ~Legato();

	virtual void MentBox() override;

	virtual void CreateBtnMent(wstring& _CancelBtnName) override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	// 2025-06-26
	void GiveQuest();
	CQuest* GetQuest() const { return m_pQuest; }

private:
	// 2025-06-26 Äù½ºÆ® ¸â¹ö Ãß°¡
	CQuest* m_pQuest = nullptr;
};