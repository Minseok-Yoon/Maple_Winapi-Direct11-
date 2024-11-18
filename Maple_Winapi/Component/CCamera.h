#pragma once
#include "../Component/CComponent.h"

struct tCamEffect
{
	CAM_EFFECT		eEffect;		// ī�޶� ȿ��
	float			fDuration;		// ȿ�� �ִ� ���� �ð�
	float			fCurTime;		// ī�޶� ȿ�� ���� ����� �ð�
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

	list<tCamEffect>	m_listCamEffect;		// ���������� ȿ���� ó���ϱ� ���� vector���� list�� ���
	class CTexture* m_pVeilTex;				// ī�޶� ������ �ý�ó(����������)
};