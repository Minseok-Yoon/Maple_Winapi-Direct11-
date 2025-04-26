#pragma once
#include "../CMath.h"
#include "../Component/CCamera.h"
#include "../Component/CTransform.h"

using namespace math;

struct GameEngineDebugInfo
{
	float4 color = float4::RED;
	CCamera* camera;
	//TransformData4 data;
	//CRendererUnit unit;
};

class CDrawCollisionBox
{
public:
	/*void DrawBox2D(const class CTransform& _pTransform, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);
	void DrawBox2D(float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);*/

	void DrawBox2D(const class CTransform& _pTransform, Vector3 _vColor, class CCamera* _pCamera = nullptr);
	void DrawBox2D(Vector2 _fScale, float _fRotation, Vector2 _fPos, Vector3 _vColor, class CCamera* _pCamera = nullptr);

	void DrawMesh(const wstring& _Mesh, Vector2 _fScale, float _fRotation, Vector2 _fPos, Vector3 _vColor, class CCamera* _pCamera = nullptr);

	void DrawSphere2D(const class CTransform& _pTransform, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);
	void DrawSphere2D(float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);

	//void DrawMesh(const wstring& _Mesh, float4 _fScale, float4 _fRotation, float4 _fPos, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);
	void DrawLine(float _fStart, float _fEnd, float4 _fColor = float4::RED, class CCamera* _pCamera = nullptr);

private:
	static void Init();
	static void Render();

	static class CScene* m_pCurScene;
	class CTransform* m_pTransform = nullptr;
};

class CCollisionParameter
{
public:
	class CCollider& Left;
	class CCollider& Right;
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
		CCollider& _Left,
		CCollider& _Right,
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