 #pragma once
#include "CComponent.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

class CLineRenderer : public CComponent
{
public:
	CLineRenderer();
	virtual ~CLineRenderer();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	//void DrawRectangle(const Vector3& _vPosition, const Vector2& _vSize, const Vector4& _vColor);
	void SetShape(const Vector3& _vBottomLeft, const Vector3& _vTopRight, const Vector4& _vColor);
	void SetLineData(const std::vector<Vector3>& points, const Vector4& color);

	// 부모 따라가기 설정 메서드 추가
	void SetFollowParent(bool follow) { m_bFollowParent = follow; }
	bool IsFollowingParent() const { return m_bFollowParent; }

private:
	bool m_bFollowParent; // 부모 오브젝트를 따라다닐지 여부

	ID3D11Buffer* m_pVertexBuffer;

	std::vector<Vertex> m_Vertices; // 선분 데이터를 저장

	Vector4 m_LineColor; // 선분 색상s
};