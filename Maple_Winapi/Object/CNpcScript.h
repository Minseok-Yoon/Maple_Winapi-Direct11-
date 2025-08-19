#pragma once
#include "../Component/CScript.h"
#include "../Component/CTransform.h"
#include "../Object/CBackGround.h"

class CNpc;
class CNpcScript : public CScript
{
public:
	CNpcScript();
	virtual ~CNpcScript();

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender(const Matrix& view, const Matrix& projection) override;

	virtual void OnCollisionEnter(class CCollider* other);
	virtual void OnCollisionStay(class CCollider* other);
	virtual void OnCollisionExit(class CCollider* other);

	CNpc* GetNpcOwner() { return m_pNpcOwner; }
	void SetNpcOwner(CNpc* _pNpcOwner) { m_pNpcOwner = _pNpcOwner; }

private:
	void idle();

private:
	CNpc* m_pNpcOwner = nullptr;
	CTransform* m_pTransform = nullptr;
	CBackGround* m_pBackGround = nullptr;
	class CAnimator* m_pAnimator;

	wstring				m_strCurAnimName; // 현재 재생 중인 애니메이션 이름
};