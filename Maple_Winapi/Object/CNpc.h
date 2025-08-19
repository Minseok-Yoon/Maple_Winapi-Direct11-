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
// npc�� ��ǳ���� ����� ���� �ϴ� ������ ������ ���� �ְ�
// �÷��̾ �浹 �߿� ��ȭŰ(space)�� ���콺 Ŭ���� ������ ū ��ȭâ�� �������⵵ �ϰ�
// ����Ʈ ���¿� ���� �޶����⵵ �Ѵ�.