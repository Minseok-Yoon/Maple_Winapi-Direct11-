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

	// SpriteRenderer ����
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
		sr = GetOwner()->AddComponent<CSpriteRenderer>();

	m_pAnimator->SetSpriteRenderer(sr);

	// �ִϸ��̼� �̸� ����
	wstring animationName = L"Legato";
	wstring filePathPattern = L"../Resources/Texture/NPC/Lake_of_Oblivion/Legato/stand%d.png";  // %d�� ������ ��ȣ ����
	int frameCount = 8;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	m_pAnimator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		231.0f,                 // �� �������� �ʺ�
		134.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	m_pAnimator->Play(animationName, true);  // �ݺ� ���
	m_strCurAnimName = animationName;
}