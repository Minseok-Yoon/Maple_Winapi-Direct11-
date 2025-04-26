#pragma once
#include "../Component/CComponent.h"

using namespace math;

struct tCamEffect
{
	CAM_EFFECT		eEffect;		// 카메라 효과
	float			fDuration;		// 효과 최대 진행 시간
	float			fCurTime;		// 카메라 효과 현재 진행된 시간
};

class CCamera : public CComponent
{
public:
	enum class PROJECTION_TYPE
	{
		PT_Perspective,			// 원근 투영
		PT_Orthographic			// 직교 투영
	};

	void SetProjectionType(const PROJECTION_TYPE _eProjectionType) { m_eProjectionType = _eProjectionType; }
	PROJECTION_TYPE GetProjectionType() const { return m_eProjectionType; }

	const Matrix& GetViewMatrix() { return m_ViewMatrix; }
	const Matrix& GetProjectionMatrix() { return m_ProjectionMatrix; }
	void SetSize(const float _fSize) { m_fSize = _fSize; }

	void SetCameraType(CAMERA_ORDER _eCameraOrder) { m_eCameraOrder = _eCameraOrder; }
	
	wstring FormatVector3(const Vector3& vec);

	Vector3 CaluatePosition(Vector3 _vPos) { return _vPos - m_vDistance; }
	Vector2 CaluatePosition2D(Vector2 _vPos) { return _vPos - m_vDistance; }

	void SetCameraMask(UINT mask) { m_CameraMask = mask; }
	UINT GetCameraMask() const { return m_CameraMask; }

	CCamera();
	virtual ~CCamera();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	void FadeIn(float _fDuration);
	void FadeOut(float _fDuration);

	void CreateViewMatrix();
	void CreateProjectionMatrix(PROJECTION_TYPE _eProjectionType);
	
public:
	class CTransform* m_pTransform;
	float			m_fAspectRatio;
	float			m_fSize;

	vector<CGameObject*>	m_vecRenderTargets;

private:
	//Matrix	ViewMatrix;
	//Matrix	ProjectionMatrix;

	PROJECTION_TYPE m_eProjectionType;
	Matrix			m_ViewMatrix;
	Matrix			m_ProjectionMatrix;
	float			m_fNear;
	float			m_fFar;

	Vector3			m_vDistance;

	list<tCamEffect>	m_listCamEffect;		// 순차적으로 효과를 처리하기 위해 vector말고 list로 사용
	class CTexture* m_pVeilTex;				// 카메라 가림막 택스처(검은색으로)

	int m_iCameraOrder = 0;

	CAMERA_ORDER m_eCameraOrder;
	UINT m_CameraMask = 0xFFFFFFFF; 
};

//void SetLayerMask(UINT _mask) { m_LayerMask = _mask; }
//UINT GetLayerMask() const { return m_LayerMask; }
//
//void SetCameraOrder(int _order) { m_iCameraOrder = _order; }
//int GetCameraOrder() const { return m_iCameraOrder; }
//
//// 특정 레이어가 이 카메라에 의해 렌더링되는지 확인
//bool IsLayerVisible(LAYER_TYPE _type) const
//{
//	return (m_LayerMask & (1 << static_cast<UINT>(_type))) != 0;
//}
//
//
//UINT	m_LayerMask = 0xFFFFFFFF;