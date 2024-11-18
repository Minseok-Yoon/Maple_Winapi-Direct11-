#include "CAudioClip.h"

CAudioClip::CAudioClip() :
    CResource(RESOURCE_TYPE::RT_AudioClip),
    m_pSound(nullptr),
    m_pChannel(nullptr),
    m_fMinDistance(1.0f),
    m_fMaxDistance(1000.0f),
    m_bLoop(false)
{
}

CAudioClip::~CAudioClip()
{
    m_pSound->release();
    m_pSound = nullptr;
}

HRESULT CAudioClip::Load(const wstring& _strPath)
{
    // 변환한 wstring을 string으로 변경
    string cPath(_strPath.begin(), _strPath.end());

    // FMOD에서 사운드 로드
    if (!CFmod::CreateSound(cPath, &m_pSound))
        return S_FALSE;

    m_pSound->set3DMinMaxDistance(m_fMinDistance, m_fMaxDistance);

    return S_OK;
}

void CAudioClip::Play()
{
    if (m_bLoop)
        m_pSound->setMode(FMOD_LOOP_NORMAL);
    else
        m_pSound->setMode(FMOD_LOOP_OFF);

    CFmod::SoundPlay(m_pSound, &m_pChannel);
}

void CAudioClip::Stop()
{
    m_pChannel->stop();
}

void CAudioClip::Set3DAttributes(const Vector3 _vPos)
{
    FMOD_VECTOR fmodPos(_vPos.x, _vPos.y, _vPos.z);
    FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);

    m_pChannel->set3DAttributes(&fmodPos, &fmodVel);
}
