#pragma once
#include "../Object/CGameObject.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

class CTexture;
class CCore
{
	SINGLETON(CCore);

public:
	int Init(HWND _hWnd, POINT _ptResolution);		// 게임 엔진 초기화 함수
	void Update();
	void LateUpdate();
	void Render();

	void Destroy();
	void Release();
	void progress();

	HDC		GetMainDC() { return m_hDC; }
	HWND	GetMainHWnd() { return m_hWnd; }
	POINT	GetResolution() { return m_ptResolution; }
	HBRUSH	GetBrush(BRUSH_TYPE _eBrush) { return m_arrBrush[(UINT)_eBrush]; }
	HPEN	GetPen(PEN_TYPE _ePen) { return m_arrPen[(UINT)_ePen]; }

	bool	IsLoaded() const { return m_bLoaded; }
	void	IsLoaded(bool _bLoaded) { m_bLoaded = _bLoaded; }

private:
	void adjustWindowRect(HWND _hWnd, POINT _ptResolution);
	void createBackBuffer(POINT _ptResolution);
	//void copyRenderTarget(HDC _hBackDC, HDC _hDC);

	void clear();
	void createHBrush();
	void createHPen();

private:
	HDC			m_hDC;								// 메인 윈도우에 그릴 DC
	HWND		m_hWnd;								// 메인 윈도우 핸들
	HDC			m_hBackDC;
	HBITMAP		m_hBackBitmap;
	POINT		m_ptResolution;						// 메인 윈도우 해상도
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::BT_END];	// 브러쉬 배열 생성
	HPEN		m_arrPen[(UINT)PEN_TYPE::PT_End];		// 펜 배열 생성

	CTexture* m_pMemTex;							// 백버퍼 택스처

	bool		m_bLoaded;
	unique_ptr<CGraphicDevice_DX11>		m_GraphicDevice;
};