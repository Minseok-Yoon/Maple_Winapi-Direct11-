#pragma once
#include "../pch.h"
#include "../ThirdParty/FW1FontWrapper/inc/FW1FontWrapper.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

#ifdef _DEBUG
#pragma comment(lib, "../ThirdParty/FW1FontWrapper/lib/Debug/FW1FontWrapper.lib")
#else
#pragma comment(lib, "../ThirdParty/FW1FontWrapper/lib//Release/FW1FontWrapper.lib")
#endif

class CFontManager
{
	SINGLETON(CFontManager);
	friend class FontFactoryCreator;


public:
	bool Init(CGraphicDevice_DX11* _device);
	void Update();
	void Render();
	void Release();

	void DrawText(ID3D11DeviceContext* _context, const wstring& _strText, float _size,
		float _x, float _y, UINT32 _color, const wstring& fontName = L"Arial");

	static IFW1Factory* FontFactory;

private:
	IFW1Factory* m_pFW1Factory = nullptr;
	IFW1FontWrapper* m_pFontWrapper = nullptr;
};