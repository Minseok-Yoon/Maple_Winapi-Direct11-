#include "../pch.h"
#include "CPlayerBehavious.h"
#include "../Manager/CKeyManager.h"
#include "../Object/CItem.h"

CPlayerBehavious::CPlayerBehavious() :
	CScript(SCRIPT_TYPE::ST_PlayerBehavious)
{
}

CPlayerBehavious::~CPlayerBehavious()
{
}

void CPlayerBehavious::OnInit()
{
}

void CPlayerBehavious::OnUpdate()
{
	//OutputDebugString(L"[DEBUG] CPlayerBehavious::Update ȣ�� ��.\n");

	if (KEY_TAP(KEY_CODE::Z))
	{
		OutputDebugString(L"[DEBUG] ZŰ ���� ������\n");
		PickUpItem();
	}
}

void CPlayerBehavious::OnLateUpdate()
{
}

void CPlayerBehavious::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CPlayerBehavious::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayerBehavious::OnCollisionStay(CCollider* _pOther)
{
	//OutputDebugString(L"[DEBUG] OnCollisionStay ȣ���\n");

	if (_pOther && dynamic_cast<CItem*>(_pOther->GetOwner()))
	{
		m_pColliderItem = _pOther->GetOwner();
	}
}

void CPlayerBehavious::OnCollisionExit(CCollider* _pOther)
{
	if (_pOther && _pOther->GetOwner() == m_pColliderItem)
	{
		m_pColliderItem = nullptr;
	}
}

void CPlayerBehavious::PickUpItem()
{
	if (m_pColliderItem == nullptr) return;

	CItem* pItem = dynamic_cast<CItem*>(m_pColliderItem);
	if (pItem)
	{
		//pItem->OnPickUp();

		Destroy(pItem);

		m_iItemCnt++;

		// ��� ���ڿ� ����
		std::wstring debugStr = L"������ �̸�: " + pItem->GetName();
		debugStr += L" : (" + std::to_wstring(m_iItemCnt) + L"/" + std::to_wstring(m_iItemTarget) + L")\n";

		OutputDebugString(debugStr.c_str());

		// ���� �浹 ������ �ʱ�ȭ
		m_pColliderItem = nullptr;
	}
}