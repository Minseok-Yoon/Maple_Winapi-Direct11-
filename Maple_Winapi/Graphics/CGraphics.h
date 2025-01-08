#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "../pch.h"

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

#define CBSLOT_TRANSFORM		0

struct Vertex
{
	math::Vector3 pos;
	math::Vector4 color;
	math::Vector2 uv;	// texture coordinate
	float isLine;
};

struct GpuBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
	D3D11_BUFFER_DESC desc = {};

	GpuBuffer() = default;
	virtual ~GpuBuffer() = default;
};

enum class SHADER_STAGE
{
	SS_VS,		// Vertex Shader
	SS_HS,		// Hull Shader
	SS_DS,		// Domain Shader
	SS_GS,		// Gemetry Shader
	SS_PS,		// Pixel Shader
	SS_CS,		// Compute Shader
	SS_ALL,
};

enum class CB_TYPE
{
	CT_Transform,
	CT_None,
	CT_End,
};

enum class SAMPLE_TYPE
{
	ST_Point,
	ST_Linear,
	ST_Anisotropic,
	ST_PostProcess,
	ST_End,
};

enum class RENDERING_MODE
{
	RM_Opaque,
	RM_CutOut,
	RM_Transparent,
	RM_PostProcess,
	RM_End,
};

enum class TEXTURE_TYPE
{
	TT_Albedo,
	TT_Normal,
	TT_Specular,
	TT_Smoothness,
	TT_Metallic,
	TT_Sprite,
	TT_End,
};

enum class RASTERIZER_STATE
{
	RS_SolidBack,
	RS_SolidFront,
	RS_SolidNone,
	RS_WireFrame,
	RS_End,
};

enum class BLEND_STATE
{
	BS_AlphaBlend,
	BS_OneOne,
	BS_End,
};

enum class DEPTHSTENCIL_STATE
{
	DS_DepthNone,
	DS_LessEqual,
	DS_End,
};

// Constant Buffer
CBUFFER(TransformCB, CBSLOT_TRANSFORM)
{
	math::Matrix world;
	math::Matrix view;
	math::Matrix projection;
};