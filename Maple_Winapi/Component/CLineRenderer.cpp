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

    // 원하는 오프셋 값 (예: (1.0f, 1.0f, 0.0f))
    Vector3 offset(0.0f, 0.0f, 0.0f);  // 이동할 방향

    // Vertex Buffer에 데이터 업로드
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = GetDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        // 각 Vertex에 오프셋 적용
        for (auto& vertex : m_Vertices)
        {
            vertex.pos += offset;  // 위치에 오프셋 더하기
            vertex.isLine = 1.0f;
        }

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
}

//void CLineRenderer::DrawRectangle(const Vector3& _vPosition, const Vector2& _vSize, const Vector4& _vColor)
//{
//    std::vector<Vector3> points = {
//        _vPosition,
//        Vector3(_vPosition.x + _vSize.x, _vPosition.y, _vPosition.z),
//        Vector3(_vPosition.x + _vSize.x, _vPosition.y + _vSize.y, _vPosition.z),
//        Vector3(_vPosition.x, _vPosition.y + _vSize.y, _vPosition.z),
//        _vPosition
//    };
//
//    // 색상을 함께 설정하여 LineRenderer로 전달
//    this->SetLineData(points, _vColor);
//    this->Render();  // 사각형 그리기
//}

void CLineRenderer::SetShape(const Vector3& _vBottomLeft, const Vector3& _vTopRight, const Vector4& _vColor)
{
    m_Vertices.clear();

    // 사각형의 네 꼭짓점
    m_Vertices.push_back({ _vBottomLeft, _vColor });                         // 왼쪽 아래
    m_Vertices.push_back({ Vector3(_vTopRight.x, _vBottomLeft.y, -2.0f), _vColor }); // 오른쪽 아래
    m_Vertices.push_back({ _vTopRight, _vColor });                          // 오른쪽 위
    m_Vertices.push_back({ Vector3(_vBottomLeft.x, _vTopRight.y, -2.0f), _vColor }); // 왼쪽 위
    m_Vertices.push_back({ _vBottomLeft, _vColor });                        // 닫힌 선

    m_LineColor = _vColor;
}

void CLineRenderer::SetLineData(const std::vector<Vector3>& points, const Vector4& color)
{
    m_Vertices.clear();
    for (const auto& point : points)
    {
        m_Vertices.push_back({ point, color }); // 색상 설정
    }
    m_LineColor = color; // 라인 색상 설정
}