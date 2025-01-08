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
	void Render() override;

	void SetLineData(const std::vector<Vector3>& points, const Vector4& color);

private:
	ID3D11Buffer* m_pVertexBuffer;

	std::vector<Vertex> m_Vertices; // ���� �����͸� ����

	Vector4 m_LineColor; // ���� ����s
};