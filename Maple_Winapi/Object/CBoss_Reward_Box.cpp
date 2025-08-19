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

    // ��� ������ 1
    DropItemInfo item1;
    item1.strItemName = L"source_of_pain";
    item1.fDropRate = 1.0f;      // 80% Ȯ��
    item1.iMinCount = 1;
    item1.iMaxCount = 1;

    // ��� ������ 2
    //DropItemInfo item2;
    //item2.strItemName = L"Golden_Shard";
    //item2.fDropRate = 0.3f;      // 30% Ȯ��
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

    // Transform ������Ʈ �������� (�̹� �θ𿡼� ������)
    m_pTransform = this->GetComponent<CTransform>();
    if (m_pTransform) {
        m_pTransform->SetLocalScale(Vector3(87.0f, 108.0f, 0.0f));

        // �����: ���� ��ü�� ������Ʈ���� �ּ� Ȯ��
        char buffer[256];
        sprintf_s(buffer, "[REWARD_BOX INIT] REWARD_BOX �ּ�: %p, Transform �ּ�: %p\n", this, m_pTransform);
        OutputDebugStringA(buffer);

        // Gravity ������Ʈ Ȯ��
        CGravity* gravity = this->GetComponent<CGravity>();
        if (gravity) {
            sprintf_s(buffer, "[REWARD_BOX INIT] Gravity �ּ�: %p\n", gravity);
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