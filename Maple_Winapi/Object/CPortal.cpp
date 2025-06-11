#include "../pch.h"
#include "CPortal.h"
#include "CObject.h"
#include "../Component/CCollider.h"
#include "../Component/CAnimator.h"
#include "../Component/CAnimation.h"
#include "../Component/CTransform.h"
#include "../Component/CPortalScript.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CSceneManager.h"

CPortal::CPortal()
{
}

CPortal::~CPortal()
{
}

void CPortal::Init()
{
	CGameObject::Init();

	m_pTransform = this->AddComponent<CTransform>();
	m_pTransform->SetLocalScale(Vector3(88.0f, 217.0f, 0.0f));

	CAnimator* animator = this->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = this->AddComponent<CAnimator>();
	}

	m_pSpriteRenderer = this->GetComponent<CSpriteRenderer>();
	if (m_pSpriteRenderer == nullptr)
	{
		m_pSpriteRenderer = this->AddComponent<CSpriteRenderer>();
	}
	animator->SetSpriteRenderer(m_pSpriteRenderer);

	// Idle 애니메이션 생성
	wstring animationName = L"Portal";
	wstring filePathPattern = L"../Resources/Texture/Portal/Portal_%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 8;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		88.0f,                 // 각 프레임의 너비
		217.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생

    if (m_pPortalCollision == nullptr)
    {
        m_pPortalCollision = AddComponent<CCollider>();
        m_pPortalCollision->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
        m_pPortalCollision->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));

        if (!m_pPortalCollision)
        {
            OutputDebugStringA("ERROR: Collider creation failed in CPortal::Init()\n");
        }
        else
        {
            OutputDebugStringA("INFO: Collider successfully added to Portal\n");
        }
    }
	this->AddComponent<CPortalScript>();
}

void CPortal::Update()
{
	CGameObject::Update();
}

void CPortal::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CPortal::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

void CPortal::OnPlayerEnter()
{
	m_pPortalCollision->IsCollider(true);
}

void CPortal::OnPlayerStay()
{
	m_pPortalCollision->IsCollider(true);
}

void CPortal::OnPlayerExit()
{
	m_pPortalCollision->IsCollider(false);
}

//int CPortal::m_iPortalCount = 0;
//
//CPortal::CPortal(const string& portalTag) :
//	m_strPortalTag(portalTag)
//{
//}
//
//CPortal::~CPortal()
//{
//}
//
//void CPortal::StagePortals()
//{
//	m_stagePortals["Main"] = {
//		{{150.f, 820.f}, "Stage1"}
//	};
//
//	m_stagePortals["Stage1"] = {
//		{{1280.f, 485.f}, "Main"}
//	};
//
//	m_portalToStageMap["Portal 0"] = "Stage1";
//	m_portalToStageMap["Portal 1"] = "Main";
//}
//
//void CPortal::PortalAnimationClip()
//{
//	/*CreateAnimator();
//
//	AddAnimationClip(L"Stand", L"texture\\Portal\\%d.bmp", 8, 1.f, 88.f, 217.f, 0.f, 0.f);*/
//}
//
//void CPortal::AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY, float _fOffsetX, float _fOffsetY)
//{
//	/*vector<wstring> vecFile;
//
//	for (int i = 1; i <= iFrameMax; i++)
//	{
//		wchar_t strFileName[MAX_PATH] = {};
//		wsprintf(strFileName, pFilePath, i);
//		vecFile.push_back(strFileName);
//	}
//
//	GetAnimator()->CreateFrameAnimation(strKey, vecFile, Vector2(0.f, 0.f), Vector2(fFrameSizeX, fFrameSizeY), fAnimationLimitTime, Vector2(_fOffsetX, _fOffsetY));*/
//}
//
//void CPortal::Update()
//{
//}
//
//void CPortal::Render()
//{
//	/*CObject::ComponentRender(_dc);
//
//	Vector2 vPos = GetPos();
//	vPos = CCamera::GetInst()->GetRenderPos(vPos);*/
//}

// 텍스처를 제대로 출력이 된다고 하는데 왜 플레이어 텍스처랑 배경 택스처가 나오는지?
// fps이 겁나게 떨어짐 아마 출력 텍스처 때문인듯?(텍스처 지웠음에도 불구하고 FPS 10정도 떨어짐)