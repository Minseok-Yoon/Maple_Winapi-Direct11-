#pragma once
#include "../Object/CGameObject.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

class CCore
{
	SINGLETON(CCore);

public:
	int Init(HWND _hWnd, UINT _iWidth, UINT _iHeight);		// ���� ���� �ʱ�ȭ �Լ�
	void Update();
	void LateUpdate();
	void Render();

	void Present();
	void Destroy();
	void Release();
	void progress();

	HWND	GetMainHWnd() { return m_hWnd; }
	
	UINT GetWidth() { return m_iWidth; }
	UINT GetHeight() { return m_iHeight; }

	HBRUSH	GetBrush(BRUSH_TYPE _eBrush) { return m_arrBrush[(UINT)_eBrush]; }
	HPEN	GetPen(PEN_TYPE _ePen) { return m_arrPen[(UINT)_ePen]; }

	bool	IsLoaded() const { return m_bLoaded; }
	void	IsLoaded(bool _bLoaded) { m_bLoaded = _bLoaded; }

private:
	void adjustWindowRect(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	//void createBackBuffer(POINT _ptResolution);
	//void copyRenderTarget(HDC _hBackDC, HDC _hDC);

	void createHBrush();
	void createHPen();

private:
	HWND		m_hWnd;								// ���� ������ �ڵ�
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::BT_END];	// �귯�� �迭 ����
	HPEN		m_arrPen[(UINT)PEN_TYPE::PT_End];		// �� �迭 ����

	UINT		m_iWidth;
	UINT		m_iHeight;

	bool		m_bLoaded;
	unique_ptr<CGraphicDevice_DX11>		m_GraphicDevice;
};