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

	default:
		OutputDebugStringW(L"⚠️ 투영 타입이 잘못됨!\n");
		break;
	}

	// 프로젝션 매트릭스 생성 로직
	assert(m_ProjectionMatrix != Matrix::Identity);
}

Vector3 CCamera::ScreenToWorld(const Vector2& screenPos)
{
	RECT rc = {};
	GetClientRect(core.GetMainHWnd(), &rc);

	float width = static_cast<float>(rc.right - rc.left);
	float height = static_cast<float>(rc.bottom - rc.top);

	// 1. 클라이언트 좌표 → NDC 좌표 (-1 ~ 1)
	float ndcX = (2.0f * screenPos.x / width) - 1.0f;
	float ndcY = (2.0f * screenPos.y / height) - 1.0f; // Y축 뒤집기 (상단 기준이기 때문)

	// 2. NDC → View 공간 (Orthographic의 경우 NDC 그대로 사용 가능)
	Vector4 ndcPos = Vector4(ndcX, ndcY, 0.0f, 1.0f);

	// 3. View 공간 → World 공간 (View * Projection 역행렬 적용)
	Matrix viewProj = m_ViewMatrix * m_ProjectionMatrix;
	Matrix invViewProj = viewProj.Invert();

	Vector4 worldPos = Vector4::Transform(ndcPos, invViewProj);

	if (fabs(worldPos.w) > 1e-6f)
	{
		worldPos.x /= worldPos.w;
		worldPos.y /= worldPos.w;
		worldPos.z /= worldPos.w;
	}
	else
	{
		// w가 거의 0이면 로그/디버그
		OutputDebugStringA("Warning: worldPos.w is zero or near-zero in ScreenToWorld\n");
	}

	return Vector3(worldPos.x, worldPos.y, worldPos.z);

	return Vector3(worldPos.x, worldPos.y, 0.0f);
}

bool CCamera::IsWorldPosVisible(const Vector3& worldPos, const Matrix& view, const Matrix& proj)
{
	Matrix viewProj = view * proj;
	Vector4 clip = Vector4::Transform(Vector4(worldPos.x, worldPos.y, worldPos.z, 1.0f), viewProj);

	// w 체크
	if (fabs(clip.w) < 1e-6f) {
		OutputDebugStringA("IsVisible: w nearly zero\n");
		return false;
	}

	// 동차 나누기 -> NDC
	Vector3 ndc(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);

	// DirectX (D3D) 기본 z 범위는 0..1, X/Y는 -1..1
	//char buf[256];
	//sprintf_s(buf, "NDC = (%f, %f, %f)\n", ndc.x, ndc.y, ndc.z);
	//OutputDebugStringA(buf);

	bool inX = (ndc.x >= -1.0f && ndc.x <= 1.0f);
	bool inY = (ndc.y >= -1.0f && ndc.y <= 1.0f);
	bool inZ = (ndc.z >= 0.0f && ndc.z <= 1.0f); // D3D: 0..1

	return inX && inY && inZ;
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

// 2025-07-01
// 여기서 드는 의문 카메라의 near와 far clip이 정확히 의미하는 의도는 뭘까?
// -> nearClip은 보이기 시작하는 지점, farClip은 가장 멀리 보이는 지점
// 또한 near = 1.0f이고 far = 1000.0f일 때 z = 0.0f 또는 z = -0.1f 이런 값들은 보이지 않는 이유는 무엇이며
// 어째서 z = -1.0f의 값은 보이는 것인가?
// -> 예시로 카메라 위치가 z = -10.0f, near = 1.0f, far = 1000.0f일 때
// 렌더링 가능한 z 범위(월드 기준)
// z = -10.0f + 1.0f = -9.0f (near plane 시작점)
// z = -10.0f + 1000.0f = 990.0f (far plane 끝점)
// 즉 -9.0f ~ 990.0f 사이에는 보인다는 점
// 카메라의 좌표계와 클리핑 규칙을 명확히 이해할 것.