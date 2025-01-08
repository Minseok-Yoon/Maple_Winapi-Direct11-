#pragma once
#include "../Component/CComponent.h"

struct tCamEffect
{
	CAM_EFFECT		eEffect;		// 카메라 효과
	float			fDuration;		// 효과 최대 진행 시간
	float			fCurTime;		// 카메라 효과 현재 진행된 시간
};

using namespace math;
class CCamera : public CComponent
{
public:
	enum class PROJECTION_TYPE
	{
		PT_Perspective,
		PT_Orthographic
	};

	static void SetGpuViewMatrix(const Matrix& matrix) { ViewMatrix = matrix; }
	static Matrix GetGpuViewMatrix() { return ViewMatrix; }

	static void SetGpuProjectionMatrix(const Matrix& matrix) { ProjectionMatrix = matrix; }
	static Matrix GetGpuProjectionMatrix() { return ProjectionMatrix; }

	Matrix GetViewMatrix() { return m_ViewMatrix; }
	Matrix GetProjectionMatrix() { return m_ProjectionMatrix; }

	void SetProjectionType(const PROJECTION_TYPE _eProjectionType) { m_eProjectionType = _eProjectionType; }
	PROJECTION_TYPE GetProjectionType() const { return m_eProjectionType; }
	
	void SetSize(const float _fSize) { m_fSize = _fSize; }
	float GetSize() const { return m_fSize; }

	void SetTarget(CGameObject* _pTarget) { m_pTargetObject = _pTarget; }
	CGameObject* GetTarget() { return m_pTargetObject; }

	Vector3 CaluatePosition(Vector3 _vPos) { return _vPos - m_vDistance; }
	Vector2 CaluatePosition2D(Vector2 _vPos) { return _vPos - m_vDistance; }

	CCamera();
	virtual ~CCamera();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void FadeIn(float _fDuration);
	void FadeOut(float _fDuration);

	void CreateViewMatrix();
	void CreateProjectionMatrix(PROJECTION_TYPE _eProjectionType);
	
public:
	float			m_fAspectRatio;
	float			m_fSize;

private:
	CGameObject*	m_pTargetObject;

	static Matrix	ViewMatrix;
	static Matrix	ProjectionMatrix;

	PROJECTION_TYPE m_eProjectionType;
	Matrix			m_ViewMatrix;
	Matrix			m_ProjectionMatrix;
	float			m_fNear;
	float			m_fFar;

	Vector3			m_vDistance;

	list<tCamEffect>	m_listCamEffect;		// 순차적으로 효과를 처리하기 위해 vector말고 list로 사용
	class CTexture* m_pVeilTex;				// 카메라 가림막 택스처(검은색으로)
};