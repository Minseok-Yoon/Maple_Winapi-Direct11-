#include "CCamera.h"
#include "../Object/CGameObject.h"
#include "../Component/CTransform.h"
#include "../Core/CCore.h"

extern CCore core;
Matrix CCamera::ViewMatrix = Matrix::Identity;
Matrix CCamera::ProjectionMatrix =Matrix::Identity;

RECT CCamera::GetClipRect()
{
	RECT clipRect = {};
	if (m_useWorldRect)
	{
		clipRect = m_worldRect;
	}
	else
	{
		const float halfWidth = m_fSize * m_fAspectRatio / 2.0f;
		const float halfHeight = m_fSize / 2.0f;

		clipRect.left = static_cast<LONG>(m_center.x - halfWidth);
		clipRect.right = static_cast<LONG>(m_center.x + halfWidth);
		clipRect.top = static_cast<LONG>(m_center.y - halfHeight);
		clipRect.bottom = static_cast<LONG>(m_center.y + halfHeight);
	}
	return clipRect;
}

CCamera::CCamera() :
	CComponent(COMPONENT_TYPE::CT_Camera),
	m_eProjectionType(PROJECTION_TYPE::PT_Perspective),
	m_ViewMatrix(Matrix::Identity),
	m_ProjectionMatrix(Matrix::Identity),
	m_fAspectRatio(0.0f),
	m_fNear(1.0f),
	m_fFar(1000.0f),
	m_fSize(1.0f),
	m_pVeilTex(nullptr),
	m_listCamEffect{}
{
}

CCamera::~CCamera()
{
}

void CCamera::Init()
{
}

void CCamera::Update()
{
#pragma region
	//CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	//Vector2 vMapSize = pCurScene->GetMapSize();

	//Vector2 vResolution = CCore::GetInst()->GetResolution();

	//if (m_pTargetObj)
	//{
	//	CTransform* tr = m_pTargetObj->GetComponent<CTransform>();
	//	m_vLookPosition = tr->GetPosition();

	//	// 카메라의 위치를 맵 경계 내로 제한
	//	m_vLookPosition.x = std::clamp(m_vLookPosition.x, vResolution.x / 2.0f, vMapSize.x - vResolution.x / 2.0f);
	//	m_vLookPosition.y = std::clamp(m_vLookPosition.y, vResolution.y / 2.0f, vMapSize.y - vResolution.y / 2.0f);

	//	// 카메라 위치 조정 (화면 중심에서 타겟 위치로)
	//	m_vDistance = m_vLookPosition - (vResolution / 2.0f);

	//	// 카메라의 Transform을 업데이트하여 타겟을 쫓도록 합니다.
	//	CTransform* cameraTransform = GetOwner()->GetComponent<CTransform>();
	//	cameraTransform->SetPosition(m_vLookPosition - m_vDistance);
	//}
	//else if (m_pGameObjectOwner)
	//{
	//	CTransform* cameraTr = m_pGameObjectOwner->GetComponent<CTransform>();
	//	if (cameraTr)
	//	{
	//		m_vLookPosition = cameraTr->GetPosition();
	//	}
	//}

	//if (m_vLookPosition.x < 0)
	//	m_vLookPosition.x = 0;
	//else if (m_vLookPosition.x > vMapSize.x - vResolution.x)
	//	m_vLookPosition.x = vMapSize.x - vResolution.x;

	//if (m_vLookPosition.y < 0)
	//	m_vLookPosition.y = 0;
	//else if (m_vLookPosition.y > vMapSize.y - vResolution.y)
	//	m_vLookPosition.y = (vMapSize.y - vResolution.y) - 10.f;
#pragma endregion
}

void CCamera::LateUpdate()
{
	CreateViewMatrix();
	CreateProjectionMatrix(m_eProjectionType);

	AdjustToWorldRect();

	ViewMatrix = m_ViewMatrix;
	ProjectionMatrix = m_ProjectionMatrix;
}

void CCamera::Render()
{
#pragma region 카메라 랜더
	//if (m_listCamEffect.empty())
	//	return;

	//// 시간 누적값을 체크해서
	//tCamEffect& effect = m_listCamEffect.front();
	//effect.fCurTime += fDeltaTime;

	//float fRatio = 0.f; // 이펙트 진행 비율
	//fRatio = effect.fCurTime / effect.fDuration;

	//if (fRatio < 0.f)
	//	fRatio = 0.f;
	//if (fRatio > 1.f)
	//	fRatio = 1.f;

	//int iAlpha = 0;

	//if (CAM_EFFECT::CE_Fade_Out == effect.eEffect)
	//{
	//	iAlpha = (int)(255.f * fRatio);
	//}

	//else if (CAM_EFFECT::CE_Fade_In == effect.eEffect)
	//{
	//	iAlpha = (int)(255.f * (1.f - fRatio));
	//}

	//BLENDFUNCTION bf = {};

	//bf.BlendOp = AC_SRC_OVER;
	//bf.BlendFlags = 0;
	//bf.AlphaFormat = 0;
	//bf.SourceConstantAlpha = iAlpha;

	//AlphaBlend(_Hdc, 0, 0,
	//	(int)m_pVeilTex->GetWidth(),
	//	(int)m_pVeilTex->GetHeight(),
	//	m_pVeilTex->GetHdc(),
	//	0, 0,
	//	(int)m_pVeilTex->GetWidth(),
	//	(int)m_pVeilTex->GetHeight(), bf);

	//// 진행 시간이 이펙트 최대 지정 시간을 넘어선 경우
	//if (effect.fDuration <= effect.fCurTime)
	//{
	//	// 효과 종료
	//	m_listCamEffect.pop_front();
	//}
#pragma endregion
}

