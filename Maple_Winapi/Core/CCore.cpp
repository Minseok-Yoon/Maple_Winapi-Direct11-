#include "CCore.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CColliderManager.h"
#include "../Manager/CDamageManager.h"
#include "../Component/CCamera.h"
#include "../Module/SelectGDI.h"
#include "../Resource/CTexture.h"
#include "../Fmod/CFmod.h"
#include "../Component/CRenderer.h"
#include "../Manager/CFontManager.h"

CCore::CCore() :
	m_hWnd(nullptr),
	m_iWidth(0),
	m_iHeight(0),
	m_bFullScreen(false),
	m_arrBrush{},
	m_arrPen{},
	m_bLoaded(false)
{
	createHBrush();
	createHPen();
}

CCore::~CCore()
{
	for (UINT i = 0; i < (UINT)PEN_TYPE::PT_End; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
}

int CCore::Init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hWnd = _hWnd;

	adjustWindowRect(_hWnd, _iWidth, _iHeight);

	CTimeManager::GetInst()->Init();
	CKeyManager::GetInst()->Init();

	m_GraphicDevice = make_unique<CGraphicDevice_DX11>();
	m_GraphicDevice->Initialize();

	// Manager 초기화
	CFmod::Init();
	CColliderManager::GetInst()->Init();
	CUIManager::GetInst()->Init();
	CSceneManager::Init();
	CResourceManager::Init();
	CFontManager::GetInst()->Init(m_GraphicDevice.get());
	CDamageManager::GetInst()->Init();

	char dbg[128];
	sprintf_s(dbg, "[CCore::Init] This instance: %p\n", this);
	OutputDebugStringA(dbg);

	return S_OK;
}

void CCore::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000 && (GetAsyncKeyState(VK_MENU) & 0x8000)) // Alt+Enter
	{
		ToggleFullScreen();
	}

	// Manager Update
	CTimeManager::GetInst()->Update();
	CKeyManager::GetInst()->Update();

	CSceneManager::Update();
	CDamageManager::GetInst()->Update();
	CColliderManager::GetInst()->Update();
	CUIManager::GetInst()->Update();
}

void CCore::LateUpdate()
{
	CColliderManager::GetInst()->LateUpdate();
	CUIManager::GetInst()->LateUpdate();
	CSceneManager::LateUpdate();
}

void CCore::Render()
{
	GetDevice()->ClearRenderTargetView();
	GetDevice()->ClearDepthStencilView();
	GetDevice()->BindViewPort();
	GetDevice()->BindDefaultRenderTarget();

	// Rendering
	CDamageManager::GetInst()->Render();
	CSceneManager::Render();
	CColliderManager::GetInst()->Render();
	CUIManager::GetInst()->Render();
}

void CCore::Present()
{
	GetDevice()->Present();
}

void CCore::Destroy()
{
}

void CCore::Release()
{
	CSceneManager::Release();
	CResourceManager::Release();
	CUIManager::Release();

	renderer::Release();
}

void CCore::progress()
{
	if (m_bLoaded == false)
		m_bLoaded = true;

	Update();
	LateUpdate();
	Render();

	Destroy();
}

void CCore::ToggleFullScreen()
{
	if (!m_bFullScreen)
	{
		// 현재 창 크기 저장
		GetWindowRect(m_hWnd, &m_tWindowRect);

		// 전체 화면 전환
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP); // 테두리 없는 스타일
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0,
			GetSystemMetrics(SM_CXSCREEN), // 화면 너비
			GetSystemMetrics(SM_CYSCREEN), // 화면 높이
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowWindow(m_hWnd, SW_MAXIMIZE);
	}
	else
	{
		// 창 모드로 전환
		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW); // 기본 창 스타일 복원
		SetWindowPos(m_hWnd, HWND_TOP, m_tWindowRect.left, m_tWindowRect.top,
			m_tWindowRect.right - m_tWindowRect.left, // 저장된 창 너비
			m_tWindowRect.bottom - m_tWindowRect.top, // 저장된 창 높이
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowWindow(m_hWnd, SW_NORMAL);
	}

	// 상태 토글
	m_bFullScreen = !m_bFullScreen;
}

void CCore::adjustWindowRect(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	char dbg[128];
	sprintf_s(dbg, "[Init arg] _iWidth: %u, _iHeight: %u\n", _iWidth, _iHeight);
	OutputDebugStringA(dbg);

	// 해상도에 맞게 윈도우 크기 조정
	RECT rt = { 0, 0, static_cast<LONG>(_iWidth), static_cast<LONG>(_iHeight) };	// 좌상단(0, 0) ~ 우하단(1280, 760)
	::AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	m_iWidth = rt.right - rt.left;
	m_iHeight = rt.bottom - rt.top;
	SetWidth(m_iWidth);
	SetHeight(m_iHeight);

	SetWindowPos(_hWnd, nullptr, 0, 0, m_iWidth, m_iHeight, 0);
	ShowWindow(_hWnd, true);

	char dbgs[128];
	sprintf_s(dbgs, "[adjustWindowRect FINAL] m_iWidth: %u, m_iHeight: %u\n", m_iWidth, m_iHeight);
	OutputDebugStringA(dbgs);
}

void CCore::createHBrush()
{
	// Hollow Brush
	m_arrBrush[(UINT)BRUSH_TYPE::BT_Hollow] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	// Black Brush
	m_arrBrush[(UINT)BRUSH_TYPE::BT_Black] = (HBRUSH)GetStockObject(BLACK_BRUSH);
}

void CCore::createHPen()
{
	// Red Pen
	m_arrPen[(UINT)PEN_TYPE::PT_Red] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	// Blue Pen
	m_arrPen[(UINT)PEN_TYPE::PT_Blue] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	// Green Pen
	m_arrPen[(UINT)PEN_TYPE::PT_Green] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
}