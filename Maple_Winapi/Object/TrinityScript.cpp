#include "TrinityScript.h"
#include "../Object/Trinity.h"
#include "../Component/CAnimator.h"
#include "../Object/CGameObject.h"
#include "../Scene/CScene.h"
#include "../Manager/CTimeManager.h"

TrinityScript::TrinityScript() :
    CScript(SCRIPT_TYPE::ST_SkillScript)
{
}

TrinityScript::~TrinityScript()
{
}

void TrinityScript::OnInit()
{
    if (!m_pOwner) return;

    // 충돌체 참조
    m_pDetectedCollision = dynamic_cast<Trinity*>(m_pOwner)->GetDetectedCollider();
    m_pHitCollision = dynamic_cast<Trinity*>(m_pOwner)->GetHitCollider();

    // 초기 비활성화
    if (m_pDetectedCollision)
    {
        m_pDetectedCollision->SetColliderActive(false);
        m_pDetectedCollision->Enable(false); // 선택적으로 렌더링도 비활성화
    }

    if (m_pHitCollision)
    {
        m_pHitCollision->SetColliderActive(false);
        m_pHitCollision->Enable(false); // 선택적으로 렌더링도 비활성화
    }

    // 애니메이션
    CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
    if (!animator) return;

    CAnimation* trinityAnim = animator->FindAnimation(L"Trinity_Right");
    if (!trinityAnim) return;

    // 히트박스 ON (프레임 3)
    trinityAnim->SetFrameEvent(3, [this]() {
        ActivateHitbox();
        UpdateHitboxPosition();
    });

    // 히트박스 OFF (프레임 7)
    trinityAnim->SetFrameEvent(7, [this]() {
        DeactivateHitbox();
    });
}

void TrinityScript::OnUpdate()
{
    if (skill)
        skill->Update();

    // 스킬 공격 타이밍 체크
    if (hitboxActive)
    {
        attackTimer -= CTimeManager::GetfDeltaTime();
        if (attackTimer <= 0.0f)
        {
            // 히트박스 비활성화
            if (m_pHitCollision)
            {
                m_pHitCollision->SetColliderActive(false); // 충돌 체크 비활성화
                m_pHitCollision->Enable(false);            // 렌더링/업데이트 비활성화 (선택)
            }

            hitboxActive = false;
        }
    }
}

void TrinityScript::OnLateUpdate()
{
    if (skill->GetSkillState() == Skill_State::SS_Idle)
    {
        // 스킬이 종료되면 감지 박스 비활성화
        if (m_pDetectedCollision)
        {
            m_pDetectedCollision->SetColliderActive(false);
            m_pDetectedCollision->Enable(false); // 선택적으로 렌더링도 비활성화
        }
    }
}

void TrinityScript::OnRender(const Matrix& view, const Matrix& projection)
{
}

void TrinityScript::OnCollisionEnter(CCollider* _pOther)
{
    /*if (hitboxActive && _pOther->GetName() == L"Enemy")
    {
        float damage = skill->GetDamage();
        _pOther->GetOwner()->TakeDamage(damage);
    }*/
}

void TrinityScript::OnCollisionStay(CCollider* _pOther)
{
}

void TrinityScript::OnCollisionExit(CCollider* _pOther)
{
}

