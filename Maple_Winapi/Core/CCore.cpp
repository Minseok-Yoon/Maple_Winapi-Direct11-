#include "CCore.h"
#include "../Manager/CTimeManager.h"
//#include "../Manager/CPathManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CCollisionManager.h"
#include "../Manager/CDamageManager.h"
#include "../Component/CCamera.h"
#include "../Module/SelectGDI.h"
#include "../Resource/CTexture.h"
#include "../Fmod/CFmod.h"
#include "../Component/CRenderer.h"

CCore::CCore() :
	m_hWnd(nullptr),
	m_hDC(nullptr),
	m_ptResolution{},
	m_hBackBitmap(nullptr),
	m_hBackDC(nullptr),
	m_arrBrush{},
	m_arrPen{},
	m_bLoaded(false)
{
	createHBrush();
	createHPen();
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	for (UINT i = 0; i < (UINT)PEN_TYPE::PT_End; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
}

int CCore::Init(HWND _hWnd, POINT _ptResolution)
{
	adjustWindowRect(_hWnd, _ptResolution);
	//createBackBuffer(_ptResolution);

	m_GraphicDevice = make_unique<CGraphicDevice_DX11>();
	m_GraphicDevice->Initialize();

	// Manager 초기화
	CFmod::Init();
	CColliderManager::GetInst()->Init();
	CUIManager::GetInst()->Init();
	//CPathManager::GetInst()->Init();
	CTimeManager::GetInst()->Init();
	CKeyManager::GetInst()->Init();
	CSceneManager::GetInst()->Init();
	CDamageManager::GetInst()->Init();

	return S_OK;
}

void CCore::Update()
{
	// Manager Update
	CTimeManager::GetInst()->Update();
	CKeyManager::GetInst()->Update();

	CSceneManager::GetInst()->Update();
	CDamageManager::GetInst()->Update();
	CColliderManager::GetInst()->Update();
	CUIManager::GetInst()->Update();
}

void CCore::LateUpdate()
{
	CColliderManager::GetInst()->LateUpdate();
	CUIManager::GetInst()->LateUpdate();
	CSceneManager::GetInst()->LateUpdate();
}

void CCore::Render()
{
	clear();

	GetDevice()->ClearRenderTargetView();
	GetDevice()->ClearDepthStencilView();
	GetDevice()->BindViewPort();
	GetDevice()->BindDefaultRenderTarget();

	// Rendering
	CSceneManager::GetInst()->Render();
	CDamageManager::GetInst()->Render(m_hBackDC);
	CColliderManager::GetInst()->Render();
	CUIManager::GetInst()->Render(m_hBackDC);
	
	GetDevice()->Present();
	//copyRenderTarget(m_hBackDC, m_hDC);
}

void CCore::Destroy()
{
	//CSceneManager::Destroy();
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

	float fDeltaTimes = CTimeManager::GetInst()->GetfDeltaTime();
	Update();
	LateUpdate();
	Render();

	Destroy();

	//CEventManager::GetInst()->Update();
}

void CCore::adjustWindowRect(HWND _hWnd, POINT _ptResolution)
{
	// main.cpp에서 width와 Height 값을 받아온다. _hwnd = g_hWnd, _ptResolution = POINT { 1280, 760 }
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hDC = GetDC(_hWnd);

	// 해상도에 맞게 윈도우 크기 조정
	RECT rt = { 0, 0, _ptResolution.x, _ptResolution.y };	// 좌상단(0, 0) ~ 우하단(1280, 760)
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(_hWnd, true);
}

void CCore::createBackBuffer(POINT _ptResolution)
{
	// 백버퍼 생성
	m_hBackBitmap = CreateCompatibleBitmap(m_hDC, _ptResolution.x, _ptResolution.y);
	m_hBackDC = CreateCompatibleDC(m_hDC);

	// 백버퍼 비트맵을 설정
	HBITMAP oldBitMap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);
	DeleteObject(oldBitMap);
#pragma region
	//// 백버퍼 크기 확인을 위한 코드 추가
	//BITMAP bitmap;
	//GetObject(m_hBackBitmap, sizeof(BITMAP), &bitmap);

	//// 출력 메시지를 문자열로 변환하여 디버그 출력 창에 보낸다
	//char debugMsg[128];
	//sprintf_s(debugMsg, "백버퍼 크기: 너비 = %d, 높이 = %d\n", bitmap.bmWidth, bitmap.bmHeight);
	//OutputDebugStringA(debugMsg);
#pragma endregion
}

//void CCore::copyRenderTarget(HDC _hBackDC, HDC _hDC)
//{
//	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
//		m_hBackDC, 0, 0, SRCCOPY);
//}

void CCore::clear()
{
	SelectGDI gdi(m_hBackDC, BRUSH_TYPE::BT_Black);
	::Rectangle(m_hBackDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
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