#include "CFontManager.h"
#include "../ThirdParty/FW1FontWrapper/inc/FW1FontWrapper.h"

CFontManager::CFontManager()
{
}

CFontManager::~CFontManager()
{
}

bool CFontManager::Init(CGraphicDevice_DX11* _device)
{
    if (!_device)
        return false;

    Microsoft::WRL::ComPtr<ID3D11Device> device = _device->GetID3D11Device();
    if (!device)
        return false;

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    for (int i = 0; i < 8; i++)
    {
        blendDesc.RenderTarget[i].BlendEnable = true;
        blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
    }

	HRESULT hr = FW1CreateFactory(FW1_VERSION, &m_pFW1Factory);
	if (FAILED(hr)) 
		return false;

	hr = m_pFW1Factory->CreateFontWrapper(device.Get(), L"Arial", blendDesc, &m_pFontWrapper);
	return SUCCEEDED(hr);
}

void CFontManager::Update()
{
}

void CFontManager::Render()
{
}

void CFontManager::Release()
{
    if (m_pFontWrapper)
    {
        m_pFontWrapper->Release();
        m_pFontWrapper = nullptr;
    }

    if (m_pFW1Factory)
    {
        m_pFW1Factory->Release();
        m_pFW1Factory = nullptr;
    }
}

void CFontManager::DrawText(ID3D11DeviceContext* _context, const wstring& _strText, float _size, 
    float _x, float _y, UINT32 _color, const wstring& fontName)
{
    if (!m_pFontWrapper)
    {
        OutputDebugStringA("Error: FontWrapper is NULL\n");
        return;
    }

    if (!_context)
    {
        OutputDebugStringA("Error: DeviceContext is NULL\n");
        return;
    }

    // 디버그 출력으로 매개변수 확인
    /*char debugStr[256];
    sprintf_s(debugStr, "Drawing text: position=%.1f, %.1f, size=%.1f, color=0x%08X\n", _x, _y, _size, _color);
    OutputDebugStringA(debugStr);*/

        m_pFontWrapper->DrawString(
            _context,
            _strText.c_str(),    // 문법 수정: c_str() 사용
            _size,
            _x,
            _y,    // 여러 색상 테스트 시 위치 조정
            _color,
            FW1_RESTORESTATE
        );
}

// 2025-07-14
//// 색상 형식 테스트를 위한 다양한 시도
    //UINT32 testColors[] = {
    //    _color,                                          // 원본 색상
    //    ((_color & 0xFF000000)) |                       // ARGB -> RGBA 변환
    //    ((_color & 0x00FF0000) >> 16) |
    //    ((_color & 0x0000FF00)) |
    //    ((_color & 0x000000FF) << 16),
    //    0xFFFF0000,  // 빨강
    //    0xFF00FF00,  // 초록
    //    0xFF0000FF,  // 파랑
    //    0xFFFFFFFF,   // 흰색
    //    0xFF000000,  // 검정 (ARGB)
    //    0xFFFFFF00,  // 노랑 (ARGB)
    //    0xFFFF00FF,  // 마젠타 (ARGB)
    //    0xFF00FFFF   // 시안 (ARGB)
    //};

   /* OutputDebugStringA("Drawing text at position: ");
    OutputDebugStringA(std::to_string(_x).c_str());
    OutputDebugStringA(", ");
    OutputDebugStringA(std::to_string(_y).c_str());
    OutputDebugStringA("\n");
    OutputDebugStringA("Drawing text at size: ");
    OutputDebugStringA(std::to_string(_size).c_str());
    OutputDebugStringA("\n");*/

    /*m_pFontWrapper->DrawString(
         _context,
         _strText.c_str(),
         _x, _y,
         _size,
         _color,
        FW1_RESTORESTATE
     );*/
     /*if (m_pFontWrapper)
     {
         m_pFontWrapper->DrawString(
             _context,
             _strText.c_str(),
             _x, _y,
             _size,
             _color,
             FW1_RESTORESTATE
         );
     }*/