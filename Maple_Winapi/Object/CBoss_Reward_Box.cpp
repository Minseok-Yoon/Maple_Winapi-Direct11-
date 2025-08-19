#include "../pch.h"
#include "CBoss_Reward_Box.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CGravity.h"

CBoss_Reward_Box::CBoss_Reward_Box()
{
	SetName(L"Boss_Reward_Box");

    CResourceManager::Load<CTexture>(L"source_of_pain", L"../Resources/Texture/Monster/DropItem/JinhliaBox/Source_of_Pain.png");

    tMonInfo info = {};
    info.strTag = "Boss_Reward_Box";
    info.fHP = 100.0f;

    // 드랍 아이템 1
    DropItemInfo item1;
    item1.strItemName = L"source_of_pain";
    item1.fDropRate = 1.0f;      // 80% 확률
    item1.iMinCount = 1;
    item1.iMaxCount = 1;

    // 드랍 아이템 2
    //DropItemInfo item2;
    //item2.strItemName = L"Golden_Shard";
    //item2.fDropRate = 0.3f;      // 30% 확률
    //item2.iMinCount = 1;
    //item2.iMaxCount = 1;

    info.vecDropItems.push_back(item1);
    //info.vecDropItems.push_back(item2);

    FieldMonsterInfo fieldMonInfo = {};
    fieldMonInfo.fRespawnTimer = 0.0f;

    SetMonInfo(info);
    SetFieldMonInfo(fieldMonInfo);
    SetDropItems(info.vecDropItems);
}

CBoss_Reward_Box::~CBoss_Reward_Box()
{
}

void CBoss_Reward_Box::Init()
{
    CMonster::Init();

    // Transform 컴포넌트 가져오기 (이미 부모에서 생성됨)
    m_pTransform = this->GetComponent<CTransform>();
    if (m_pTransform) {
        m_pTransform->SetLocalScale(Vector3(87.0f, 108.0f, 0.0f));

        // 디버깅: 현재 객체와 컴포넌트들의 주소 확인
        char buffer[256];
        sprintf_s(buffer, "[REWARD_BOX INIT] REWARD_BOX 주소: %p, Transform 주소: %p\n", this, m_pTransform);
        OutputDebugStringA(buffer);

        // Gravity 컴포넌트 확인
        CGravity* gravity = this->GetComponent<CGravity>();
        if (gravity) {
            sprintf_s(buffer, "[REWARD_BOX INIT] Gravity 주소: %p\n", gravity);
            OutputDebugStringA(buffer);
        }
        else {
            OutputDebugStringA("[REWARD_BOX INIT] ERROR: Gravity not found!\n");
        }
    }
    else {
        OutputDebugStringA("ERROR: Transform not found in REWARD_BOX::Init()\n");
    }
}

void CBoss_Reward_Box::Update()
{
    CMonster::Update();
}

void CBoss_Reward_Box::LateUpdate()
{
    CMonster::LateUpdate();
}

void CBoss_Reward_Box::Render(const Matrix& view, const Matrix& projection)
{
    CMonster::Render(view, projection);
}