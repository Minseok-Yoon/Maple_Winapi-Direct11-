#include "CAudioListener.h"
#include "CTransform.h"
#include "../Object/CGameObject.h"
#include "../Fmod/CFmod.h"

CAudioListener::CAudioListener() :
	CComponent(COMPONENT_TYPE::CT_AudioListener)
{
}

CAudioListener::~CAudioListener()
{
}

void CAudioListener::Init()
{
}

void CAudioListener::Update()
{
}

void CAudioListener::LateUpdate()
{
	CTransform* tr = nullptr;
	tr = GetOwner()->GetComponent<CTransform>();

	Vector3 vPos;
	CFmod::Set3DListenerAttributes(vPos);
}

void CAudioListener::Render()
{
}
