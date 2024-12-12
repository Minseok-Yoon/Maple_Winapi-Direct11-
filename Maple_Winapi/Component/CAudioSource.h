#pragma once
#include "../Component/CComponent.h"
#include "../Resource/CAudioClip.h"

class CAudioSource final : public CComponent
{
public:
	CAudioSource();
	virtual ~CAudioSource();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void Play() const;
	void Stop() const;
	void SetLoop(bool _bLoop) const;

	void SetClip(CAudioClip* _pAudioClip) { m_pAudioClip = _pAudioClip; }
	CAudioClip* GetClip() const { return m_pAudioClip; }

private:
	CAudioClip* m_pAudioClip;
};