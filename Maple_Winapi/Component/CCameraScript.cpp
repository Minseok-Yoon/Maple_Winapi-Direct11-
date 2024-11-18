#include "CCameraScript.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Object/CGameObject.h"

CCameraScript::CCameraScript()
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::Init()
{
}

void CCameraScript::Update()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	math::Vector3 vPos = tr->GetPosition();

	if (KEY_HOLD(KEY_CODE::A))
		vPos += 20.0f * -tr->Right() * DeltaTime;
	if (KEY_HOLD(KEY_CODE::W))
		vPos += 20.0f * tr->Forward() * DeltaTime;
	if (KEY_HOLD(KEY_CODE::D))
		vPos += 20.0f * tr->Right() * DeltaTime;
	if (KEY_HOLD(KEY_CODE::S))
		vPos += 20.0f * -tr->Forward() * DeltaTime;
	if (KEY_HOLD(KEY_CODE::E))
		vPos += 20.0f * tr->Up() * DeltaTime;
	if (KEY_HOLD(KEY_CODE::Q))
		vPos += 20.0f * -tr->Up() * DeltaTime;

	tr->SetPosition(vPos);
}

void CCameraScript::LateUpdate()
{
}

void CCameraScript::Render()
{
}
