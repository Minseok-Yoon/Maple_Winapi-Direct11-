#include "CScript.h"
#include "CCollider.h"

CScript::CScript() :
	CComponent(COMPONENT_TYPE::CT_Script)
{
}

CScript::~CScript()
{
}

void CScript::Init()
{
}

void CScript::Update()
{
}

void CScript::LateUpdate()
{
}

void CScript::Render(const Matrix& view, const Matrix& projection)
{
}

void CScript::OnCollisionEnter(CCollider* _pOther)
{
}

void CScript::OnCollisionStay(CCollider* _pOther)
{
}

void CScript::OnCollisionExit(CCollider* _pOther)
{
}
