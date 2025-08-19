#include "../pch.h"
#include "CBaseSkillScript.h"
#include "../Scene/CScene.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CTimeManager.h"
#include "../Object/CBaseSkill.h"
#include "../Component/CAnimator.h"
#include "../Component/CSpriteRenderer.h"
#include "../Manager/CKeyManager.h"

CBaseSkillScript::CBaseSkillScript() :
	CScript(SCRIPT_TYPE::ST_SkillScript)
{
}

CBaseSkillScript::~CBaseSkillScript()
{
}

void CBaseSkillScript::OnInit()
{
	m_pTransform = GetOwner()->GetTransform();

	if (m_pTransform == nullptr)
	{
		OutputDebugStringA("ERROR: CMonsterScript - Transform is null!\n");
	}

	CScene* pScene = CSceneManager::GetCurScene();
	if (!pScene)
	{
		OutputDebugStringA("ERROR: CMonsterScript - Current scene is null!\n");
		return;
	}

	m_pBackGround = pScene->GetBackGround();
	if (!m_pBackGround)
	{
		OutputDebugStringA("ERROR: CMonsterScript - BackGround is null!\n");
	}
}

void CBaseSkillScript::OnUpdate()
{
	if (!m_pAnimator)
		m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	if (!m_pAnimator)
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();

	if (!m_pSkillOwner) return;

	// NŰ �Է� ó��
	if (KEY_TAP(KEY_CODE::N))
	{
		if (m_pAnimator->IsPlaying())
		{
			m_bRestartRequested = false; // ��� ���̸� ����
		}
		else
		{
			attack(Skill_State::SS_Casting); // �ٷ� ����
		}
	}

	switch (m_pSkillOwner->GetSkillState())
	{
	case Skill_State::SS_Idle: break;
	case Skill_State::SS_Casting: break; // OnUpdate������ �ٷ� Play �� ��
	case Skill_State::SS_Cooldown: break;
	case Skill_State::SS_End: Dead(); break;
	}

	// ���� Ÿ�̹� ���� �� ������ ó��
	if (!m_bHasDealtDamage && m_pAnimator && m_pAnimator->IsPlaying()) // ��: 2��° �����ӿ��� ���� ����
	{
		int frameIdx = m_pAnimator->GetCurrentFrameIndex();
		const int attackfrm = 3;
		if (frameIdx == attackfrm)
		{
			if (m_pColliderMonster)
			{
				CMonster* pMonster = dynamic_cast<CMonster*>(m_pColliderMonster);
				if (pMonster)
				{
					// �� �κ� PlayerScript�� ���� �߻� playerScri�� nullptr
					CBaseSkillScript* playerScri = GetOwner()->GetComponent<CBaseSkillScript>();
					if (playerScri)
					{
						playerScri->PlayerSkillAttack(pMonster);  // ���� ������ �ִ� �Լ� ȣ��
						m_bHasDealtDamage = true;
					}
				}
			}
		}
	}
}

void CBaseSkillScript::OnLateUpdate()
{
}

void CBaseSkillScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CBaseSkillScript::OnCollisionEnter(CCollider* other)
{
}

void CBaseSkillScript::OnCollisionStay(CCollider* other)
{
	if (other && dynamic_cast<CMonster*>(other->GetOwner()))
	{
		m_pColliderMonster = other->GetOwner();
	}
}

void CBaseSkillScript::OnCollisionExit(CCollider* other)
{
}

void CBaseSkillScript::attack(Skill_State state)
{
	if (!m_pAnimator) return;

	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (!sr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
		sr->SetActive(true);
	}

	m_pAnimator->SetSpriteRenderer(sr);
	OutputDebugStringW((L"[Skill::attack] Owner=" + GetOwner()->GetName() + L" sr=" + std::to_wstring((uintptr_t)sr) + L"\n").c_str());

	auto it = m_AnimMap.find(state);
	if (it == m_AnimMap.end()) return;

	const SkillAnimInfo& animData = it->second;
	std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	std::wstring animName = std::to_wstring(static_cast<int>(state)) + L"_" + direction;

	if (!m_pAnimator->HasAnimation(animName))
	{
		std::wstring path = animData.pathPattern;
		size_t pos = path.find(L"%s");
		if (pos != std::wstring::npos) path.replace(pos, 2, direction);

		m_pAnimator->AddFrameAnimation(
			animName, path.c_str(), animData.frameCount,
			Vector2(0, 0), animData.frameSize.x, animData.frameSize.y,
			0.f, 0.f, animData.frameDuration
		);
	}

	// �׻� Reset + Sprite ���� �� ���
	m_pAnimator->ResetAnimation();
	sr->SetTexture(nullptr);
	m_strCurAnimName = animName;

	// �ִϸ��̼� �Ϸ� �ݹ�
	auto& completeEvent = m_pAnimator->GetCompleteEvent(animName);
	completeEvent = [this, sr]()
	{
		EndSkillAnimation();
	};

	// ���� ����
	if (m_pSkillOwner)
		m_pSkillOwner->SetSkillState(Skill_State::SS_Casting);

	m_pAnimator->Play(animName, false);
}