void CCamera::FadeIn(float _fDuration)
{
	tCamEffect ef = {};
	ef.eEffect = CAM_EFFECT::CE_Fade_In;
	ef.fDuration = _fDuration;
	ef.fCurTime = 0.f;

	m_listCamEffect.push_back(ef);

	if (0.f == _fDuration)
		assert(nullptr);
}

void CCamera::FadeOut(float _fDuration)
{
	tCamEffect ef = {};
	ef.eEffect = CAM_EFFECT::CE_Fade_Out;
	ef.fDuration = _fDuration;
	ef.fCurTime = 0.f;

	m_listCamEffect.push_back(ef);

	if (0.f == _fDuration)
		assert(nullptr);
}

void CCamera::CreateViewMatrix()
{
	CTransform* tr = GetOwner()->GetComponent<CTransform>();

	const Vector3 pos = tr->GetPosition();
	const Vector3 up = tr->Up();
	const Vector3 forward = tr->Forward();

	m_ViewMatrix = Matrix::CreateLookToLH(pos, forward, up);
}

void CCamera::CreateProjectionMatrix(PROJECTION_TYPE _eProjectionType)
{
	RECT winRect = {};
	GetClientRect(core.GetMainHWnd(), &winRect);
	const float width = static_cast<float>(winRect.right - winRect.left);
	const float height = static_cast<float>(winRect.bottom - winRect.top);
	m_fAspectRatio = width / height;

	switch (_eProjectionType)
	{
	case PROJECTION_TYPE::PT_Perspective:
		m_ProjectionMatrix = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, m_fAspectRatio, m_fNear, m_fFar);
		break;

	case PROJECTION_TYPE::PT_Orthographic:
		m_ProjectionMatrix = Matrix::CreateOrthographicLH(width / m_fSize, height / m_fSize, m_fNear, m_fFar);
		break;
	}
}

void CCamera::AdjustToWorldRect()
{
	if (!m_useWorldRect)
		return;

	const RECT clipRect = GetClipRect();

	// X축 이동 제한
	if (clipRect.right - clipRect.left > m_worldRect.right - m_worldRect.left)
	{
		m_center.x = (m_worldRect.left + m_worldRect.right) / 2.0f;
	}
	else
	{
		m_center.x = std::clamp(m_center.x,
			m_worldRect.left + (clipRect.right - clipRect.left) / 2.0f,
			m_worldRect.right - (clipRect.right - clipRect.left) / 2.0f);
	}

	// Y축 이동 제한
	if (clipRect.bottom - clipRect.top > m_worldRect.bottom - m_worldRect.top)
	{
		m_center.y = (m_worldRect.top + m_worldRect.bottom) / 2.0f;
	}
	else
	{
		m_center.y = std::clamp(m_center.y,
			m_worldRect.top + (clipRect.bottom - clipRect.top) / 2.0f,
			m_worldRect.bottom - (clipRect.bottom - clipRect.top) / 2.0f);
	}
}

void CCamera::ChangeDisplayMode(int _iMode)
{
	m_displayMode = _iMode;

	switch (m_displayMode)
	{
	case 0: // 800x600
		core.SetWidth(800);
		core.SetHeight(600);
		break;
	case 1: // 1024x768
		core.SetWidth(1024);
		core.SetHeight(768);
		break;
	case 2: // 1268x760
		core.SetWidth(1268);
		core.SetHeight(768);
		break;
	case 3: // 1366x768
		core.SetWidth(1366);
		core.SetHeight(768);
		break;
	case 4: // Fullscreen
		core.SetWidth(1600);
		core.SetHeight(900);
		break;
	default:
		ChangeDisplayMode(0); // Default to 800x600
		break;
	}
}

RECT CCamera::MeasureDrawRect(int _iSpriteWidth, int _iSpriteHeight, const XMFLOAT2& position, const XMFLOAT2& origin, bool _bflipX)
{
	RECT drawingRect;
	drawingRect.left = static_cast<LONG>(position.x - origin.x);
	drawingRect.top = static_cast<LONG>(position.y - origin.y);
	drawingRect.right = drawingRect.left + _iSpriteWidth;
	drawingRect.bottom = drawingRect.top + _iSpriteHeight;

	if (_bflipX) {
		drawingRect.left -= _iSpriteWidth;
		drawingRect.right -= _iSpriteWidth;
	}

	return drawingRect;
}

Vector2 CCamera::CameraToWorld(const Vector2& _vCameraPoint)
{
	Vector2 worldPoint = _vCameraPoint;
	worldPoint.x += m_center.x - (m_fSize * m_fAspectRatio) / 2.0f;
	worldPoint.y += m_center.y - (m_fSize / 2.0f);
	return worldPoint;
}

bool CCamera::CheckSpriteVisible(const RECT& _tSpriteRect)
{
	RECT clip = GetClipRect();
	return !(_tSpriteRect.right < clip.left || _tSpriteRect.left > clip.right ||
		_tSpriteRect.bottom < clip.top || _tSpriteRect.top > clip.bottom);
}
