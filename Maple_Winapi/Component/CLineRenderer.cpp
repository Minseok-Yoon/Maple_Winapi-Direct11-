#include "CLineRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CMaterial.h"

CLineRenderer::CLineRenderer() : 
	CComponent(COMPONENT_TYPE::CT_LineRenderer),
	m_pVertexBuffer(nullptr), 
	m_LineColor(1.0f, 0.0f, 0.0f, 1.0f) // 기본 색상: 흰색
{
}

CLineRenderer::~CLineRenderer()
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
}

void CLineRenderer::Init()
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(Vertex) * 100; // 최대 100개의 Vertex 지원 (필요시 조정)
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer);
    if (FAILED(hr))
    {
        // 에러 처리
    }
}

void CLineRenderer::Update()
{
}

void CLineRenderer::LateUpdate()
{
}

void CLineRenderer::Render()
{
    if (m_Vertices.empty() || m_pVertexBuffer == nullptr)
        return;

    // Vertex Buffer에 데이터 업로드
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = GetDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, m_Vertices.data(), sizeof(Vertex) * m_Vertices.size());
        GetDeviceContext()->Unmap(m_pVertexBuffer, 0);
    }

    // Vertex Buffer 바인딩
    UINT stride = sizeof(Vertex);
    UINT offsetValue = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offsetValue);

    // Primitive Topology 설정 (Line Strip)
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    GetDeviceContext()->Draw((UINT)m_Vertices.size(), 0);

    // 색상 값 OutputDebugString으로 출력
    char colorMessage[100];
    sprintf_s(colorMessage, "LineRenderer Color: R=%.2f, G=%.2f, B=%.2f, A=%.2f\n", m_LineColor.x, m_LineColor.y, m_LineColor.z, m_LineColor.w);
    OutputDebugStringA(colorMessage);  // 색상 값 출력
}

void CLineRenderer::SetLineData(const std::vector<Vector3>& points, const Vector4& color)
{
    m_Vertices.clear();
    m_LineColor = color;

    for (const auto& point : points)
    {
        m_Vertices.push_back({ point, color }); // 초기화 시 색상 설정
    }
}