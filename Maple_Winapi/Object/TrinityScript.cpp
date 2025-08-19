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

    // �浹ü ����
    m_pDetectedCollision = dynamic_cast<Trinity*>(m_pOwner)->GetDetectedCollider();
    m_pHitCollision = dynamic_cast<Trinity*>(m_pOwner)->GetHitCollider();

    // �ʱ� ��Ȱ��ȭ
    if (m_pDetectedCollision)
    {
        m_pDetectedCollision->SetColliderActive(false);
        m_pDetectedCollision->Enable(false); // ���������� �������� ��Ȱ��ȭ
    }

    if (m_pHitCollision)
    {
        m_pHitCollision->SetColliderActive(false);
        m_pHitCollision->Enable(false); // ���������� �������� ��Ȱ��ȭ
    }

    // �ִϸ��̼�
    CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
    if (!animator) return;

    CAnimation* trinityAnim = animator->FindAnimation(L"Trinity_Right");
    if (!trinityAnim) return;

    // ��Ʈ�ڽ� ON (������ 3)
    trinityAnim->SetFrameEvent(3, [this]() {
        ActivateHitbox();
        UpdateHitboxPosition();
    });

    // ��Ʈ�ڽ� OFF (������ 7)
    trinityAnim->SetFrameEvent(7, [this]() {
        DeactivateHitbox();
    });
}

void TrinityScript::OnUpdate()
{
    if (skill)
        skill->Update();

    // ��ų ���� Ÿ�̹� üũ
    if (hitboxActive)
    {
        attackTimer -= CTimeManager::GetfDeltaTime();
        if (attackTimer <= 0.0f)
        {
            // ��Ʈ�ڽ� ��Ȱ��ȭ
            if (m_pHitCollision)
            {
                m_pHitCollision->SetColliderActive(false); // �浹 üũ ��Ȱ��ȭ
                m_pHitCollision->Enable(false);            // ������/������Ʈ ��Ȱ��ȭ (����)
            }

            hitboxActive = false;
        }
    }
}

void TrinityScript::OnLateUpdate()
{
    if (skill->GetSkillState() == Skill_State::SS_Idle)
    {
        // ��ų�� ����Ǹ� ���� �ڽ� ��Ȱ��ȭ
        if (m_pDetectedCollision)
        {
            m_pDetectedCollision->SetColliderActive(false);
            m_pDetectedCollision->Enable(false); // ���������� �������� ��Ȱ��ȭ
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
    // 4. attack�� ����Ǵ���
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

    // �ִϸ��̼� ���� Ȯ��
    if (m_SkillAniMap.find(state) == m_SkillAniMap.end())
    {
        OutputDebugStringA("ERROR: No animation data found for state\n");
        return;
    }

    const SkillAnimInfo& animData = m_SkillAniMap[state];
    std::wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
    std::wstring animName = L"Trinity_" + direction;

    // ���� ���� �ִϸ��̼��� ��� ���̸� �ߺ� ��� ����
   /* if (m_strCurAnimName == animName && m_pAnimator->IsPlaying())
    {
        OutputDebugStringA("Animation already playing, skipping\n");
        return;
    }*/

    // �ִϸ��̼��� ������ �߰�
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

    // 5. Play �� �ִϸ��̼��� ����� �Ǵ���
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

    // �ִϸ��̼� ���
    m_pAnimator->Play(animName, true);
    m_strCurAnimName = animName;
}

void TrinityScript::ActivateHitbox()
{
    if (m_pDetectedCollision)
    {
        m_pDetectedCollision->SetColliderActive(true);
        m_pDetectedCollision->Enable(true); // ���������� �������� Ȱ��ȭ
    }

    if (m_pHitCollision)
    {
        m_pHitCollision->SetColliderActive(true);
        m_pHitCollision->Enable(true); // ���������� �������� Ȱ��ȭ
    }

    hitboxActive = true;
    attackTimer = hitboxDuration;
}

void TrinityScript::DeactivateHitbox()
{
    if (m_pDetectedCollision)
    {
        m_pDetectedCollision->SetColliderActive(false);
        m_pDetectedCollision->Enable(false); // ���������� �������� Ȱ��ȭ
    }

    if (m_pHitCollision)
    {
        m_pHitCollision->SetColliderActive(false);
        m_pHitCollision->Enable(false); // ���������� �������� Ȱ��ȭ
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
    //    hitboxActive = false;  // �ʱ� ��Ȱ��ȭ
    //    DetectedCollision->SetActive(false);
    //    hitCollision->SetActive(false);
    //};

    //// Ư�� �����ӿ��� ���� ��ȿ ó��
    //animator->GetCompleteEvent(L"Trinity") = [this]()
    //{
    //    hitboxActive = true;
    //    DetectedCollision->SetActive(true);
    //    hitCollision->SetActive(true);

    //    // �÷��̾� ��ġ�� ���� ��ġ ����
    //    Vector3 pos = GetPlayer()->GetWorldPosition();
    //    DetectedCollision->SetLocalPosition(pos);
    //    hitCollision->SetLocalPosition(pos);
    //};

    //// �ִϸ��̼� ���� �� ��Ȱ��ȭ
    //animator->GetEndEvent(L"Trinity") = [this]()
    //{
    //    hitboxActive = false;
    //    DetectedCollision->SetActive(false);
    //    hitCollision->SetActive(false);
    //};