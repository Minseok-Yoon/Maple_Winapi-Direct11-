#pragma once
#include "../Component/CScript.h"
#include "../Component/CTransform.h"

class CCameraScript : public CScript
{
public:
	CCameraScript();
	virtual ~CCameraScript();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	float Clamp(float value, float minVal, float maxVal);
	Vector3 ClampCameraPosition(const Vector3& _vPos);

	void SetTarget(CGameObject* target) 
	{ 
		m_pTarget = target;

		std::wstring msg = L"[�����] SetTarget ȣ���. ���: ";
		msg += (m_pTarget ? m_pTarget->GetName() : L"����");
		msg += L"\n";

		OutputDebugStringW(msg.c_str());
	}

private:
	Vector2			m_vLookPosition;
	bool			m_bFreeCameraMode; // ����ī�޶� ��� ����

	CGameObject* m_pTarget;
	Vector3 m_vOffset;
};