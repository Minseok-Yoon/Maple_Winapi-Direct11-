#include "../pch.h"
#include "CNpcScript.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"
#include "CBackGround.h"
#include "../Component/CAnimator.h"

CNpcScript::CNpcScript() :
	CScript(SCRIPT_TYPE::ST_NpcScript),
	m_pAnimator(nullptr)
{
}

CNpcScript::~CNpcScript()
{
}

void CNpcScript::OnInit()
{
	m_pTransform = GetOwner()->GetTransform();

	CScene* pScene = CSceneManager::GetCurScene();
	m_pBackGround = pScene->GetBackGround();
}

void CNpcScript::OnUpdate()
{
	if (!m_pAnimator)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	if (!m_pAnimator)
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();

	if (!m_pNpcOwner) return;

	if (m_pNpcOwner->GetNpcCurState() == NPC_STATE::NS_Idle)
	{
		idle();
	}
}

void CNpcScript::OnLateUpdate()
{
}

void CNpcScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CNpcScript::OnCollisionEnter(CCollider* other)
{
}

void CNpcScript::OnCollisionStay(CCollider* other)
{
}

void CNpcScript::OnCollisionExit(CCollider* other)
{
}

void CNpcScript::idle()
{
	if (m_pAnimator == nullptr)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();

	// SpriteRenderer 설정
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
		sr = GetOwner()->AddComponent<CSpriteRenderer>();

	m_pAnimator->SetSpriteRenderer(sr);

	// 애니메이션 이름 생성
	wstring animationName = L"Legato";
	wstring filePathPattern = L"../Resources/Texture/NPC/Lake_of_Oblivion/Legato/stand%d.png";  // %d로 프레임 번호 변경
	int frameCount = 8;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	m_pAnimator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		231.0f,                 // 각 프레임의 너비
		134.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	m_pAnimator->Play(animationName, true);  // 반복 재생
	m_strCurAnimName = animationName;
}