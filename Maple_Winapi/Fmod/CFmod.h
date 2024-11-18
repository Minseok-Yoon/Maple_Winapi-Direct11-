#pragma once
#include "../pch.h"
#include "../../External/Include/Fmod/fmod_studio.hpp"
#include "../../External/Include/Fmod/fmod.hpp"
#include "../../External/Include/Fmod/fmod_common.h"
#include "../../External/Include/Fmod/fmod_codec.h"
#include "../../External/Include/Fmod/fmod_errors.h"

#ifdef _DEBUG
#pragma comment(lib,  "../External/Library/Fmod/Debug/fmodL_vc.lib")
#pragma comment(lib, "../External/Library/Fmod/Debug/fmodstudioL_vc.lib")
#else
#pragma comment(lib, "../External/Library/Fmod/Debug/fmod_vc.lib")
#pragma comment(lib, "../External/Library/Fmod/Release/fmodstudio_vc.lib")
#endif

using namespace math;
class CFmod
{
public:
    static void Init();
    static bool CreateSound(const string& _strPath, FMOD::Sound** sound);
    static void SoundPlay(FMOD::Sound* _pSound, FMOD::Channel** channel);
    static void Set3DListenerAttributes(const Vector3& _vPos);
    static void Release();

private:
    static FMOD::Studio::System* m_pSystem;
    static FMOD::System* m_pCoreSystem;
};