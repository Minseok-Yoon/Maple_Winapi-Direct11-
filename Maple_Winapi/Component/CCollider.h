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

    void RenderUI();

    string WStringToString(const wstring& wstr);

    // 설정
    void SetOffsetPos(Vector2 v) { m_vOffsetPos = v; RebuildLocal(); }
    Vector2 GetOffsetPos() const { return m_vOffsetPos; }

    void SetScale(Vector2 size) { m_vSize = size;   RebuildLocal(); }
    Vector2 GetScale() const { return m_vSize; }

    void SetColor(const Vector4& c01) { m_vColor = c01; }
    void SetColliderActive(bool a) { m_bEnable = a; }
    bool IsColliderActive() const { return m_bEnable; }

    void SetColliderType(COLLIDER_TYPE t) { m_eColliderType = t; }
    COLLIDER_TYPE GetColliderType() const { return m_eColliderType; }

    void SetTransform(CTransform* t) { m_pTransform = t; }
    CTransform* GetTransform() const { return m_pTransform; }

    UINT GetID() const { return m_iID; }

    static void ToggleRenderColliders() { g_bRenderColliders = !g_bRenderColliders; }
    static bool IsRenderCollidersEnabled() { return g_bRenderColliders; }

    void SetBottomLeft(Vector3 _vBottomLeft) { m_vBottomLeft = _vBottomLeft; }
    void SetTopRight(Vector3 _vTopRight) { m_vTopRight = _vTopRight; }

    Vector3 GetWorldBottomLeft() { return m_vWorldBottomLeft; }
    Vector3 GetWorldTopRight() { return m_vWorldTopRight; }

    void IsCollider(bool _IsColliding) { m_bIsColliding = _IsColliding; }
    bool IsColliding() const { return m_bIsColliding; };

private:
    void RebuildLocal();                // 로컬 BL/TR 재계산
    void UpdateWorldCorners();          // 로컬 4코너 → 월드

public:
    Vector3          m_vWorldBottomLeft;  // 월드 좌표계에서의 좌하단 좌표
    Vector3          m_vWorldTopRight;    // 월드 좌표계에서의 우상단 좌표
    Vector3 m_vBottomLeft{ -0.5f,-0.5f,0 }; // 파생
    Vector3 m_vTopRight{ 0.5f, 0.5f,0 }; // 파생

private:
    // 표시/상태
    Vector4 m_vColor{ 0.f,0.6f,1.f,1.f };   // 0~1
    Vector4 m_vOriginalColor{};           // 충돌 시 임시 보관
    bool    m_bIsColliding{ false };
    bool    m_bEnable{ true };

    // 로컬 정의(센터/사이즈 + 파생: BL/TR)
    Vector3 m_vCenter{ 0,0,0 };             // 로컬 중심
    Vector2 m_vSize{ 1.f,1.f };             // 로컬 폭/높이
    Vector2 m_vOffsetPos{ 0,0 };            // 소유자 기준 오프셋(로컬)

    // 월드 코너 캐시(렌더용)
    Vector3 m_blW, m_brW, m_trW, m_tlW;

    // 시스템
    CTransform* m_pTransform{ nullptr };
    COLLIDER_TYPE m_eColliderType{ COLLIDER_TYPE::CT_Rect2D };
    UINT          m_iID;

    Vector2			m_vScale;		// 충돌체의 크기
    bool			m_bIsColliderActive = false;

    // 정적
    static UINT  g_iNextID;
    static bool  g_bRenderColliders;

//	void RectCollider(const Vector2& scale, const Vector4& color, const Vector3& bottomLeft, const Vector3& topRight);
//	void RenderUI();
//
//	string WStringToString(const wstring& wstr);
//
//	void SetCollisionDetected(bool _bDetected) { m_bCollisionDetected = _bDetected; }
//
//	COLLIDER_TYPE GetColliderType() { return m_eColliderType; }
//
//	static void ToggleRenderColliders() { g_bRenderColliders = !g_bRenderColliders; }
//	static bool IsRenderCollidersEnabled() { return g_bRenderColliders; }
//
//	void SetOffsetPos(Vector2 _vPos) { m_vOffsetPos = _vPos; }
//	Vector2 GetOffsetPos() { return m_vOffsetPos; }
//
//	void SetScale(Vector2 _vScale) { m_vScale = _vScale; }
//	Vector2 GetScale() { return m_vScale; }
//
//	void SetColTag(string _strColTag) { m_strColTag = _strColTag; }
//	string GetColTag() { return m_strColTag; }
//
//	UINT GetID() { return m_iID; }
//
//	void ColliderEnable(bool _bEnable) { m_bEnable = _bEnable; }
//
//	// 대기
//	void SetLayer(int layer) { m_iLayer = layer; }
//	int GetLayer() const { return m_iLayer; }
//
//	void SetColliderActive(bool _bActive) { m_bIsColliderActive = _bActive; }
//	bool IsColliderActive() const { return m_bIsColliderActive; }
//
//	CTransform* GetTransform() { return m_pTransform; }
//	void SetTransform(CTransform* pTransform) { m_pTransform = pTransform; }
//
//	void SetBottomLeft(Vector3 _vBottomLeft) { m_vBottomLeft = _vBottomLeft; }
//	void SetTopRight(Vector3 _vTopRight) { m_vTopRight = _vTopRight; }
//
//	Vector3 GetBottomLeft() { return m_vBottomLeft; }
//	Vector3 GetTopRight() { return m_vTopRight; }
//
//	// 새로 추가된 함수
//	Vector3 GetWorldBottomLeft() { return m_vWorldBottomLeft; }
//	Vector3 GetWorldTopRight() { return m_vWorldTopRight; }
//
//	void IsCollider(bool _IsColliding) { m_bIsColliding = _IsColliding; }
//	bool IsColliding() const { return m_bIsColliding; }
//
//public:
//	COLLIDER_TYPE	m_eColliderType;
//
//	static UINT		g_iNextID;
//	static bool		g_bRenderColliders;
//
//	bool			m_bIsColliderActive = false;
//	bool			m_bIsColliding = false;
//
//	Vector3			m_vCenter;		// 기준점을 중심
//	Vector3			m_vBottomLeft;	// 중심으로부터 상대적인 거리(좌하단)
//	Vector3			m_vTopRight;	// 중심으로부터 상대적인 거리(우상단)
//	Vector3          m_vWorldBottomLeft;  // 월드 좌표계에서의 좌하단 좌표
//	Vector3          m_vWorldTopRight;    // 월드 좌표계에서의 우상단 좌표
//	Vector4			m_vColor;
//
//	Vector4			m_vOriginalColor; // 원래 색상을 저장할 변수
//
//private:
//	CTransform*		m_pTransform;
//
//	Vector2			m_vOffsetPos;	// 오브젝트로 부터 상대적인 위치
//	Vector2			m_vScale;		// 충돌체의 크기
//
//	UINT			m_iID;			// 충돌체 고유한 ID 값
//	UINT			m_iCol;
//
//	bool			m_bEnable;
//
//	int				m_iLayer;
//
//	string			m_strColTag;
//
//	bool			m_bCollisionDetected;

};