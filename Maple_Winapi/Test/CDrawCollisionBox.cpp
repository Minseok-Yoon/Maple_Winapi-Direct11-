#include "CDrawCollisionBox.h"
#include "../Component/CTransform.h"
#include "../Graphics/CConstantBuffer.h"
#include "../Component/CRenderer.h"
#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"

CConstantBuffer g_TransformBuffer(CB_TYPE::CT_Transform);
CConstantBuffer g_DebugColorBuffer(CB_TYPE::CT_Color);

vector<GameEngineDebugInfo> DebugInfo;

//void CDrawCollisionBox::DrawBox2D(const CTransform& _pTransform, float4 _fColor, CCamera* _pCamera)
//{
//	CDrawCollisionBox::DrawBox2D(_pTransform.GetWorldScale(), _pTransform.GetWorldRotation(), _pTransform.GetWorldPosition(), _fColor, _pCamera);
//}
//
//void CDrawCollisionBox::DrawBox2D(float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor, CCamera* _pCamera)
//{
//	CDrawCollisionBox::DrawMesh(L"RectMesh", _fScale, _fRotation, _fPos, _fColor, renderer::mainCamera);
//}

//void CDrawCollisionBox::DrawBox2D(const CTransform& _pTransform, Vector3 _vColor, CCamera* _pCamera)
//{
//    DrawBox2D(_pTransform.GetScale2D(), _pTransform.GetRotationZ2D(), _pTransform.GetPosition2D(), _vColor, _pCamera);
//}
//
//void CDrawCollisionBox::DrawBox2D(Vector2 _fScale, float _fRotation, Vector2 _fPos, Vector3 _vColor, CCamera* _pCamera)
//{
//    DrawMesh(L"RectMesh", _fScale, _fRotation, _fPos, _vColor, renderer::mainCamera);
//}
//
//void CDrawCollisionBox::DrawMesh(const wstring& _Mesh, Vector2 _fScale, float _fRotation, Vector2 _fPos, Vector3 _vColor, CCamera* _pCamera)
//{
//    // DirectX Device Context 가져오기
//    ID3D11DeviceContext* pContext = GetDeviceContext();
//    if (!pContext) return;
//
//    // 정점 데이터 - 직사각형 박스
//    struct Vertex
//    {
//        Vector3 position;
//        float4  color;
//    };
//
//    Vertex vertices[] =
//    {
//        { { -0.5f, -0.5f, 0.0f }, _vColor },
//        { {  0.5f, -0.5f, 0.0f }, _vColor },
//        { {  0.5f,  0.5f, 0.0f }, _vColor },
//        { { -0.5f,  0.5f, 0.0f }, _vColor },
//    };
//
//    // 인덱스 데이터 (사각형을 선으로 그리기 위한 인덱스)
//    UINT indices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
//
//    // 정점 버퍼 생성
//    D3D11_BUFFER_DESC vertexBufferDesc = {};
//    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    vertexBufferDesc.ByteWidth = sizeof(vertices);
//    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//    D3D11_SUBRESOURCE_DATA vertexData = {};
//    vertexData.pSysMem = vertices;
//
//    ID3D11Buffer* pVertexBuffer = nullptr;
//    HRESULT hr = GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
//
//    // 인덱스 버퍼 생성
//    D3D11_BUFFER_DESC indexBufferDesc = {};
//    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    indexBufferDesc.ByteWidth = sizeof(indices);
//    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//    D3D11_SUBRESOURCE_DATA indexData = {};
//    indexData.pSysMem = indices;
//
//    ID3D11Buffer* pIndexBuffer = nullptr;
//    hr = GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
//
//    // 버퍼 바인딩
//    UINT stride = sizeof(Vertex);
//    UINT offset = 0;
//    pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
//    pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//    // 월드 변환 행렬 생성
//    XMMATRIX scale = XMMatrixScaling(_fScale.x, _fScale.y, 1.0f);
//    XMMATRIX rotation = XMMatrixRotationZ(_fRotation);  // 2D 회전은 Z축 기준으로만!
//    XMMATRIX translation = XMMatrixTranslation(_fPos.x, _fPos.y, 0.0f);
//    XMMATRIX world = scale * rotation * translation;
//
//    // 쉐이더에 월드 행렬 설정
//    if (m_pTransform)
//        m_pTransform->SetWorldMatrix(world);
//
//    // 드로우 호출
//    pContext->DrawIndexed(8, 0, 0);
//
//    // 해제
//    if (pVertexBuffer) pVertexBuffer->Release();
//    if (pIndexBuffer) pIndexBuffer->Release();
//}
//
//void CDrawCollisionBox::DrawSphere2D(const CTransform& _pTransform, float4 _fColor, CCamera* _pCamera)
//{
//}
//
//void CDrawCollisionBox::DrawSphere2D(float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor, CCamera* _pCamera)
//{
//}

