#pragma once
#include "../CMath.h"
#include "../CMath4.h"

class CDrawCollisionBox
{
	void DrawBox2D(const class CTransform& _pTransform, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);
	void DrawBox2D(float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);

	void DrawSphere2D(const class CTransform& _pTransform, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);
	void DrawSphere2D(float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);

	void DrawMesh(const string_view& _Mesh, float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);
	void DrawLine(float _fStart, float _fEnd, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);

private:
	static void Init();
	static void Render();

	static class CScene* m_pCurScene;
};

class CCollisionData
{
public:
	union
	{
		DirectX::BoundingSphere			CIRCLE;		// pos pos pos scale (16Byte)
		DirectX::BoundingBox			AABB;		// pos pos pos scale scale scale(24Byte)
		DirectX::BoundingOrientedBox	OBB;		// pos pos pos pos pos scale scale scale scale scale(40Byte)
	};

	CCollisionData()
		: OBB()
	{}
};

class CCollisionParameter
{
public:
	CCollisionData& Left;
	CCollisionData& Right;
	COLLIDER_TYPE LeftType = COLLIDER_TYPE::CT_AABB2D;
	COLLIDER_TYPE RightType = COLLIDER_TYPE::CT_AABB2D;

	inline int GetLeftTypeToInt() const
	{
		return static_cast<int>(LeftType);
	}

	inline int GetRightTypeToInt() const
	{
		return static_cast<int>(RightType);
	}

	CCollisionParameter(
		CCollisionData& _Left,
		CCollisionData& _Right,
		COLLIDER_TYPE _LeftType = COLLIDER_TYPE::CT_AABB2D,
		COLLIDER_TYPE _RightType = COLLIDER_TYPE::CT_AABB2D
	)
		:
		Left(_Left),
		Right(_Right),
		LeftType(_LeftType),
		RightType(_RightType)
	{
	}
};