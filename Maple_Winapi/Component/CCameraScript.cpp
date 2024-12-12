#include "CCameraScript.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"

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
		vPos += 200.0f * -tr->Right() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::W))
		vPos += 200.0f * tr->Forward() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::D))
		vPos += 200.0f * tr->Right() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::S))
		vPos += 200.0f * -tr->Forward() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::E))
		vPos += 200.0f * tr->Up() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::Q))
		vPos += 200.0f * -tr->Up() * CTimeManager::GetfDeltaTime();

	tr->SetPosition(vPos);
}

void CCameraScript::LateUpdate()
{
}

void CCameraScript::Render()
{
}
