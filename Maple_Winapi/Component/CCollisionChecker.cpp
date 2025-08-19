#include "../pch.h"
#include "CCollisionChecker.h"
#include "CPixelCollider.h"

void CCollisionChecker::InitCollisionPoints(float width, float height)
{
	m_vecCollisionPoints.clear();

	m_vecCollisionPoints.push_back({{0, height / 2}, 1.0f });			// �߹� �߾� (�ֿ켱)
	m_vecCollisionPoints.push_back({ {-width / 2, height / 2}, 0.8f }); // �߹� ����
	m_vecCollisionPoints.push_back({ {width / 2, height / 2}, 0.8f });  // �߹� ������
	m_vecCollisionPoints.push_back({ {0, 0}, 0.5f });					// �߾�
}

//bool CCollisionChecker::CheckCollision(const Vector3& position)
//{
//    // �켱���� ������ �˻�, �浹 �߰� �� ��� ��ȯ
//	for (const auto& point : m_vecCollisionPoints) {
//		Vector3 checkPos = position + Vector3(point.offset.x, point.offset.y, 0);
//
//		PIXEL color = m_pPixelCollider->GetPixelColor(static_cast<int>(checkPos.x), static_cast<int>(checkPos.y));
//
//		// ���� ���� �浹 Ȯ��
//		if ((color.r == GROUND_COLOR.R && color.g == GROUND_COLOR.G && color.b == GROUND_COLOR.B) ||
//			(color.r == FOOTHOLD_COLOR.R && color.g == FOOTHOLD_COLOR.G && color.b == FOOTHOLD_COLOR.B)) {
//			return true;
//		}
//	}
//	return false;
//}
