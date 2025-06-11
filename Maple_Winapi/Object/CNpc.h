#pragma once
#include "../Object/CGameObject.h"
#include "../Object/CUI.h"

class CNpc : public CGameObject
{
public:
	CNpc();
	virtual ~CNpc();

	void MentBox();

	void CreateBtnMent(wstring& _CancelBtnName);

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

private:
	shared_ptr<CUI> m_pMentBGTexture = nullptr;
	shared_ptr<CUI> m_pMent = nullptr;
};

