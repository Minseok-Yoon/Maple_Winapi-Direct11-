#include "CLineRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CMaterial.h"

CLineRenderer::CLineRenderer() : 
	CComponent(COMPONENT_TYPE::CT_LineRenderer),
	m_pVertexBuffer(nullptr), 
	m_LineColor(1.0f, 0.0f, 0.0f, 1.0f) // �⺻ ����: ���
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
    bufferDesc.ByteWidth = sizeof(Vertex) * 100; // �ִ� 100���� Vertex ���� (�ʿ�� ����)
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = GetDevice()->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer);
    if (FAILED(hr))
    {
        // ���� ó��
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

    // ���ϴ� ������ �� (��: (1.0f, 1.0f, 0.0f))
    Vector3 offset(0.0f, 0.0f, 0.0f);  // �̵��� ����

    // Vertex Buffer�� ������ ���ε�
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = GetDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        // �� Vertex�� ������ ����
        for (auto& vertex : m_Vertices)
        {
            vertex.pos += offset;  // ��ġ�� ������ ���ϱ�
            vertex.isLine = 1.0f;
        }

        memcpy(mappedResource.pData, m_Vertices.data(), sizeof(Vertex) * m_Vertices.size());
        GetDeviceContext()->Unmap(m_pVertexBuffer, 0);
    }

    // Vertex Buffer ���ε�
    UINT stride = sizeof(Vertex);
    UINT offsetValue = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offsetValue);

    // Primitive Topology ���� (Line Strip)
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
//    // ������ �Բ� �����Ͽ� LineRenderer�� ����
//    this->SetLineData(points, _vColor);
//    this->Render();  // �簢�� �׸���
//}

void CLineRenderer::SetShape(const Vector3& _vBottomLeft, const Vector3& _vTopRight, const Vector4& _vColor)
{
    m_Vertices.clear();

    // �簢���� �� ������
    m_Vertices.push_back({ _vBottomLeft, _vColor });                         // ���� �Ʒ�
    m_Vertices.push_back({ Vector3(_vTopRight.x, _vBottomLeft.y, -2.0f), _vColor }); // ������ �Ʒ�
    m_Vertices.push_back({ _vTopRight, _vColor });                          // ������ ��
    m_Vertices.push_back({ Vector3(_vBottomLeft.x, _vTopRight.y, -2.0f), _vColor }); // ���� ��
    m_Vertices.push_back({ _vBottomLeft, _vColor });                        // ���� ��

    m_LineColor = _vColor;
}

void CLineRenderer::SetLineData(const std::vector<Vector3>& points, const Vector4& color)
{
    m_Vertices.clear();
    for (const auto& point : points)
    {
        m_Vertices.push_back({ point, color }); // ���� ����
    }
    m_LineColor = color; // ���� ���� ����
}