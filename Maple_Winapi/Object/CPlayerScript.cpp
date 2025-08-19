#include "CPlayerScript.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"
#include "../Object/CPlayer.h"
#include "../Component/CAnimator.h"
#include "../Component/CGravity.h"
#include "../Component/CTransform.h"
#include "../Component/CRigidBody.h"
#include "../Component/CRenderer.h"
#include "../Component/CPixelCollider.h"
#include "../Object/CItem.h"
#include "CInventory.h"
#include "Trinity.h"
#include "../Manager/CSkillManager.h"

CPlayerScript::CPlayerScript() :
	CScript(SCRIPT_TYPE::ST_PlayerScript),
	m_ePlayerState(PLAYER_STATE::PS_Idle),
	m_iDir(1),
	m_pPixelCollider(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::OnInit()
{
}

void CPlayerScript::OnUpdate()
{
	// 플레이어 객체 가져오기
	CGameObject* pPlayer = renderer::selectedObject;

	if (KEY_HOLD(KEY_CODE::RIGHT) || KEY_TAP(KEY_CODE::RIGHT))
	{
		m_iDir = 1;
	}
	if (KEY_HOLD(KEY_CODE::LEFT) || KEY_TAP(KEY_CODE::LEFT))
	{
		m_iDir = -1;
	}

	if (pPlayer != nullptr)
	{
		m_pAnimator = pPlayer->GetComponent<CAnimator>();
	}

	if (m_pAnimator == nullptr)
	{
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}
	m_pAnimator->SetSpriteRenderer(sr);

	switch (m_ePlayerState)
	{
	case PLAYER_STATE::PS_Idle:
		idle(m_iDir);
		break;
	case PLAYER_STATE::PS_Walk:
		move(m_iDir);
		break;
	case PLAYER_STATE::PS_Attack:
		attack(m_iDir);
		break;
	case PLAYER_STATE::PS_Prone:
		prone(m_iDir);
		break;
	case PLAYER_STATE::PS_Jump:
		jump(m_iDir);
		break;
	}

	// 아이템 줍기
	if (KEY_TAP(KEY_CODE::Z) && m_pColliderItem)
	{
		std::wstring itemName = m_pColliderItem->GetName();

		// 인벤토리에 아이템 추가
		CInventory::GetInst()->AddItem(itemName, 1);

		// 아이템을 습득했으니 배경에서 아이템 제거
		Destroy(m_pColliderItem);  // 혹은 RemoveObject, 비활성화 등 프로젝트 방식에 따라 다름
		m_pColliderItem = nullptr;

		std::wstring debugStr = L"[Player] 아이템 습득: " + itemName + L"\n";
		OutputDebugString(debugStr.c_str());
	}

	if (KEY_TAP(KEY_CODE::N) || KEY_HOLD(KEY_CODE::N))
	{
		//OutputDebugStringA("[DEBUG] N key pressed - Activating Trinity skill\n");
		CSkillManager::GetInst()->ActiveSkill(SKILL_TYPE::ST_Trinity);
	}

	if (KEY_AWAY(KEY_CODE::N))
	{
		//OutputDebugStringA("[DEBUG] N key released - Deactivating Trinity skill\n");
		CSkillManager::GetInst()->DeactiveSkill(SKILL_TYPE::ST_Trinity);

	}

	//CPlayer* player = dynamic_cast<CPlayer*>(GetOwner());
	// 
	//if (KEY_TAP(KEY_CODE::N) || KEY_HOLD(KEY_CODE::N))
	//{
	//	// 1. N키 입력이 실제로 감지되는지
	//	OutputDebugStringA("[DEBUG] N key detected\n");
	//	if (player->pTrinity)
	//		player->pTrinity->UseSkill();
	//}

	//if (KEY_AWAY(KEY_CODE::N))
	//{
	//	// 1. N키 입력이 실제로 감지되는지
	//	OutputDebugStringA("[DEBUG] N key detected\n");
	//	if (player->pTrinity)
	//		player->pTrinity->EndSkill();
	//}
}

void CPlayerScript::OnLateUpdate()
{
}

void CPlayerScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void CPlayerScript::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayerScript::OnCollisionStay(CCollider* _pOther)
{
	if (_pOther && dynamic_cast<CMonster*>(_pOther->GetOwner()))
	{
		m_pColliderMonster = _pOther->GetOwner();
	}

	// 아이템 감지
	if (_pOther && dynamic_cast<CItem*>(_pOther->GetOwner()))
	{
		m_pColliderItem = _pOther->GetOwner();
	}
}

void CPlayerScript::OnCollisionExit(CCollider* _pOther)
{
	if (_pOther && _pOther->GetOwner() == m_pColliderItem)
	{
		m_pColliderItem = nullptr;
	}
}

void CPlayerScript::PlayerAttack(CMonster* _pMonster)
{
	if (_pMonster)
	{
		bool isCriticalHit = (rand() % 10) < 2;  // 20% 확률로 크리티컬 히트

		int attackDamage;
		if (isCriticalHit)
		{
			// 크리티컬 데미지: 30 ~ 200
			attackDamage = 30 + rand() % 171;  // rand() % 171은 0 ~ 170 범위의 값 생성
		}
		else
		{
			// 일반 데미지: 0 ~ 30
			attackDamage = rand() % 31;  // rand() % 31은 0 ~ 30 범위의 값 생성
		}

		// 몬스터 위치에서 데미지를 출력
		Vector3 monsterPos = _pMonster->GetTransform()->GetWorldPosition();
		//CDamageManager::GetInst()->CreateDamage(attackDamage, monsterPos, isCriticalHit);

		// 몬스터에 데미지를 입힘
		_pMonster->ReduceHP(attackDamage);

		// 디버그 출력
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

//Vector2 CPlayerScript::GetSlopeAdjustedForce(Vector2 baseForce)
//{
//	CPlayer* player = dynamic_cast<CPlayer*>(GetOwner());
//	if (!player) return baseForce;
//
//	// 경사각 계산 (간단한 방법)
//	Vector3 currentPos = GetOwner()->GetTransform()->GetWorldPosition();
//	Vector3 frontPos = currentPos;
//	frontPos.x += (baseForce.x > 0) ? 5.0f : -5.0f;
//
//	float currentY = player->FindClosestGroundY(currentPos);
//	float frontY = player->FindClosestGroundY(frontPos);
//
//	float slope = (frontY - currentY) / 5.0f;
//
//	// 경사가 있으면 Y 방향 Force 추가
//	if (abs(slope) > 0.1f)
//	{
//		baseForce.y = slope * 100.0f; // 경사에 따른 Y Force
//	}
//
//	return baseForce;
//}

void CPlayerScript::idle(int _iDir)
{
	CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	if (rb)
		rb->ClearForce();

	m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	if (m_pAnimator == nullptr)
	{
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Idle";
	wstring filePathPattern = L"../Resources/Texture/Player/Idle/" + direction + L"/%d.png";  // %d로 프레임 번호 변경
	int frameCount = 3;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	m_pAnimator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	//animator->SetSpriteRenderer(sr);
	m_pAnimator->Play(animationName, true);  // 반복 재생

	if (KEY_HOLD(KEY_CODE::LEFT) || KEY_TAP(KEY_CODE::LEFT) ||
		KEY_HOLD(KEY_CODE::RIGHT) || KEY_TAP(KEY_CODE::RIGHT))
	{
		m_ePlayerState = PLAYER_STATE::PS_Walk;
	}

	if (KEY_HOLD(KEY_CODE::DOWN) || KEY_TAP(KEY_CODE::DOWN))
	{
		m_ePlayerState = PLAYER_STATE::PS_Prone;
	}

	if (KEY_HOLD(KEY_CODE::ALT) || KEY_TAP(KEY_CODE::ALT))
	{
		m_ePlayerState = PLAYER_STATE::PS_Jump;
	}

	if (KEY_HOLD(KEY_CODE::X) || KEY_TAP(KEY_CODE::X))
	{
		m_ePlayerState = PLAYER_STATE::PS_Attack;
	}
}

void CPlayerScript::move(int _iDir)
{
	CTransform* transform = GetOwner()->GetComponent<CTransform>();
	Vector3 pos = transform->GetWorldPosition();

	float moveSpeed = 100.0f * CTimeManager::GetfDeltaTime(); // 프레임 독립적 이동

	if (KEY_HOLD(KEY_CODE::RIGHT))
	{
		pos.x += moveSpeed;
	}
	if (KEY_HOLD(KEY_CODE::LEFT))
	{
		pos.x -= moveSpeed;
	}

	transform->SetWorldPosition(pos);
	/*CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	rb->ClearForce();

	if (KEY_HOLD(KEY_CODE::RIGHT))
	{
		rb->AddForce(Vector2(200.0f, 0.0f));
	}
	if (KEY_HOLD(KEY_CODE::LEFT))
	{
		rb->AddForce(Vector2(-200.0f, 0.0f));
	}*/
	/*if (KEY_HOLD(KEY_CODE::DOWN))
	{
		rb->AddForce(Vector2(0.0f, -100.0f));
	}*/

	if (KEY_AWAY(KEY_CODE::RIGHT) || KEY_AWAY(KEY_CODE::LEFT) ||
		KEY_AWAY(KEY_CODE::UP) || KEY_AWAY(KEY_CODE::DOWN))
	{
		pos.x = 0.0f;
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}


	m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	if (m_pAnimator == nullptr)
	{
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Walk 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Walk" + direction;
	wstring filePathPattern = L"../Resources/Texture/Player/Walk/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 4;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	m_pAnimator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	m_pAnimator->Play(animationName, true);  // 반복 재생
}

void CPlayerScript::prone(int _iDir)
{
	m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	if (m_pAnimator == nullptr)
	{
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Prone";
	wstring filePathPattern = L"../Resources/Texture/Player/Prone/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 1;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	m_pAnimator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		80.0f,                 // 각 프레임의 너비
		37.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	m_pAnimator->Play(animationName, true);  // 반복 재생

	if(KEY_AWAY(KEY_CODE::DOWN))
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}
}

void CPlayerScript::jump(int _iDir)
{
	CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	rb->ClearForce();

	m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	if (m_pAnimator == nullptr)
	{
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Jump";
	wstring filePathPattern = L"../Resources/Texture/Player/Jump/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 1;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 1.0f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	m_pAnimator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	m_pAnimator->Play(animationName, false);  // 반복 재생

	// 2025-06-13
	CGravity* gravity = GetOwner()->GetComponent<CGravity>();
	if (gravity)
	{
		gravity->Jump(-500.0f);
	}

	// 애니메이션이 끝났으면 상태 초기화
	if (m_pAnimator->End())
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}
}

void CPlayerScript::attack(int _iDir)
{
	m_pAnimator = GetOwner()->GetComponent<CAnimator>();
	if (m_pAnimator == nullptr)
	{
		m_pAnimator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Attack 애니메이션 생성
	wstring animationName = L"Attack";
	wstring filePathPattern = L"../Resources/Texture/Player/Attack/Right/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 3;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.2f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	m_pAnimator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		55.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	m_pAnimator->Play(animationName, false);  // 반복되지 않도록 false 설정

	// 공격 타이밍 감지 및 데미지 처리
	if (!m_bHasDealtDamage && m_pAnimator->GetCurrentFrameIndex() == 1) // 예: 2번째 프레임에서 공격 판정
	{
		if (m_pColliderMonster)
		{
			CMonster* pMonster = dynamic_cast<CMonster*>(m_pColliderMonster);
			if (pMonster)
			{
				// 이 부분 PlayerScript가 문제 발생 playerScri가 nullptr
				CPlayerScript* playerScri = GetOwner()->GetComponent<CPlayerScript>();
				if (playerScri)
				{
					playerScri->PlayerAttack(pMonster);  // 실제 데미지 주는 함수 호출
					m_bHasDealtDamage = true;
				}
			}
		}
	}

	// 애니메이션이 끝났으면 상태 초기화
	if (m_pAnimator->End())
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
		m_bHasDealtDamage = false;
		m_pColliderMonster = nullptr;
	}
}