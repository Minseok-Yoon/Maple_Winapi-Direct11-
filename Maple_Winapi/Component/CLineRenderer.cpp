#include "CLineRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CMaterial.h"
#include "../Object/CGameObject.h"
#include "../Component/CTransform.h"
#include "CCamera.h"

CLineRenderer::CLineRenderer() : 
	CComponent(COMPONENT_TYPE::CT_LineRenderer),
	m_pVertexBuffer(nullptr),
	m_LineColor(1.0f, 0.0f, 0.0f, 1.0f), // 기본 색상: 흰색
    m_bFollowParent(false)
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

void CLineRenderer::Render(const Matrix& view, const Matrix& projection)
{
    if(m_Vertices.empty() || m_pVertexBuffer == nullptr)
        return;

    // Vertex Buffer에 데이터 업로드
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = GetDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        // 로컬 좌표를 사용
        for (auto& vertex : m_Vertices)
        {
            vertex.isLine = 1.0f;
        }
        memcpy(mappedResource.pData, m_Vertices.data(), sizeof(Vertex) * m_Vertices.size());
        GetDeviceContext()->Unmap(m_pVertexBuffer, 0);
    }

    // 중요: 변환 행렬 바인딩
    //// 이 오브젝트의 Transform 컴포넌트를 사용하여 월드 변환 행렬을 셰이더에 바인딩
    //CTransform* transform = GetOwner()->GetComponent<CTransform>();
    //transform->Bind();

    // Vertex Buffer 바인딩
    UINT stride = sizeof(Vertex);
    UINT offsetValue = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offsetValue);
    // Primitive Topology 설정 (Line Strip)
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    GetDeviceContext()->Draw((UINT)m_Vertices.size(), 0);
}

void CLineRenderer::SetShape(const Vector3& _vBottomLeft, const Vector3& _vTopRight, const Vector4& _vColor)
{
    /*m_Vertices.clear();

    // 사각형의 네 꼭짓점
    m_Vertices.push_back({ _vBottomLeft, _vColor });                         // 왼쪽 아래
    m_Vertices.push_back({ Vector3(_vTopRight.x, _vBottomLeft.y, -2.0f), _vColor }); // 오른쪽 아래
    m_Vertices.push_back({ _vTopRight, _vColor });                          // 오른쪽 위
    m_Vertices.push_back({ Vector3(_vBottomLeft.x, _vTopRight.y, -2.0f), _vColor }); // 왼쪽 위
    m_Vertices.push_back({ _vBottomLeft, _vColor });                        // 닫힌 선

    m_LineColor = _vColor;*/
    m_Vertices.clear();

    // 사각형의 네 꼭짓점
    m_Vertices.push_back({ _vBottomLeft, _vColor });                              // 왼쪽 아래
    m_Vertices.push_back({ Vector3(_vTopRight.x, _vBottomLeft.y, -2.0f), _vColor }); // 오른쪽 아래
    m_Vertices.push_back({ _vTopRight, _vColor });                               // 오른쪽 위
    m_Vertices.push_back({ Vector3(_vBottomLeft.x, _vTopRight.y, -2.0f), _vColor }); // 왼쪽 위
    m_Vertices.push_back({ _vBottomLeft, _vColor });                             // 닫힌 선

    m_LineColor = _vColor;
}

void CLineRenderer::SetLineData(const std::vector<Vector3>& points, const Vector4& color)
{
    m_Vertices.clear();

    // 로컬 좌표로 저장 (변환하지 않음)
    for (const auto& point : points)
    {
        m_Vertices.push_back({ point, color });
    }

    m_LineColor = color;
}

//if (m_Vertices.empty() || m_pVertexBuffer == nullptr)
   //    return;

   //std::vector<Vertex> renderVertices = m_Vertices; // 원본 정점 데이터 복사

   //// 부모의 변환이 여전히 적용되지 않도록 확인
   //if (m_bFollowParent)
   //{
   //    // 부모의 Transform 컴포넌트 가져오기
   //    CTransform* parentTransform = GetOwner()->GetComponent<CTransform>();
   //    if (parentTransform)
   //    {
   //        // 부모의 월드 변환 행렬 가져오기
   //        Matrix worldMatrix = parentTransform->GetWorldMatrix();

   //        // 각 정점에 월드 변환 적용
   //        for (auto& vertex : renderVertices)
   //        {
   //            vertex.pos = Vector3::Transform(vertex.pos, worldMatrix);
   //        }
   //    }
   //}
   //else
   //{
   //    // 부모 변환을 따르지 않음
   //    for (auto& vertex : renderVertices)
   //    {
   //        // 로컬 좌표에서만 처리
   //        vertex.pos = vertex.pos;  // No transformation
   //    }
   //}

   //// 원하는 오프셋 값 (예: (1.0f, 1.0f, 0.0f))
   //Vector3 offset(0.0f, 0.0f, 0.0f);  // 이동할 방향
   //// Vertex Buffer에 데이터 업로드
   //D3D11_MAPPED_SUBRESOURCE mappedResource;
   //HRESULT hr = GetDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
   //if (SUCCEEDED(hr))
   //{
   //    // 각 Vertex에 오프셋 적용
   //    for (auto& vertex : m_Vertices)
   //    {
   //        //vertex.pos += offset;  // 위치에 오프셋 더하기
   //        vertex.isLine = 1.0f;
   //    }
   //    memcpy(mappedResource.pData, m_Vertices.data(), sizeof(Vertex) * m_Vertices.size());
   //    GetDeviceContext()->Unmap(m_pVertexBuffer, 0);
   //}

   //// Vertex Buffer 바인딩
   //UINT stride = sizeof(Vertex);
   //UINT offsetValue = 0;
   //GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offsetValue);
   //// Primitive Topology 설정 (Line Strip)
   //GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
   //GetDeviceContext()->Draw((UINT)m_Vertices.size(), 0);

// 플레이어 충돌박스 구현을 LineRenderer로 구현을 했고, 그것을 부모-자식 관계로 만들었다.
// 부모-자식이 설정되어 있는 상태로 line오브젝트를 만드니 그것 역시 부모-자식으로 인식이 되어 
// 가만히 있어야 되는 선분이 플레이어를 따라서 똑같이 움직이는 것이다.

// 가설)그럼 여기서 몬스터 오브젝트를 만들고 나서 움직이면 왜 안움직이나 싶었는데 
// 몬스터를 부모로 인식해서 그런게 아닌가 싶어서 선분을 먼저 만들고 플레이어를 생성하면 어떻게 되나
// 싶어서 선분을 먼저 만들었는데 오류가 발생한다.

// 클립 공간(View 공간)에서 그려지는지 확인 필요
// 만약 LineRenderer가 스크린 공간(UI처럼 동작)ㅇ서 그려진다면 카메라가 움직여도 화면에 고정됨.