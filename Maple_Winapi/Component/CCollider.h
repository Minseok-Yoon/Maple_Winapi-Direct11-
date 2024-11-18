#pragma once
#include "CComponent.h"

class CCollider : public CComponent
{
public:
	CCollider(COLLIDER_TYPE _eColliderType);
	~CCollider();

public:
	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

	// �浹 ���� �Լ�
	virtual void OnCollisionEnter(class CCollider* _pOther);	// �浹 ���� ��
	virtual void OnCollisionStay(class CCollider* _pOther);		// �浹 ��
	virtual void OnCollisionExit(class CCollider* _pOther);		// �浹 ���� ��

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
};