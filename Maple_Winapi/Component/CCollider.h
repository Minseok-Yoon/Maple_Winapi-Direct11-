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

	// �浹 ���� �Լ�
	virtual void OnCollisionEnter(class CCollider* _pOther);	// �浹 ���� ��
	virtual void OnCollisionStay(class CCollider* _pOther);		// �浹 ��
	virtual void OnCollisionExit(class CCollider* _pOther);		// �浹 ���� ��

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

	// ���
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

	// ���� �߰��� �Լ�
	Vector3 GetWorldBottomLeft() { return m_vWorldBottomLeft; }
	Vector3 GetWorldTopRight() { return m_vWorldTopRight; }

public:
	COLLIDER_TYPE	m_eColliderType;

	static UINT		g_iNextID;
	static bool		g_bRenderColliders;

	bool			m_bIsColliderActive = false;
	bool			m_bIsColliding = false;

	Vector3			m_vCenter;		// �������� �߽�
	Vector3			m_vBottomLeft;	// �߽����κ��� ������� �Ÿ�(���ϴ�)
	Vector3			m_vTopRight;	// �߽����κ��� ������� �Ÿ�(����)
	Vector3          m_vWorldBottomLeft;  // ���� ��ǥ�迡���� ���ϴ� ��ǥ
	Vector3          m_vWorldTopRight;    // ���� ��ǥ�迡���� ���� ��ǥ
	Vector4			m_vColor;

	Vector4			m_vOriginalColor; // ���� ������ ������ ����

private:
	CTransform*		m_pTransform;

	Vector2			m_vOffsetPos;	// ������Ʈ�� ���� ������� ��ġ
	Vector2			m_vScale;		// �浹ü�� ũ��

	UINT			m_iID;			// �浹ü ������ ID ��
	UINT			m_iCol;

	bool			m_bEnable;

	int				m_iLayer;

	string			m_strColTag;

	bool			m_bCollisionDetected;

};