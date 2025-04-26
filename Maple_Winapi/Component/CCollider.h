#pragma once
#include "CComponent.h"
#include "../Object/CGameObject.h"

class CCollider : public CComponent
{
public:
	CCollider();
	virtual ~CCollider();

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	// 충돌 시점 함수
	virtual void OnCollisionEnter(class CCollider* _pOther);	// 충돌 진입 시
	virtual void OnCollisionStay(class CCollider* _pOther);		// 충돌 중
	virtual void OnCollisionExit(class CCollider* _pOther);		// 충돌 해제 시

	void RectCollider(const Vector2& scale, const Vector4& color, const Vector3& bottomLeft, const Vector3& topRight);
	void RenderUI();

	string WStringToString(const wstring& wstr);

	void SetCollisionDetected(bool _bDetected) { m_bCollisionDetected = _bDetected; }

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

	void SetColliderActive(bool _bActive) { m_bIsColliderActive = _bActive; }
	bool IsColliderActive() const { return m_bIsColliderActive; }

	CTransform* GetTransform() { return m_pTransform; }
	void SetTransform(CTransform* pTransform) { m_pTransform = pTransform; }

	void SetBottomLeft(Vector3 _vBottomLeft) { m_vBottomLeft = _vBottomLeft; }
	void SetTopRight(Vector3 _vTopRight) { m_vTopRight = _vTopRight; }

	Vector3 GetBottomLeft() { return m_vBottomLeft; }
	Vector3 GetTopRight() { return m_vTopRight; }

	// 새로 추가된 함수
	Vector3 GetWorldBottomLeft() { return m_vWorldBottomLeft; }
	Vector3 GetWorldTopRight() { return m_vWorldTopRight; }

public:
	COLLIDER_TYPE	m_eColliderType;

	static UINT		g_iNextID;
	static bool		g_bRenderColliders;

	bool			m_bIsColliderActive = false;
	bool			m_bIsColliding = false;

	Vector3			m_vCenter;		// 기준점을 중심
	Vector3			m_vBottomLeft;	// 중심으로부터 상대적인 거리(좌하단)
	Vector3			m_vTopRight;	// 중심으로부터 상대적인 거리(우상단)
	Vector3          m_vWorldBottomLeft;  // 월드 좌표계에서의 좌하단 좌표
	Vector3          m_vWorldTopRight;    // 월드 좌표계에서의 우상단 좌표
	Vector4			m_vColor;

	Vector4			m_vOriginalColor; // 원래 색상을 저장할 변수

private:
	CTransform*		m_pTransform;

	Vector2			m_vOffsetPos;	// 오브젝트로 부터 상대적인 위치
	Vector2			m_vScale;		// 충돌체의 크기

	UINT			m_iID;			// 충돌체 고유한 ID 값
	UINT			m_iCol;

	bool			m_bEnable;

	int				m_iLayer;

	string			m_strColTag;

	bool			m_bCollisionDetected;

};