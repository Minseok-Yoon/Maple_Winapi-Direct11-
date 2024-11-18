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

	static void SetGpuViewMatrix(math::Matrix matrix) { ViewMatrix = matrix; }
	static math::Matrix GetGpuViewMatrix() { return ViewMatrix; }

	static void SetGpuProjectionMatrix(math::Matrix matrix) { ProjectionMatrix = matrix; }
	static math::Matrix GetGpuProjectionMatrix() { return ProjectionMatrix; }

	void SetProjectionType(PROJECTION_TYPE _eProjectionType) { m_eProjectionType = _eProjectionType; }
	void SetSize(float _fSize) { m_fSize = _fSize; }

	CCamera();
	~CCamera();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	void FadeIn(float _fDuration);
	void FadeOut(float _fDuration);

	void CreateViewMatrix();
	void CreateProjectionMatrix(PROJECTION_TYPE _eProjectionType);

private:
	static math::Matrix ViewMatrix;
	static math::Matrix	ProjectionMatrix;

	PROJECTION_TYPE m_eProjectionType;
	math::Matrix	m_ViewMatrix;
	math::Matrix	m_ProjectionMatrix;
	float			m_fAspectRatio;
	float			m_fNear;
	float			m_fFar;
	float			m_fSize;

	list<tCamEffect>	m_listCamEffect;		// 순차적으로 효과를 처리하기 위해 vector말고 list로 사용
	class CTexture* m_pVeilTex;				// 카메라 가림막 택스처(검은색으로)
};