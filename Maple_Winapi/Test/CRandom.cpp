#include "../pch.h"
#include "CRandom.h"

CRandom::CRandom()
{
}

CRandom::CRandom(int _iSeed)
{
}

CRandom::~CRandom()
{
}

int CRandom::RandomInt(int _iMin, int _iMax)
{
    std::uniform_int_distribution<int> RandomCreator(_iMin, _iMax);
    return RandomCreator(MtGen);
}

float CRandom::RandomFloat(float _fMin, float _fMax)
{
    std::uniform_real_distribution<float> RandomCreator(_fMin, _fMax);
    return RandomCreator(MtGen);
}
