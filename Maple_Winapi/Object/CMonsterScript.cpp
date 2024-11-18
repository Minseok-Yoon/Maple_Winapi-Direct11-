#include "CMonsterScript.h"

CMonsterScript::CMonsterScript()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Init()
{
}

void CMonsterScript::Update()
{
}

void CMonsterScript::LateUpdate()
{
}

void CMonsterScript::Render()
{
}

void CMonsterScript::AttackEffect()
{
}

void CMonsterScript::OnCollisionEnter(CCollider* other)
{
}

void CMonsterScript::OnCollisionStay(CCollider* other)
{
}

void CMonsterScript::OnCollisionExit(CCollider* other)
{
}
