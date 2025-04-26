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

	// �θ� ���󰡱� ���� �޼��� �߰�
	void SetFollowParent(bool follow) { m_bFollowParent = follow; }
	bool IsFollowingParent() const { return m_bFollowParent; }

private:
	bool m_bFollowParent; // �θ� ������Ʈ�� ����ٴ��� ����

	ID3D11Buffer* m_pVertexBuffer;

	std::vector<Vertex> m_Vertices; // ���� �����͸� ����

	Vector4 m_LineColor; // ���� ����s
};