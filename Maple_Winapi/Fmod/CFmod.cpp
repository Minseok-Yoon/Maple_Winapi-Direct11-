#include "CFmod.h"

FMOD::Studio::System* CFmod::m_pSystem = nullptr;
FMOD::System* CFmod::m_pCoreSystem = nullptr;

void CFmod::Init()
{
    if (m_pSystem != nullptr)
    {
        return;
    }

    void* extraDriverData = nullptr;

    FMOD::Studio::System::create(&m_pSystem);
    FMOD_RESULT result;

    m_pSystem->getCoreSystem(&m_pCoreSystem);
    m_pCoreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

    result = m_pSystem->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
    if (result != FMOD_OK) {
        std::wstringstream ws;
        ws << L"FMOD System initialization failed: " << FMOD_ErrorString(result) << L"\n";
        OutputDebugStringW(ws.str().c_str());
        return;
    }

#pragma region
    //FMOD::Studio::Bank* masterBank = NULL;
    //FMOD::Sound* sound1;
    //FMOD::Channel* channel1 = nullptr;
    ////mSystem->load(Common_MediaPath("Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank);
    //result = m_pCoreSystem->createSound("../Resources/Texture/MoonlightShadow.wav", FMOD_3D, 0, &sound1);
    //if (result != FMOD_OK || sound1 == nullptr) {
    //    std::wstringstream ws;
    //    ws << L"Failed to create sound: " << FMOD_ErrorString(result) << L"\n";
    //    OutputDebugStringW(ws.str().c_str());
    //    return;
    //}
 
    //const float DISTANCEFACTOR = 1.0f;
    //result = sound1->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
    //result = sound1->setMode(FMOD_LOOP_OFF);
    //if (result != FMOD_OK) {
    //    std::wstringstream ws;
    //    ws << L"Failed to load sound: " << FMOD_ErrorString(result) << L"\n";
    //    OutputDebugStringW(ws.str().c_str());
    //}
    //FMOD_VECTOR pos = { 0.0f * DISTANCEFACTOR, 0.0f, 10.0f };
    //FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
    //result = m_pCoreSystem->playSound(sound1, 0, false, &channel1);
    //result = channel1->set3DAttributes(&pos, &vel);
    ////result = channel1->setPaused(false);
    //static float t = 0;
    //static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
    //FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
    //FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
    //FMOD_VECTOR vel2 = { 0.0f, 0.0f, 0.0f };;
    //FMOD_VECTOR listenerpos = { 0.0f, 0.0f, 0.0f };
    //result = m_pCoreSystem->set3DListenerAttributes(0, &listenerpos, &vel2, &forward, &up);
    //SoundPlay(sound1, &channel1);
#pragma endregion
}

bool CFmod::CreateSound(const string& _strPath, FMOD::Sound** sound)
{
    if (FMOD_OK != m_pCoreSystem->createSound(_strPath.c_str(), FMOD_DEFAULT, nullptr, sound))
    {
        return false;
    }
    return true;
}

void CFmod::SoundPlay(FMOD::Sound* _pSound, FMOD::Channel** channel)
{
    m_pCoreSystem->playSound(_pSound, 0, false, channel);
}

void CFmod::Set3DListenerAttributes(const Vector3& _vPos)
{
    FMOD_VECTOR fmodPos(_vPos.x, _vPos.y, _vPos.z);
    FMOD_VECTOR fmodVel(0.0f, 0.0f, 0.0f);
    FMOD_VECTOR fmodForward(0.0f, 0.0f, 1.0f);
    FMOD_VECTOR fmodUp(0.0f, 1.0f, 0.0f);

    m_pCoreSystem->set3DListenerAttributes(0, &fmodPos, &fmodVel, &fmodForward, &fmodUp);
}

void CFmod::Release()
{
    if (m_pSystem != nullptr)
    {
        m_pSystem->release();
        m_pSystem = nullptr;
        m_pCoreSystem = nullptr;
    }
}