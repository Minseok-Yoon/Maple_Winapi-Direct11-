#include "../pch.h"
#include "CTimeManager.h"
#include "../Core/CCore.h"

LARGE_INTEGER CTimeManager::m_llFrequency = {};
LARGE_INTEGER CTimeManager::m_llPrevCount = {};
LARGE_INTEGER CTimeManager::m_llCurCount = {};
float CTimeManager::m_fDeltaTime = 0.0f;

CTimeManager::CTimeManager() :
	m_dAcc(0.),
	m_iCallCount(0)
{
}

CTimeManager::~CTimeManager()
{
}

void CTimeManager::Init()
{
	// ���� ī��Ʈ
	QueryPerformanceCounter(&m_llPrevCount);

	// �ʴ� ī��Ʈ Ƚ��
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimeManager::Update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// ���� �������� ī���ð�, ���� ������ ī���� ���� ���̸� ���Ѵ�.
	// �� '�� �����ӿ� �ɸ� �ð���'�� ���ϰ� �ʹ�.
	m_fDeltaTime = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;

	// ����ī��Ʈ ���� ���簪���� ����(�������� ����� ���ؼ�)
	m_llPrevCount = m_llCurCount;

	Render();

	// �ּ� �������� 60�̸����� �ȶ������� ����
#ifdef  _DEBUG
	if (m_fDeltaTime > (1. / 60.))
		m_fDeltaTime = (1. / 60.);
#endif //  _DEBUGG
}

// ������ �ӵ�(FPS)�� ����ϰ�, ������ ������ �ð�(DeltaTime)�� �����ϴ� ������ �Ѵ�.
void CTimeManager::Render()
{
	++m_iCallCount;
	m_dAcc += m_fDeltaTime;	// DT ����

	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;
		m_dAcc = 0.f;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DeltaTime : %f", m_iFPS, m_fDeltaTime);
		SetWindowText(CCore::GetInst()->GetMainHWnd(), szBuffer);
	}
}

// FPS�� DeltaTime�� ȭ���� ������ �Ʒ��� ����ϰ� ������, �׷��Ƚ� API(OpenGL, DirectX)�� ������� ������
// ��ǻ� �����ϱⰡ ����
// Ȥ�� ���� ����(Unity ��)���� ������ ���شٰ� �Ѵ�.