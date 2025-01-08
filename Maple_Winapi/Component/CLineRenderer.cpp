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
    Vector3 offset(-0.5f, 0.0f, 0.0f);  // �̵��� ����

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

    // ���� �� OutputDebugString���� ���
    char colorMessage[100];
    sprintf_s(colorMessage, "LineRenderer Color: R=%.2f, G=%.2f, B=%.2f, A=%.2f\n", m_LineColor.x, m_LineColor.y, m_LineColor.z, m_LineColor.w);
    OutputDebugStringA(colorMessage);  // ���� �� ���
}

void CLineRenderer::SetLineData(const std::vector<Vector3>& points, const Vector4& color)
{
    m_Vertices.clear();
    m_LineColor = color;

    for (const auto& point : points)
    {
        m_Vertices.push_back({ point, color }); // �ʱ�ȭ �� ���� ����
    }
}