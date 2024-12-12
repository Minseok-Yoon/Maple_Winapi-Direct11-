#include "CAudioSource.h"
#include "CTransform.h"
#include "../Object/CGameObject.h"
#include "../Fmod/CFmod.h"

CAudioSource::CAudioSource() :
	CComponent(COMPONENT_TYPE::CT_AudioSource),
	m_pAudioClip(nullptr)
{
}

CAudioSource::~CAudioSource()
{
}

void CAudioSource::Init()
{
}

void CAudioSource::Update()
{
}

void CAudioSource::LateUpdate()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();
	Vector3 vPos = tr->GetPosition();

	//m_pAudioClip->Set3DAttributes(vPos);
}

void CAudioSource::Render()
{
}

void CAudioSource::Play() const
{
	m_pAudioClip->Play();
}

void CAudioSource::Stop() const
{
	m_pAudioClip->Stop();
}

void CAudioSource::SetLoop(bool _bLoop) const
{
	m_pAudioClip->SetLoop(_bLoop);
}
