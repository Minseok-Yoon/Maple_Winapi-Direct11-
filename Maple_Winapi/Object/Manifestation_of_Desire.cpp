#include "../pch.h"
#include "Manifestation_of_Desire.h"
#include "../Component/CGravity.h"

Manifestation_of_Desire::Manifestation_of_Desire()
{
	SetName(L"Manifestation_of_Desire");

	tMonInfo info = {};
	info.strTag = "Manifestation_of_Desire";
	info.fHP = 300.0f;

	FieldMonsterInfo fieldMonInfo = {};

	SetMonInfo(info);
	SetFieldMonInfo(fieldMonInfo);
}

Manifestation_of_Desire::~Manifestation_of_Desire()
{
}

void Manifestation_of_Desire::Init()
{
    CMonster::Init();

    // Transform 컴포넌트 가져오기 (이미 부모에서 생성됨)
    m_pTransform = this->GetComponent<CTransform>();
    if (m_pTransform) {
        m_pTransform->SetLocalScale(Vector3(87.0f, 108.0f, 0.0f));

        // 디버깅: 현재 객체와 컴포넌트들의 주소 확인
        char buffer[256];
        sprintf_s(buffer, "[ERDAS INIT] Erdas 주소: %p, Transform 주소: %p\n", this, m_pTransform);
        OutputDebugStringA(buffer);

        // Gravity 컴포넌트 확인
        CGravity* gravity = this->GetComponent<CGravity>();
        if (gravity) {
            sprintf_s(buffer, "[ERDAS INIT] Gravity 주소: %p\n", gravity);
            OutputDebugStringA(buffer);
        }
        else {
            OutputDebugStringA("[ERDAS INIT] ERROR: Gravity not found!\n");
        }
    }
    else {
        OutputDebugStringA("ERROR: Transform not found in Erdas_of_Gladness::Init()\n");
    }

    CMonsterScript* pScript = GetComponent<CMonsterScript>();
    if (pScript)
    {
        MonsterAnimInfo idleData = {
            L"../Resources/Texture/Monster/Erdas/Gladness/Stand/%s/Gladness_stand_%d.png",
            8, 0.1f, Vector2(87.f, 108.f)
        };
        pScript->SetAnimationData(MON_STATE::MS_Idle, idleData);

        MonsterAnimInfo moveData = {
            L"../Resources/Texture/Monster/Erdas/Gladness/Move/%s/Gladness_move_%d.png",
            6, 0.1f, Vector2(87.f, 108.f)
        };
        pScript->SetAnimationData(MON_STATE::MS_Move, moveData);

        MonsterAnimInfo attackData = {
            L"../Resources/Texture/Monster/Erdas/Gladness/Attack/%s/Gladness_attack_%d.png",
            10, 0.08f, Vector2(87.f, 108.f)
        };
        pScript->SetAnimationData(MON_STATE::MS_Attack, attackData);

        MonsterAnimInfo deadData = {
            L"../Resources/Texture/Monster/Erdas/Gladness/Dead/%s/Gladness_Dead_%d.png",
            12, 0.12f, Vector2(87.f, 108.f)
        };
        pScript->SetAnimationData(MON_STATE::MS_Dead, deadData);
    }
}

void Manifestation_of_Desire::Update()
{
    CMonster::Update();
}

void Manifestation_of_Desire::LateUpdate()
{
    CMonster::LateUpdate();
}

void Manifestation_of_Desire::Render(const Matrix& view, const Matrix& projection)
{
    CMonster::Render(view, projection);
}