void CBaseSkillScript::Dead()
{
	std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	std::wstring animName = std::to_wstring(static_cast<int>(Skill_State::SS_End)) + L"_" + direction;

	if (!m_pAnimator || !m_pAnimator->HasAnimation(animName))
		return;

	function<void()>& completeEvent = m_pAnimator->GetCompleteEvent(animName);
	if (!completeEvent)  //  �ߺ� ��� ����
	{
		completeEvent = [this]() {
			GetOwner()->SetDead();
		};
	}
}

void CBaseSkillScript::OnAnimationCycleComplete()
{
	if (m_bRestartRequested)
	{
		attack(Skill_State::SS_Casting); // ��� ���� ����
		m_bRestartRequested = false;
	}
}

void CBaseSkillScript::RestartCurrentAnimation()
{
	if (m_pAnimator && !m_strCurAnimName.empty())
	{
		// ���� �ִϸ��̼��� ó������ �ٽ� ���
		m_pAnimator->ResetAnimation();
		m_pAnimator->Play(m_strCurAnimName, false);

		// �ʿ��ϴٸ� ��ų ���µ� ����
		if (m_pSkillOwner)
		{
			m_pSkillOwner->SetSkillState(Skill_State::SS_Casting);
		}
	}
}

void CBaseSkillScript::EndSkillAnimation()
{
	if (m_pAnimator)
	{
		// �����ϰ� �ִϸ����� ���� ���� �� ���� ����
		m_pAnimator->Stop();
		m_pAnimator->SetSpriteRenderer(nullptr);
	}

	if (auto* sr = GetOwner()->GetComponent<CSpriteRenderer>())
	{
		sr->SetTexture(nullptr);
		sr->SetActive(false);
	}

	if (m_pSkillOwner)
		m_pSkillOwner->SetSkillState(Skill_State::SS_Idle);

	m_strCurAnimName.clear();
	m_bRestartRequested = false;
	m_bHasDealtDamage = false;
}

void CBaseSkillScript::FinishAndHide()
{
	m_strCurAnimName.clear();
	if (m_pAnimator) {
		//m_pAnimator->Stop();          // ��� ���� (�ִٸ�)
		m_pAnimator->ResetAnimation();
	}
	if (auto* sr = GetOwner()->GetComponent<CSpriteRenderer>()) {
		sr->SetTexture(nullptr);      // ������ ������ ǥ�� ����
	}
}

void CBaseSkillScript::PlayerSkillAttack(CMonster* _pMonster)
{
	if (_pMonster)
	{
		bool isCriticalHit = (rand() % 10) < 10;  // 20% Ȯ���� ũ��Ƽ�� ��Ʈ

		int attackDamage;
		if (isCriticalHit)
		{
			// ũ��Ƽ�� ������: 30 ~ 200
			attackDamage = 30 + rand() % 5000;  // rand() % 171�� 0 ~ 170 ������ �� ����
		}
		else
		{
			// �Ϲ� ������: 0 ~ 30
			attackDamage = rand() % 31;  // rand() % 31�� 0 ~ 30 ������ �� ����
		}

		// ���� ��ġ���� �������� ���
		Vector3 monsterPos = _pMonster->GetTransform()->GetWorldPosition();
		//CDamageManager::GetInst()->CreateDamage(attackDamage, monsterPos, isCriticalHit);

		// ���Ϳ� �������� ����
		_pMonster->ReduceHP(attackDamage);

		// ����� ���
		tMonInfo& monInfo = _pMonster->GetMonInfo();
		char debugBuffer[256];
		sprintf_s(debugBuffer,
			"[ATTACK] %s attacked %s for %d damage (%s). Remaining HP: %.1f\n",
			GetOwner()->GetName().c_str(),
			monInfo.strTag.c_str(),
			attackDamage,
			isCriticalHit ? "CRITICAL" : "normal",
			_pMonster->GetCurHp());
		OutputDebugStringA(debugBuffer);
	}
}

