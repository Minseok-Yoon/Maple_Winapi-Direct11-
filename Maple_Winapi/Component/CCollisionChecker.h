#pragma once
#include "../CMath.h"

using namespace math;

//class CPixelCollider;
class CCollisionChecker
{
	struct CollisionPoint {
		Vector2 offset; // ĳ���� �߽����κ����� ��� ��ġ
		float priority; // �˻� �켱����
	};

public:
	void InitCollisionPoints(float width, float height);
	//bool CheckCollision(const Vector3& position);

private:
	vector<CollisionPoint> m_vecCollisionPoints;
	//CPixelCollider* m_pPixelCollider = nullptr;
};