//void CDrawCollisionBox::DrawMesh(const wstring& _Mesh, float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor, CCamera* _pCamera)
//{
//    // DirectX Device Context 가져오기
//    ID3D11DeviceContext* pContext = GetDeviceContext();
//    if (!pContext) return;
//
//    // 정점 데이터 - 직사각형 박스
//    struct Vertex
//    {
//        Vector3 position;
//        float4  color;
//    };
//
//    Vertex vertices[] =
//    {
//        { { -0.5f, -0.5f, 0.0f }, _fColor },
//        { {  0.5f, -0.5f, 0.0f }, _fColor },
//        { {  0.5f,  0.5f, 0.0f }, _fColor },
//        { { -0.5f,  0.5f, 0.0f }, _fColor },
//    };
//
//    // 인덱스 데이터 (사각형을 선으로 그리기 위한 인덱스)
//    UINT indices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };
//
//    // 정점 버퍼 생성
//    D3D11_BUFFER_DESC vertexBufferDesc = {};
//    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    vertexBufferDesc.ByteWidth = sizeof(vertices);
//    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//    D3D11_SUBRESOURCE_DATA vertexData = {};
//    vertexData.pSysMem = vertices;
//
//    ID3D11Buffer* pVertexBuffer = nullptr;
//    HRESULT hr = GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
//
//    // 인덱스 버퍼 생성
//    D3D11_BUFFER_DESC indexBufferDesc = {};
//    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//    indexBufferDesc.ByteWidth = sizeof(indices);
//    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//    D3D11_SUBRESOURCE_DATA indexData = {};
//    indexData.pSysMem = indices;
//
//    ID3D11Buffer* pIndexBuffer = nullptr;
//    hr = GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
//
//    // 버퍼 바인딩
//    UINT stride = sizeof(Vertex);
//    UINT offset = 0;
//    pContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
//    pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//    // 월드 변환 행렬 생성
//    XMMATRIX scale = XMMatrixScaling(_fScale.X, _fScale.Y, _fScale.Z);
//    XMMATRIX rotation = XMMatrixRotationRollPitchYaw(_fRotation.X, _fRotation.Y, _fRotation.Z);
//    XMMATRIX translation = XMMatrixTranslation(_fPos.X, _fPos.Y, _fPos.Z);
//    XMMATRIX world = scale * rotation * translation;
//
//    // 쉐이더에 월드 행렬 설정
//    if(m_pTransform)
//        m_pTransform->SetWorldMatrix(world);
//
//    // 드로우 호출
//    pContext->DrawIndexed(8, 0, 0);
//
//    // 해제
//    if (pVertexBuffer) pVertexBuffer->Release();
//    if (pIndexBuffer) pIndexBuffer->Release();
//}

void CDrawCollisionBox::DrawLine(float _fStart, float _fEnd, float4 _fColor, CCamera* _pCamera)
{
}

void CDrawCollisionBox::Init()
{
	DebugInfo.resize(100);
}

void CDrawCollisionBox::Render()
{
	for (size_t i = 0; i < DebugInfo.size(); i++)
	{
		DebugInfo[i].unit.ResSetting();
		DebugInfo[i].unit.Draw();
	}

	DebugInfo.clear();
}