//void CBaseSkillScript::attack(Skill_State state)
//{
//	char dbg[256];
//	sprintf_s(dbg, "[DEBUG] attack start - state=%d time=%f\n", (int)state, CTimeManager::GetfDeltaTime());
//	OutputDebugStringA(dbg);
//
//	if (!m_pAnimator)
//		m_pAnimator = GetOwner()->GetComponent<CAnimator>();
//	if (!m_pAnimator)
//		m_pAnimator = GetOwner()->AddComponent<CAnimator>();
//
//	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
//	if (!sr)
//		sr = GetOwner()->AddComponent<CSpriteRenderer>();
//
//	m_pAnimator->SetSpriteRenderer(sr);
//
//	wchar_t buf[256];
//	sprintf_s(dbg, "[DEBUG] attack called state=%d, animMapSize=%d, this=%p\n",
//		(int)state, (int)m_AnimMap.size(), this);
//	OutputDebugStringA(dbg);
//
//	if (m_AnimMap.find(state) == m_AnimMap.end())
//	{
//		if (state == Skill_State::SS_Idle || state == Skill_State::SS_End) {
//			FinishAndHide();
//			return;
//		}
//		// �� �� ���´� ��¥ ���� �����̴� ���� �α�
//		swprintf(buf, 256, L"[ERROR] No animation data found for state=%d. Available states:", (int)state);
//		OutputDebugStringW(buf);
//		for (auto& kv : m_AnimMap) {
//			swprintf(buf, 256, L" %d", (int)kv.first);
//			OutputDebugStringW(buf);
//		}
//		OutputDebugStringW(L"\n");
//		return;
//	}
//
//	const SkillAnimInfo& animData = m_AnimMap[state];
//	std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
//	std::wstring animName = std::to_wstring(static_cast<int>(state)) + L"_" + direction;
//
//	sprintf_s(dbg,
//		"[DEBUG] attack: state=%d name=%ws cur=%ws isPlaying=%d has=%d\n",
//		(int)state, animName.c_str(), m_strCurAnimName.c_str(),
//		(m_pAnimator ? m_pAnimator->IsPlaying() : -1),
//		(m_pAnimator ? m_pAnimator->HasAnimation(animName) : -1)
//	);
//	OutputDebugStringA(dbg);
//
//	if (m_strCurAnimName == animName)
//	{
//		if (!m_pAnimator->IsPlaying())
//		{
//			m_pAnimator->ResetAnimation();
//			m_pAnimator->Play(animName, false);
//		}
//		return;
//	}
//
//	if (!m_pAnimator->HasAnimation(animName))
//	{
//		std::wstring path = animData.pathPattern;
//		// path���� %s �� direction ġȯ
//		size_t pos = path.find(L"%s");
//		if (pos != std::wstring::npos)
//			path.replace(pos, 2, direction);
//
//		m_pAnimator->AddFrameAnimation(
//			animName,
//			path.c_str(),
//			animData.frameCount,
//			Vector2(0.f, 0.f),
//			animData.frameSize.x,
//			animData.frameSize.y,
//			0.f, 0.f,
//			animData.frameDuration
//		);
//	}
//
//
//	/*if (state == Skill_State::SS_Idle)
//	{
//		function<void()>& startEvent = m_pAnimator->GetStartEvent(animName);
//		startEvent = [this]() {
//			OnAnimationCycleComplete();
//		};
//	}*/
//
//	if (state == Skill_State::SS_Casting) {
//		auto& completeEvent = m_pAnimator->GetCompleteEvent(animName);
//		completeEvent = [this]() { OnAnimationCycleComplete(); };
//	}
//
//	m_pAnimator->Play(animName, false);
//	m_strCurAnimName = animName;
//
//	// Animator Play ����
//	sprintf_s(dbg, "[DEBUG] Play called at time=%f\n", CTimeManager::GetfDeltaTime());
//	OutputDebugStringA(dbg);
//}

//void CBaseSkillScript::EndSkillAnimation()
//{
//	if (m_pSkillOwner)
//	{
//		// ��ų ���¸� ��ٿ����� ����
//		m_pSkillOwner->SetSkillState(Skill_State::SS_Idle);
//
//		// ��ų ��ü ��Ȱ��ȭ
//		GetOwner()->SetActive(false);
//
//		if (m_pAnimator)
//		{
//			m_pAnimator->End();
//			m_pAnimator->ResetAnimation();
//
//			if (auto* sr = GetOwner()->GetComponent<CSpriteRenderer>())
//				sr->SetTexture(nullptr);
//		}
//		m_strCurAnimName.clear();
//	}
//}