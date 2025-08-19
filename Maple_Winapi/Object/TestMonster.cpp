#include "../pch.h"
#include "TestMonster.h"
#include "../Component/CGravity.h"

TestMonster::TestMonster()
{
	SetName(L"TestMonster");

    tMonInfo info = {};
    info.strTag = "TestMonster";
    info.fHP = 30000000.0f;

    FieldMonsterInfo fieldMonInfo = {};
    fieldMonInfo.fRespawnTimer = 6.0f;

    SetMonInfo(info);
    SetFieldMonInfo(fieldMonInfo);
}

TestMonster::~TestMonster()
{
}

void TestMonster::Init()
{
    CMonster::Init();

    // Transform ������Ʈ �������� (�̹� �θ𿡼� ������)
    m_pTransform = this->GetComponent<CTransform>();
    if (m_pTransform) {
        m_pTransform->SetLocalScale(Vector3(157.0f, 146.0f, 0.0f));

        char buffer[256];
        sprintf_s(buffer, "[TestMonster INIT] TestMonster �ּ�: %p, Transform �ּ�: %p\n", this, m_pTransform);
        OutputDebugStringA(buffer);

        // Gravity ������Ʈ Ȯ��
        CGravity* gravity = this->GetComponent<CGravity>(); 
        if (gravity) {
            sprintf_s(buffer, "[TestMonster INIT] Gravity �ּ�: %p\n", gravity);
            OutputDebugStringA(buffer);
        }
        else {
            OutputDebugStringA("[TestMonster INIT] ERROR: Gravity not found!\n");
        }
    }
    else {
        OutputDebugStringA("ERROR: Transform not found in TestMonster::Init()\n");
    }

    CMonsterScript* pScript = GetComponent<CMonsterScript>();
    if (pScript)
    {
        MonsterAnimInfo idleData = {
            L"../Resources/Texture/Monster/TestMonster/%s/TestMonster_%d.png",
            6, 0.2f, Vector2(157.0f, 146.0f)
        };
        pScript->SetAnimationData(MON_STATE::MS_Idle, idleData);
    }
}

void TestMonster::Update()
{
    CMonster::Update();
}

void TestMonster::LateUpdate()
{
    CMonster::LateUpdate();
}

void TestMonster::Render(const Matrix& view, const Matrix& projection)
{
    CMonster::Render(view, projection);
}
