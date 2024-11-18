#pragma once
#include "../Component/CComponent.h"
#include "../Resource/CAudioClip.h"

class CAudioSource : public CComponent
{
public:
	CAudioSource();
	~CAudioSource();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void Play();
	void Stop();
	void SetLoop(bool _bLoop);

	void SetClip(CAudioClip* _pAudioClip) { m_pAudioClip = _pAudioClip; }
	CAudioClip* GetClip() { return m_pAudioClip; }

private:
	CAudioClip* m_pAudioClip;
};