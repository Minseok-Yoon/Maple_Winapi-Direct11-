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

	void SetOrthographic(float left, float right, float top, float bottom)
	{
		m_left = left;
		m_right = right;
		m_top = top;
		m_bottom = bottom;
	}

	// 정사영 카메라의 시야 범위 반환
	float GetLeft() const { return m_left; }
	float GetRight() const { return m_right; }
	float GetTop() const { return m_top; }
	float GetBottom() const { return m_bottom; }

	// 클립 영역 가져오기
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

	int m_width;  // 화면 너비
	int m_height; // 화면 높이
	float m_left, m_right, m_top, m_bottom; // 정사영 카메라의 시야 범위

	XMFLOAT2		m_center;				 // 카메라 중심 좌표
	int				m_displayMode;           // 디스플레이 모드
	bool			m_useWorldRect;			 // 월드 경계 사용 여부
	RECT			m_worldRect;             // 월드 경계
	RECT			m_clipRect;              // 현재 클립 영역

	list<tCamEffect>	m_listCamEffect;		// 순차적으로 효과를 처리하기 위해 vector말고 list로 사용
	class CTexture* m_pVeilTex;				// 카메라 가림막 택스처(검은색으로)
};