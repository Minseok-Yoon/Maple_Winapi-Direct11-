#pragma once
#include "../Graphics/CGraphicsDevice_DX11.h"
#include "../Graphics/CConstantBuffer.h"

class CCamera;
class CGameObject;
namespace renderer
{
	extern CCamera* mainCamera;
	extern CCamera* uiCamera;
	extern CCamera* activeCamera;

	extern CGameObject* selectedObject;
	extern CConstantBuffer* constantBuffers[(UINT)CB_TYPE::CT_End];

	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)SAMPLE_TYPE::ST_End];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)RASTERIZER_STATE::RS_End];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)BLEND_STATE::BS_End];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)DEPTHSTENCIL_STATE::DS_End];

	void Init();
	void Release();
}