#include "CCamera.h"
#include "../Object/CGameObject.h"
#include "../Component/CTransform.h"
#include "../Core/CCore.h"
#include "../Manager/CSceneManager.h"
#include "../Scene/CScene.h"
#include <imgui.h>
#include "CRenderer.h"

extern CCore core;

wstring CCamera::FormatVector3(const Vector3& vec)
{
	wstringstream ss;
	ss << L"(" << vec.x << L", " << vec.y << L", " << vec.z << L")";
	return ss.str();
}

CCamera::CCamera() :
	CComponent(COMPONENT_TYPE::CT_Camera),
	m_eProjectionType(PROJECTION_TYPE::PT_Perspective),	// 투영 타입
	m_vDistance(Vector3(0.0f, 0.0f, 0.0f)),
	m_ViewMatrix(Matrix::Identity),
	m_ProjectionMatrix(Matrix::Identity),
	m_fAspectRatio(0.0f),
	m_fNear(1.0f),
	m_fFar(1000.0f),
	m_fSize(1.0f), // Size가 올라갈수록 확대되어 보여진다.
	m_pVeilTex(nullptr),
	m_listCamEffect{}
{
	m_pTransform = new CTransform();
}

CCamera::~CCamera()
{
	if (m_pTransform)
	{
		delete m_pTransform;
		m_pTransform = nullptr;
	}
}

void CCamera::Init()
{
	CComponent::Init();

	CScene* curscene = CSceneManager::GetCurScene();
	curscene->AddCamera(this);
}

void CCamera::Update()
{
}

void CCamera::LateUpdate()
{
	CreateViewMatrix();
	CreateProjectionMatrix(m_eProjectionType);
}

void CCamera::Render(const Matrix& view, const Matrix& projection)
{
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

	const Vector3 pos = tr->GetWorldPosition();
	const Vector3 up = tr->Up();
	const Vector3 forward = tr->Forward();

	m_ViewMatrix = Matrix::CreateLookToLH(pos, forward, up);

	// 디버깅: ViewMatrix 확인
	assert(m_ViewMatrix != Matrix::Identity);
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

	// 프로젝션 매트릭스 생성 로직
	assert(m_ProjectionMatrix != Matrix::Identity);
}

// 2025-04-17
//if (m_pTargetObject != nullptr)
	//{
	//	Vector3 targetPos = m_pTargetObject->m_pTransform->GetWorldPosition();
	//	Vector3 camPos = targetPos + m_vDistance;

	//	m_pTransform->SetLocalPosition(camPos);

	//	Vector3 local = m_pTargetObject->GetTransform()->GetLocalPosition();
	//	Vector3 world = m_pTargetObject->GetTransform()->GetWorldPosition();

	//	std::wstring debugMsg = L"[Camera] 타겟 LocalPosition: " + FormatVector3(local) + L"\n";
	//	OutputDebugStringW(debugMsg.c_str());

	//	debugMsg = L"[Camera] 타겟 WorldPosition: " + FormatVector3(world) + L"\n";
	//	OutputDebugStringW(debugMsg.c_str());

	//	// 디버깅 로그
	//	std::wstring targetStr = FormatVector3(targetPos);
	//	std::wstring camStr = FormatVector3(camPos);

	//	std::wstring debugMsgs = L"[Camera] 타겟 위치: " + targetStr + L"\n";
	//	OutputDebugStringW(debugMsgs.c_str());

	//	debugMsg = L"[Camera] 실제 카메라 위치로 이동: " + camStr + L"\n";
	//	OutputDebugStringW(debugMsg.c_str());
	//}

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

//Matrix CCamera::ViewMatrix = Matrix::Identity;			// 뷰 매트릭스
//Matrix CCamera::ProjectionMatrix =Matrix::Identity;		// 투영 매트릭스

//void CCamera::LateUpdate()
//{
//	//CreateViewMatrix();
//	//CreateProjectionMatrix(m_eProjectionType);
//
//	//renderer::activeCamera = this; //  여기서 바꿔줌
//
//	//SetGpuViewMatrix(m_ViewMatrix);
//	//SetGpuProjectionMatrix(m_ProjectionMatrix);
//
//	////OutputDebugString((L"[Camera] ViewMatrix: " + FormatMatrix(ViewMatrix) + L"\n").c_str());
//	////OutputDebugString((L"[Camera] ProjectionMatrix: " + FormatMatrix(ProjectionMatrix) + L"\n").c_str());
//
//	CreateViewMatrix();
//	CreateProjectionMatrix(m_eProjectionType);
//	//renderer::activeCamera = this; //  여기서 바꿔줌
//	//SetGpuViewMatrix(m_ViewMatrix);
//	//SetGpuProjectionMatrix(m_ProjectionMatrix);
//
//	//// GPU에 설정된 행렬 확인
//	//OutputDebugStringW(L"View & Projection Matrix 업데이트 완료\n");
//}