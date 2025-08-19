#pragma once
#include "../CMath.h"

using namespace math;

//class CPixelCollider;
class CCollisionChecker
{
	struct CollisionPoint {
		Vector2 offset; // 캐릭터 중심으로부터의 상대 위치
		float priority; // 검사 우선순위
	};

public:
	void InitCollisionPoints(float width, float height);
	//bool CheckCollision(const Vector3& position);

private:
	vector<CollisionPoint> m_vecCollisionPoints;
	//CPixelCollider* m_pPixelCollider = nullptr;
};