#pragma once
#include "CComponent.h"

class CCollider : public CComponent
{
public:
	CCollider();
	virtual ~CCollider();

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	// 충돌 시점 함수
	virtual void OnCollisionEnter(class CCollider* _pOther);	// 충돌 진입 시
	virtual void OnCollisionStay(class CCollider* _pOther);		// 충돌 중
	virtual void OnCollisionExit(class CCollider* _pOther);		// 충돌 해제 시

	void RectCollider();

	COLLIDER_TYPE GetColliderType() { return m_eColliderType; }

	static void ToggleRenderColliders() { g_bRenderColliders = !g_bRenderColliders; }
	static bool IsRenderCollidersEnabled() { return g_bRenderColliders; }

	void SetOffsetPos(Vector2 _vPos) { m_vOffsetPos = _vPos; }
	Vector2 GetOffsetPos() { return m_vOffsetPos; }

	void SetScale(Vector2 _vScale) { m_vScale = _vScale; }
	Vector2 GetScale() { return m_vScale; }

	void SetColTag(string _strColTag) { m_strColTag = _strColTag; }
	string GetColTag() { return m_strColTag; }

	UINT GetID() { return m_iID; }

	void Enable(bool _bEnable) { m_bEnable = _bEnable; }

	// 대기
	void SetLayer(int layer) { m_iLayer = layer; }
	int GetLayer() const { return m_iLayer; }

private:
	COLLIDER_TYPE	m_eColliderType;

	static UINT		g_iNextID;
	static bool		g_bRenderColliders;  // 충돌체를 렌더링할지 여부를 나타내는 변수

	Vector2			m_vOffsetPos;	// 오브젝트로 부터 상대적인 위치
	Vector2			m_vScale;		// 충돌체의 크기

	UINT			m_iID;			// 충돌체 고유한 ID 값

	bool			m_bEnable;

	int				m_iLayer;

	string			m_strColTag;

	Vector3			m_vBottomLeft;
	Vector3			m_vTopRight;
	Vector4			m_vColor;
};