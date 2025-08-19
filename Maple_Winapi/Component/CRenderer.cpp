#include "CRenderer.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

#include "../Manager/CResourceManager.h"
#include "../Resource/CMesh.h"
#include "../Resource/CShader.h"
#include "../Resource/CMaterial.h"

namespace renderer
{
	CCamera* mainCamera = nullptr;
	CCamera* uiCamera = nullptr;
	CCamera* activeCamera = nullptr; // 현재 렌더링할 카메라

	CGameObject* selectedObject = nullptr;
	CConstantBuffer* constantBuffers[static_cast<UINT>(CB_TYPE::CT_End)] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_End)] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[static_cast<UINT>(RASTERIZER_STATE::RS_End)] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[static_cast<UINT>(BLEND_STATE::BS_End)] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[static_cast<UINT>(DEPTHSTENCIL_STATE::DS_End)] = {};

	void LoadStates()
	{
#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, 
			samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_Anisotropic)].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_Point)].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_Linear)].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_PostProcess)].GetAddressOf());

		GetDevice()->BindSamplers((UINT)SAMPLE_TYPE::ST_Point, 1, samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_Point)].GetAddressOf());
		GetDevice()->BindSamplers((UINT)SAMPLE_TYPE::ST_Linear, 1, samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_Linear)].GetAddressOf());
		GetDevice()->BindSamplers((UINT)SAMPLE_TYPE::ST_Anisotropic, 1, samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_Anisotropic)].GetAddressOf());
		GetDevice()->BindSamplers((UINT)SAMPLE_TYPE::ST_PostProcess, 1, samplerStates[static_cast<UINT>(SAMPLE_TYPE::ST_PostProcess)].GetAddressOf());
#pragma endregion
#pragma region rasterize state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.AntialiasedLineEnable = false;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rsDesc.DepthBias = 0;
		rsDesc.DepthBiasClamp = 0.0f;
		rsDesc.DepthClipEnable = true;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.MultisampleEnable = false;
		rsDesc.ScissorEnable = false;
		rsDesc.SlopeScaledDepthBias = 0.0f;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[static_cast<UINT>(RASTERIZER_STATE::RS_SolidBack)].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[static_cast<UINT>(RASTERIZER_STATE::RS_SolidFront)].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[static_cast<UINT>(RASTERIZER_STATE::RS_SolidNone)].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[static_cast<UINT>(RASTERIZER_STATE::RS_WireFrame)].GetAddressOf());
#pragma endregion
#pragma region blend state
		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.RenderTarget[0].BlendEnable = FALSE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[static_cast<UINT>(BLEND_STATE::BS_Opaque)].GetAddressOf());

		bsDesc = {};
		bsDesc.RenderTarget[0].BlendEnable = FALSE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[static_cast<UINT>(BLEND_STATE::BS_Cutout)].GetAddressOf());

		bsDesc = {};
		bsDesc.RenderTarget[0].BlendEnable = TRUE;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[static_cast<UINT>(BLEND_STATE::BS_Transparent)].GetAddressOf());

		bsDesc = {};
		bsDesc.RenderTarget[0].BlendEnable = TRUE;

		// 색상 블렌딩
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		// 알파 블렌딩 (보통 무시)
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		// 출력 마스크: RGBA 다 써도 된다
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[static_cast<UINT>(BLEND_STATE::BS_OneOne)].GetAddressOf());
#pragma endregion
#pragma region depthstencil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = FALSE;
		GetDevice()->CreateDepthStencilState(
			&dsDesc, depthStencilStates[static_cast<UINT>(DEPTHSTENCIL_STATE::DS_LessEqual)].GetAddressOf());

		dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(
			&dsDesc, depthStencilStates[static_cast<UINT>(DEPTHSTENCIL_STATE::DS_DepthNone)].GetAddressOf());

		dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(
			&dsDesc, depthStencilStates[static_cast<UINT>(DEPTHSTENCIL_STATE::DS_Always)].GetAddressOf());
