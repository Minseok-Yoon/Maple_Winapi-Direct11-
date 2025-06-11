#pragma once
#include <random>
#include "../CMath.h"

class CRandom
{
public:
	CRandom();
	CRandom(int _iSeed);
	~CRandom();

	int RandomInt(int _iMin, int _iMax);
	float RandomFloat(float _fMin, float _fMax);

	void SetSeed(__int64 _iSeed) { MtGen = mt19937_64(_iSeed); }

private:
	mt19937_64 MtGen = mt19937_64(time(nullptr));
};

