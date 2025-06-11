#include "CScript.h"
#include "CCollider.h"

CScript::CScript(SCRIPT_TYPE _eScriptType) :
	CComponent(COMPONENT_TYPE::CT_Script)
{
}

CScript::~CScript()
{
}

void CScript::Init()
{
	OnInit();
}

void CScript::Update()
{
	OnUpdate();
}

void CScript::LateUpdate()
{
	OnLateUpdate();
}

void CScript::Render(const Matrix& view, const Matrix& projection)
{
	OnRender(view, projection);
}

void CScript::OnInit()
{
}

void CScript::OnUpdate()
{
}

void CScript::OnLateUpdate()
{
}

void CScript::OnRender(const Matrix& view, const Matrix& projection)
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
