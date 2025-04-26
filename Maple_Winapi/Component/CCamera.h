#pragma once
#include "../Component/CComponent.h"

using namespace math;

struct tCamEffect
{
	CAM_EFFECT		eEffect;		// ī�޶� ȿ��
	float			fDuration;		// ȿ�� �ִ� ���� �ð�
	float			fCurTime;		// ī�޶� ȿ�� ���� ����� �ð�
};

class CCamera : public CComponent
{
public:
	enum class PROJECTION_TYPE
	{
		PT_Perspective,			// ���� ����
		PT_Orthographic			// ���� ����
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

	list<tCamEffect>	m_listCamEffect;		// ���������� ȿ���� ó���ϱ� ���� vector���� list�� ���
	class CTexture* m_pVeilTex;				// ī�޶� ������ �ý�ó(����������)

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
//// Ư�� ���̾ �� ī�޶� ���� �������Ǵ��� Ȯ��
//bool IsLayerVisible(LAYER_TYPE _type) const
//{
//	return (m_LayerMask & (1 << static_cast<UINT>(_type))) != 0;
//}
//
//
//UINT	m_LayerMask = 0xFFFFFFFF;