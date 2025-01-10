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

	// �浹 ���� �Լ�
	virtual void OnCollisionEnter(class CCollider* _pOther);	// �浹 ���� ��
	virtual void OnCollisionStay(class CCollider* _pOther);		// �浹 ��
	virtual void OnCollisionExit(class CCollider* _pOther);		// �浹 ���� ��

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

	// ���
	void SetLayer(int layer) { m_iLayer = layer; }
	int GetLayer() const { return m_iLayer; }

private:
	COLLIDER_TYPE	m_eColliderType;

	static UINT		g_iNextID;
	static bool		g_bRenderColliders;  // �浹ü�� ���������� ���θ� ��Ÿ���� ����

	Vector2			m_vOffsetPos;	// ������Ʈ�� ���� ������� ��ġ
	Vector2			m_vScale;		// �浹ü�� ũ��

	UINT			m_iID;			// �浹ü ������ ID ��

	bool			m_bEnable;

	int				m_iLayer;

	string			m_strColTag;

	Vector3			m_vBottomLeft;
	Vector3			m_vTopRight;
	Vector4			m_vColor;
};