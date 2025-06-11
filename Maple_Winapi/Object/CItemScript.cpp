#include "../pch.h"
#include "CItemScript.h"
#include "../Manager/CKeyManager.h"
#include "../Object/CItem.h"

CItemScript::CItemScript() :
	CScript(SCRIPT_TYPE::ST_ItemScript)
{
}

CItemScript::~CItemScript()
{
}

void CItemScript::OnInit()
{
}

void CItemScript::OnUpdate()
{
	if (KEY_TAP(KEY_CODE::Z))
	{
		OutputDebugString(L"[DEBUG] Z키 누름 감지됨\n");
		PickUpItem();
	}
}

void CItemScript::OnLateUpdate()
{
}

void CItemScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CItemScript::OnCollisionEnter(CCollider* _pOther)
{
}

void CItemScript::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther && dynamic_cast<CItem*>(_pOther->GetOwner()))
	{
		m_pColliderItem = _pOther->GetOwner();
	}
}

void CItemScript::OnCollisionExit(CCollider* _pOther)
{
	if (_pOther && _pOther->GetOwner() == m_pColliderItem)
	{
		m_pColliderItem = nullptr;
	}
}

void CItemScript::PickUpItem()
{
	if (m_pColliderItem == nullptr) return;

	CItem* pItem = dynamic_cast<CItem*>(m_pColliderItem);
	if (pItem)
	{
		//pItem->OnPickUp();

		Destroy(pItem);

		m_iItemCnt++;

		// 출력 문자열 구성
		std::wstring debugStr = L"아이템 이름: " + pItem->GetName();
		debugStr += L" : (" + std::to_wstring(m_iItemCnt) + L"/" + std::to_wstring(m_iItemTarget) + L")\n";

		OutputDebugString(debugStr.c_str());

		// 다음 충돌 아이템 초기화
		m_pColliderItem = nullptr;
	}
}