void TrinityScript::attack(Skill_State state)
{
    // 4. attack이 실행되는지
    OutputDebugStringA("TrinityScript::attack() called\n");

    if (!m_pAnimator)
        m_pAnimator = GetOwner()->GetComponent<CAnimator>();
    if (!m_pAnimator)
        m_pAnimator = GetOwner()->AddComponent<CAnimator>();

    if (!m_pAnimator) {
        OutputDebugStringA("ERROR: m_pAnimator is null!\n");
        return;
    }
    OutputDebugStringA("m_pAnimator is valid\n");

    CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
    if (!sr)
        sr = GetOwner()->AddComponent<CSpriteRenderer>();

    if (!sr) {
        OutputDebugStringA("ERROR: SpriteRenderer is null!\n");
        return;
    }
    OutputDebugStringA("SpriteRenderer is valid\n");

    m_pAnimator->SetSpriteRenderer(sr);

    // 애니메이션 정보 확인
    if (m_SkillAniMap.find(state) == m_SkillAniMap.end())
    {
        OutputDebugStringA("ERROR: No animation data found for state\n");
        return;
    }

    const SkillAnimInfo& animData = m_SkillAniMap[state];
    std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
    std::wstring animName = L"Trinity_" + direction;

    // 현재 같은 애니메이션이 재생 중이면 중복 재생 방지
   /* if (m_strCurAnimName == animName && m_pAnimator->IsPlaying())
    {
        OutputDebugStringA("Animation already playing, skipping\n");
        return;
    }*/

    // 애니메이션이 없으면 추가
    auto anim = m_pAnimator->GetCurrentAnimation();
    if (!anim || anim->GetCurFrameIndex() == 0)
    {
        std::wstring path = animData.pathPattern;
        size_t pos = path.find(L"%s");
        if (pos != std::wstring::npos)
            path.replace(pos, 2, direction);

        OutputDebugString((L"[DEBUG] Reloading animation from " + path + L"\n").c_str());

        m_pAnimator->AddFrameAnimation(
            animName,
            path.c_str(),
            animData.frameCount,
            Vector2(0.f, 0.f),
            animData.frameSize.x,
            animData.frameSize.y,
            0.f, 0.f,
            animData.frameDuration
        );
    }
    /*if (!m_pAnimator->HasAnimation(animName) ||
        m_pAnimator->GetCurrentAnimation()->GetCurFrameIndex() == 0)
    {
        std::wstring path = animData.pathPattern;
        size_t pos = path.find(L"%s");
        if (pos != std::wstring::npos)
            path.replace(pos, 2, direction);

        m_pAnimator->AddFrameAnimation(
            animName,
            path.c_str(),
            animData.frameCount,
            Vector2(0.f, 0.f),
            animData.frameSize.x,
            animData.frameSize.y,
            0.f, 0.f,
            animData.frameDuration
        );
    }*/

    // 5. Play 전 애니메이션이 재생이 되는지
    bool hasAnim = m_pAnimator->HasAnimation(animName);
    wchar_t dbg[256];
    swprintf(dbg, 256, L"[DEBUG] Play %s, hasAnim=%d\n", animName.c_str(), hasAnim);
    OutputDebugStringW(dbg);

    /*auto anim = m_pAnimator->GetCurrentAnimation();
    if (anim)
    {
        wchar_t buf[128];
        swprintf(buf, 128, L"[DEBUG] Frame count: %d\n", anim->GetCurFrameIndex());
        OutputDebugStringW(buf);
    }*/

    // 애니메이션 재생
    m_pAnimator->Play(animName, true);
    m_strCurAnimName = animName;
}

void TrinityScript::ActivateHitbox()
{
    if (m_pDetectedCollision)
    {
        m_pDetectedCollision->SetColliderActive(true);
        m_pDetectedCollision->Enable(true); // 선택적으로 렌더링도 활성화
    }

    if (m_pHitCollision)
    {
        m_pHitCollision->SetColliderActive(true);
        m_pHitCollision->Enable(true); // 선택적으로 렌더링도 활성화
    }

    hitboxActive = true;
    attackTimer = hitboxDuration;
}

void TrinityScript::DeactivateHitbox()
{
    if (m_pDetectedCollision)
    {
        m_pDetectedCollision->SetColliderActive(false);
        m_pDetectedCollision->Enable(false); // 선택적으로 렌더링도 활성화
    }

    if (m_pHitCollision)
    {
        m_pHitCollision->SetColliderActive(false);
        m_pHitCollision->Enable(false); // 선택적으로 렌더링도 활성화
    }

    hitboxActive = false;
}

void TrinityScript::UpdateHitboxPosition()
{
    if (!m_pPlayer) return;

    Vector3 playerPos = m_pPlayer->GetTransform()->GetWorldPosition();

    if (m_pDetectedCollision)
    {
        CTransform* tr = m_pDetectedCollision->GetOwner()->GetComponent<CTransform>();
        if (tr) tr->SetWorldPosition(playerPos);
    }

    if (m_pHitCollision)
    {
        CTransform* tr = m_pHitCollision->GetOwner()->GetComponent<CTransform>();
        if (tr) tr->SetWorldPosition(playerPos);
    }
}

//animator->GetStartEvent(L"Trinity") = [this]()
    //{
    //    hitboxActive = false;  // 초기 비활성화
    //    DetectedCollision->SetActive(false);
    //    hitCollision->SetActive(false);
    //};

    //// 특정 프레임에서 공격 유효 처리
    //animator->GetCompleteEvent(L"Trinity") = [this]()
    //{
    //    hitboxActive = true;
    //    DetectedCollision->SetActive(true);
    //    hitCollision->SetActive(true);

    //    // 플레이어 위치에 따라 위치 갱신
    //    Vector3 pos = GetPlayer()->GetWorldPosition();
    //    DetectedCollision->SetLocalPosition(pos);
    //    hitCollision->SetLocalPosition(pos);
    //};

    //// 애니메이션 종료 후 비활성화
    //animator->GetEndEvent(L"Trinity") = [this]()
    //{
    //    hitboxActive = false;
    //    DetectedCollision->SetActive(false);
    //    hitCollision->SetActive(false);
    //};