#pragma once
#include "../Object/CGameObject.h"
#include "../Object/CUI.h"
#include "../Resource/CTexture.h"
#include "../Object/CNpcScript.h"
#include "../Component/CCamera.h"

class CNpc : public CGameObject
{
public:
	CNpc();
	virtual ~CNpc();

	virtual void NpcClick();
	virtual void MentBox();

	virtual void CreateBtnMent(wstring& _CancelBtnName);

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

	// 2025-06-18
	void SetNpcCurState(NPC_STATE _eNpcState) { m_eNpcState = _eNpcState; }
	NPC_STATE GetNpcCurState() const { return m_eNpcState; }

private:
	shared_ptr<CUI> m_pMentBGTexture = nullptr;
	shared_ptr<CUI> m_pMent = nullptr;

	// 2025-06-14
	CTexture* m_pNpcTexture;

	// 2025-06-16
	class CAnimator* m_pAnimator = nullptr;
	CNpcScript* m_pNpcScript = nullptr;
	class CCollider* m_pCollider = nullptr;

	// 2025-06-17
	NPC_STATE			m_eNpcState;

	// 2025-06-18
	CUI::tUIEvent			m_tNpcOnClick;
	bool	m_bMouseOn = false;
	CCamera* m_pCamera = nullptr;

	// 2025-06-20
	CTexture*	m_pNpcMentBox = nullptr;
};

// 2025-06-17 
// npc는 말풍선을 띄워서 말을 하는 것으로 보여줄 수도 있고
// 플레이어가 충돌 중에 대화키(space)나 마우스 클릭을 누르면 큰 대화창이 보여지기도 하고
// 퀘스트 상태에 따라서 달라지기도 한다.