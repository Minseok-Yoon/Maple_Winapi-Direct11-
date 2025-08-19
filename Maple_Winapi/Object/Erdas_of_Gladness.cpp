#include "../pch.h"
#include "Erdas_of_Gladness.h"
#include "../Component/CTransform.h"
#include "../Component/CSpriteRenderer.h"
#include "../Object/CMonsterScript.h"
#include "../Resource/CTexture.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CGravity.h"

Erdas_of_Gladness::Erdas_of_Gladness()
{
	SetName(L"Erdas_of_Gladness");

    CResourceManager::Load<CTexture>(L"Piece_of_Gledness", L"../Resources/Texture/Monster/DropItem/Piece_of_Gledness.png");

    tMonInfo info = {};
    info.strTag = "Erdas_of_Gladness";
    info.fHP = 300.0f;
    
    DropItemInfo item1;
    item1.strItemName = L"Piece_of_Gledness";
    item1.fDropRate = 1.0f;      // 80% 확률
    item1.iMinCount = 1;
    item1.iMaxCount = 1;

    info.vecDropItems.push_back(item1);

    FieldMonsterInfo fieldMonInfo = {};
    fieldMonInfo.fRespawnTimer = 6.0f;

    SetMonInfo(info);
    SetFieldMonInfo(fieldMonInfo);
    SetDropItems(info.vecDropItems);
}

Erdas_of_Gladness::~Erdas_of_Gladness()
{
}

void Erdas_of_Gladness::Init()
{
	OutputDebugStringA("Erdas_of_Gladness::Init() 호출됨\n");

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

void Erdas_of_Gladness::Update()
{
	CMonster::Update();
}

void Erdas_of_Gladness::LateUpdate()
{
	CMonster::LateUpdate();
}

void Erdas_of_Gladness::Render(const Matrix& view, const Matrix& projection)
{
	CMonster::Render(view, projection);
}

//void Erdas_of_Gladness::Init()
//{
//	OutputDebugStringA("Erdas_of_Gladness::Init() 호출됨\n");
//
//	CMonster::Init();
//
//	m_pTransform = this->GetComponent<CTransform>();
//	m_pTransform->SetLocalScale(Vector3(87.0f, 108.0f, 0.0f));
//}

//OutputDebugStringA("Erdas_of_Gladness::Update() 호출됨\n");

//CMonster::Update();

/*CGravity* gravity = this->GetComponent<CGravity>();
if (!gravity)
    OutputDebugStringA("ERROR: Gravity not found on Erdas_of_Gladness\n");
else
    OutputDebugStringA("INFO: Gravity found on Erdas_of_Gladness\n");*/