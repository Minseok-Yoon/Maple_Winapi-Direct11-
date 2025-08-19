#include "../pch.h"
#include "CCollisionChecker.h"
#include "CPixelCollider.h"

void CCollisionChecker::InitCollisionPoints(float width, float height)
{
	m_vecCollisionPoints.clear();

	m_vecCollisionPoints.push_back({{0, height / 2}, 1.0f });			// 발밑 중앙 (최우선)
	m_vecCollisionPoints.push_back({ {-width / 2, height / 2}, 0.8f }); // 발밑 왼쪽
	m_vecCollisionPoints.push_back({ {width / 2, height / 2}, 0.8f });  // 발밑 오른쪽
	m_vecCollisionPoints.push_back({ {0, 0}, 0.5f });					// 중앙
}

//bool CCollisionChecker::CheckCollision(const Vector3& position)
//{
//    // 우선순위 순으로 검사, 충돌 발견 시 즉시 반환
//	for (const auto& point : m_vecCollisionPoints) {
//		Vector3 checkPos = position + Vector3(point.offset.x, point.offset.y, 0);
//
//		PIXEL color = m_pPixelCollider->GetPixelColor(static_cast<int>(checkPos.x), static_cast<int>(checkPos.y));
//
//		// 색상 기준 충돌 확인
//		if ((color.r == GROUND_COLOR.R && color.g == GROUND_COLOR.G && color.b == GROUND_COLOR.B) ||
//			(color.r == FOOTHOLD_COLOR.R && color.g == FOOTHOLD_COLOR.G && color.b == FOOTHOLD_COLOR.B)) {
//			return true;
//		}
//	}
//	return false;
//}