#pragma endregion
	}

	void LoadTriangleMesh()
	{
		CMesh* mesh = new CMesh();

		vector<Vertex> vertexes = {};
		vector<UINT> indices = {};

		vertexes.resize(3);
		vertexes[0].pos = Vector3(0.0f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[2] = {};
		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		CShader* triangleShader = CResourceManager::Find<CShader>(L"TriangleShader");
		mesh->SetVertexBufferParams(2, inputLayoutDesces, triangleShader->GetVSBlob()->GetBufferPointer(), triangleShader->GetVSBlob()->GetBufferSize());

		mesh->CreateVB(vertexes);
		mesh->CreateIB(indices);

		CResourceManager::Insert(L"TriangleMesh", mesh);
	}

	void LoadRectMesh()
	{
		CMesh* mesh = new CMesh();

		std::vector<Vertex> vertexes = {};
		std::vector<UINT> indices = {};

		vertexes.resize(4);
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);
		vertexes[0].isLine = 0.0f;  // 선분 여부 설정

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);
		vertexes[1].isLine = 0.0f;  // 선분 여부 설정

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);
		vertexes[2].isLine = 0.0f;  // 선분 여부 설정

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);
		vertexes[3].isLine = 0.0f;  // 선분 여부 설정

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[4] = {};  // 배열 크기 4로 확장

		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		inputLayoutDesces[2].AlignedByteOffset = 28;
		inputLayoutDesces[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputLayoutDesces[2].InputSlot = 0;
		inputLayoutDesces[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[2].SemanticName = "TEXCOORD";
		inputLayoutDesces[2].SemanticIndex = 0;  // 첫 번째 텍스처 좌표

		inputLayoutDesces[3].AlignedByteOffset = 36;  // isLine의 오프셋 (기존 오프셋에 4바이트 추가)
		inputLayoutDesces[3].Format = DXGI_FORMAT_R32_FLOAT;
		inputLayoutDesces[3].InputSlot = 0;
		inputLayoutDesces[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[3].SemanticName = "TEXCOORD";  // 세만틱 이름을 "TEXCOORD"로 사용
		inputLayoutDesces[3].SemanticIndex = 1;  // "TEXCOORD1"로 설정

		CShader* spriteShader = CResourceManager::Find<CShader>(L"Sprite-Default-Shader");
		mesh->SetVertexBufferParams(4, inputLayoutDesces, spriteShader->GetVSBlob()->GetBufferPointer(), spriteShader->GetVSBlob()->GetBufferSize());

		mesh->CreateVB(vertexes);
		mesh->CreateIB(indices);

		CResourceManager::Insert(L"RectMesh", mesh);
	}

	void LoadMeshes()
	{
		LoadTriangleMesh();
		LoadRectMesh();
	}

	void LoadShaders()
	{
		CResourceManager::Load<CShader>(L"TriangleShader", L"..\\Shader\\Triangle");
		CResourceManager::Load<CShader>(L"Sprite-Default-Shader", L"..\\Shader\\Sprite-Default");
		CResourceManager::Load<CShader>(L"WireframeShader", L"..\\Shaders\\Wireframe");
	}

	void LoadMaterials()
	{
		auto triangleMaterial = new CMaterial();
		triangleMaterial->SetShader(CResourceManager::Find<CShader>(L"TriangleShader"));
		CResourceManager::Insert(L"TriangleMaterial", triangleMaterial);

		auto* spriteMaterial = new CMaterial();
		spriteMaterial->SetShader(CResourceManager::Find<CShader>(L"Sprite-Default-Shader"));
		CResourceManager::Insert(L"Sprite-Default-Material", spriteMaterial);

		auto* spriteBorderMaterial = new CMaterial();
		spriteBorderMaterial->SetShader(CResourceManager::Find<CShader>(L"Sprite-Border-Material"));
		CResourceManager::Insert(L"Sprite-Border-Material", spriteBorderMaterial);
	}

	void LoadConstantBuffers()
	{
		constantBuffers[CBSLOT_TRANSFORM] = new CConstantBuffer(CB_TYPE::CT_Transform);
		constantBuffers[CBSLOT_TRANSFORM]->Create(sizeof(TransformCB));

		constantBuffers[CBSLOT_COLOR] = new CConstantBuffer(CB_TYPE::CT_Color);
		constantBuffers[CBSLOT_COLOR]->Create(sizeof(ColorCB));
	}

	void Init()
	{
		LoadStates();
		LoadShaders();
		LoadMeshes();
		LoadMaterials();
		LoadConstantBuffers();
	}

	void Release()
	{
		for (UINT i = 0; i < (UINT)CB_TYPE::CT_End; i++)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
	}
}

// 2025-06-11 렌더링 파이프라인 문제점
// 1. 텍스처가 없을 때, 명시적으로 투명 1x1 텍스처 바인딩
// 2. 알파 블렌딩이 제대로 설정되지 않은 경우 - 가까이 가면 블렌딩 순서 문제로 Z-buffer에 의해서 알파가 무시되고
// 불투명하게 보이는 경우. 또는 ID3D11BlendState 설정이 잘못되어 있음.
// 3. Z-Buffer와 투명 오브젝트의 그리기 순서 - 그리기 순서를 카메라로부터 먼 것부터 가까운 순으로 수동 정렬해서 그려야 한다.
