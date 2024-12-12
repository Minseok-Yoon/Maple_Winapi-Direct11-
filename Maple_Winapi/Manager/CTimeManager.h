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
	static LARGE_INTEGER	m_llCurCount;	// ���� �ð��� ��Ÿ���� ����
	static LARGE_INTEGER	m_llPrevCount;	// ���� �����ӿ����� �ð��� ��Ÿ���� ����
	static LARGE_INTEGER	m_llFrequency;

	static float m_fDeltaTime;	// ������ ���� �ð���
	double		 m_dAcc;			// 1�� üũ�� ���� ���� �ð�
	UINT		 m_iCallCount;	// �Լ� ȣ�� Ƚ�� üũ
	UINT		 m_iFPS;			// �ʴ� ȣ�� Ƚ��
};