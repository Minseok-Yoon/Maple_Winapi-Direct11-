#include "CMonsterScript.h"
#include "../Component/CRigidBody.h"
#include "../Manager/CKeyManager.h"

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

void CMonsterScript::Render(const Matrix& view, const Matrix& projection)
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
