#pragma once
#include "../pch.h"

class CTimeManager
{
	SINGLETON(CTimeManager);

public:
	void Init();
	void Update();
	void Render();

	static float GetfDeltaTime() { return m_fDeltaTime; }

private:
	static LARGE_INTEGER	m_llCurCount;	// 현재 시간을 나타내는 변수
	static LARGE_INTEGER	m_llPrevCount;	// 이전 프레임에서의 시간을 나타내는 변수
	static LARGE_INTEGER	m_llFrequency;

	static float m_fDeltaTime;	// 프레임 간의 시간값
	double		 m_dAcc;			// 1초 체크를 위한 누적 시간
	UINT		 m_iCallCount;	// 함수 호출 횟수 체크
	UINT		 m_iFPS;			// 초당 호출 횟수
};