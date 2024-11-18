#pragma once
#include "../Fmod/CFmod.h"
#include "../Resource/CResource.h"

class CAudioClip : public CResource
{
public:
	CAudioClip();
	virtual ~CAudioClip();

	virtual HRESULT Load(const wstring& _strPath) override;

	void Play();
	void Stop();
	void Set3DAttributes(const Vector3 _vPos);
	void SetLoop(bool _bLoop) { m_bLoop = _bLoop; }

private:
	FMOD::Sound*	m_pSound;
	FMOD::Channel*	m_pChannel;
	float			m_fMinDistance;
	float			m_fMaxDistance;
	bool			m_bLoop;
};