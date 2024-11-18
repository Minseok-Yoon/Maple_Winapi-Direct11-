#include "CCamera.h"
#include "../Object/CGameObject.h"
#include "../Component/CTransform.h"
#include "../Core/CCore.h"

extern CCore core;
math::Matrix CCamera::ViewMatrix = math::Matrix::Identity;
math::Matrix CCamera::ProjectionMatrix = math::Matrix::Identity;

CCamera::CCamera() :
	CComponent(COMPONENT_TYPE::CT_Camera),
	m_eProjectionType(PROJECTION_TYPE::PT_Perspective),
	m_ViewMatrix(math::Matrix::Identity),
	m_ProjectionMatrix(math::Matrix::Identity),
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
	// Core Ŭ�������� ��ġ ���� �޾ƿ´�.
	//m_vResolution = CCore::GetInst()->GetResolution();
	//m_pVeilTex = CTexture::Create(L"CameraVeil", (UINT)m_vResolution.x, (UINT)m_vResolution.y);
}

void CCamera::Update()
{
	//CScene* pCurScene = CSceneManager::GetInst()->GetCurScene();
	//Vector2 vMapSize = pCurScene->GetMapSize();

	//Vector2 vResolution = CCore::GetInst()->GetResolution();

	//if (m_pTargetObj)
	//{
	//	CTransform* tr = m_pTargetObj->GetComponent<CTransform>();
	//	m_vLookPosition = tr->GetPosition();

	//	// ī�޶��� ��ġ�� �� ��� ���� ����
	//	m_vLookPosition.x = std::clamp(m_vLookPosition.x, vResolution.x / 2.0f, vMapSize.x - vResolution.x / 2.0f);
	//	m_vLookPosition.y = std::clamp(m_vLookPosition.y, vResolution.y / 2.0f, vMapSize.y - vResolution.y / 2.0f);

	//	// ī�޶� ��ġ ���� (ȭ�� �߽ɿ��� Ÿ�� ��ġ��)
	//	m_vDistance = m_vLookPosition - (vResolution / 2.0f);

	//	// ī�޶��� Transform�� ������Ʈ�Ͽ� Ÿ���� �ѵ��� �մϴ�.
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
}

void CCamera::LateUpdate()
{
	CreateViewMatrix();
	CreateProjectionMatrix(m_eProjectionType);

	ViewMatrix = m_ViewMatrix;
	ProjectionMatrix = m_ProjectionMatrix;
}

void CCamera::Render()
{
	//if (m_listCamEffect.empty())
	//	return;

	//// �ð� �������� üũ�ؼ�
	//tCamEffect& effect = m_listCamEffect.front();
	//effect.fCurTime += fDeltaTime;

	//float fRatio = 0.f; // ����Ʈ ���� ����
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

	//// ���� �ð��� ����Ʈ �ִ� ���� �ð��� �Ѿ ���
	//if (effect.fDuration <= effect.fCurTime)
	//{
	//	// ȿ�� ����
	//	m_listCamEffect.pop_front();
	//}
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

	m_ViewMatrix = math::Matrix::CreateLookToLH(pos, forward, up);
}

void CCamera::CreateProjectionMatrix(PROJECTION_TYPE _eProjectionType)
{
	RECT winRect;
	GetClientRect(core.GetMainHWnd(), &winRect);
	float width = (winRect.right - winRect.left);
	float height = (winRect.bottom - winRect.top);
	m_fAspectRatio = width / height;

	switch (_eProjectionType)
	{
	case PROJECTION_TYPE::PT_Perspective:
		m_ProjectionMatrix = math::Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, m_fAspectRatio, m_fNear, m_fFar);
		break;

	case PROJECTION_TYPE::PT_Orthographic:
		m_ProjectionMatrix = math::Matrix::CreateOrthographicLH(width / m_fSize, height / m_fSize, m_fNear, m_fFar);
		break;
	}
}
