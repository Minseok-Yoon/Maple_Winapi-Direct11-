#include "../pch.h"
#include "CPortalScript.h"
#include "CCollider.h"
#include "../Object/CPortal.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CSceneManager.h"

CPortalScript::CPortalScript() :
	CScript(SCRIPT_TYPE::ST_PortalScript)
{
}

CPortalScript::~CPortalScript()
{
}

void CPortalScript::OnInit()
{
}

void CPortalScript::OnUpdate()
{
}

void CPortalScript::OnLateUpdate()
{
	CGameObject* owner = GetOwner();
	CPortal* portal = dynamic_cast<CPortal*>(owner);

	if (!portal)
		return;

	// �浹 ������ Ȯ��
	CCollider* collider = portal->GetComponent<CCollider>();
	if (collider && collider->IsColliding())  // IsColliding()�� m_bIsColliding ��ȯ
	{
		// ���� ����Ű �Է� Ȯ��
		if (KEY_TAP(KEY_CODE::UP))
		{
			wstring mapName = portal->GetMoveMap();
			if (!mapName.empty())
			{
				CSceneManager::RequestSceneChange(mapName);  // �����ϰ� �� ��ȯ ����
			}
		}
	}
}

void CPortalScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CPortalScript::OnCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		CPortal* portal = dynamic_cast<CPortal*>(GetOwner());
		if (portal)
			portal->OnPlayerEnter();
	}
}

void CPortalScript::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		CPortal* portal = dynamic_cast<CPortal*>(GetOwner());
		if (portal)
			portal->OnPlayerStay();
	}
}

void CPortalScript::OnCollisionExit(CCollider* _pOther)
{
	if (_pOther->GetOwner()->GetName() == L"Player")
	{
		CPortal* portal = dynamic_cast<CPortal*>(GetOwner());
		if (portal)
			portal->OnPlayerExit();
	}
}
