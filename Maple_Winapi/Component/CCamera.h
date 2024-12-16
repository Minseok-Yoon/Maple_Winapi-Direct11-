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

	static void SetGpuViewMatrix(const Matrix& matrix) { ViewMatrix = matrix; }
	static Matrix GetGpuViewMatrix() { return ViewMatrix; }

	static void SetGpuProjectionMatrix(const Matrix& matrix) { ProjectionMatrix = matrix; }
	static Matrix GetGpuProjectionMatrix() { return ProjectionMatrix; }

	Matrix GetViewMatrix() { return m_ViewMatrix; }
	Matrix GetProjectionMatrix() { return m_ProjectionMatrix; }

	void SetProjectionType(const PROJECTION_TYPE _eProjectionType) { m_eProjectionType = _eProjectionType; }
	void SetSize(const float _fSize) { m_fSize = _fSize; }

	RECT GetClipRect();

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

	void AdjustToWorldRect();
	void ChangeDisplayMode(int _iMode);

	RECT MeasureDrawRect(int _iSpriteWidth, int _iSpriteHeight, const XMFLOAT2& position, const XMFLOAT2& origin, bool _bflipX);
	Vector2 CameraToWorld(const Vector2& _vCameraPoint);
	bool CheckSpriteVisible(const RECT& _tSpriteRect);

private:
	static Matrix	ViewMatrix;
	static Matrix	ProjectionMatrix;

	PROJECTION_TYPE m_eProjectionType;
	Matrix			m_ViewMatrix;
	Matrix			m_ProjectionMatrix;
	float			m_fAspectRatio;
	float			m_fNear;
	float			m_fFar;
	float			m_fSize;

	int m_width;  // ȭ�� �ʺ�
	int m_height; // ȭ�� ����

	XMFLOAT2		m_center;				 // ī�޶� �߽� ��ǥ
	int				m_displayMode;           // ���÷��� ���
	bool			m_useWorldRect;			 // ���� ��� ��� ����
	RECT			m_worldRect;             // ���� ���
	RECT			m_clipRect;              // ���� Ŭ�� ����

	list<tCamEffect>	m_listCamEffect;		// ���������� ȿ���� ó���ϱ� ���� vector���� list�� ���
	class CTexture* m_pVeilTex;				// ī�޶� ������ �ý�ó(����������)
};