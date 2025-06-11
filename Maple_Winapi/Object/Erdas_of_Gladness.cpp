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
    info.strDropItme = L"Piece_of_Gledness";

    FieldMonsterInfo fieldMonInfo = {};
    fieldMonInfo.fRespawnTimer = 6.0f;

    SetMonInfo(info);
    SetFieldMonInfo(fieldMonInfo);
    SetDropItem(info.strDropItme);
}

Erdas_of_Gladness::~Erdas_of_Gladness()
{
}

void Erdas_of_Gladness::Init()
{
	OutputDebugStringA("Erdas_of_Gladness::Init() ȣ���\n");

	CMonster::Init();

    // Transform ������Ʈ �������� (�̹� �θ𿡼� ������)
    m_pTransform = this->GetComponent<CTransform>();
    if (m_pTransform) {
        m_pTransform->SetLocalScale(Vector3(87.0f, 108.0f, 0.0f));

        // �����: ���� ��ü�� ������Ʈ���� �ּ� Ȯ��
        char buffer[256];
        sprintf_s(buffer, "[ERDAS INIT] Erdas �ּ�: %p, Transform �ּ�: %p\n", this, m_pTransform);
        OutputDebugStringA(buffer);

        // Gravity ������Ʈ Ȯ��
        CGravity* gravity = this->GetComponent<CGravity>();
        if (gravity) {
            sprintf_s(buffer, "[ERDAS INIT] Gravity �ּ�: %p\n", gravity);
            OutputDebugStringA(buffer);
        }
        else {
            OutputDebugStringA("[ERDAS INIT] ERROR: Gravity not found!\n");
        }
    }
    else {
        OutputDebugStringA("ERROR: Transform not found in Erdas_of_Gladness::Init()\n");
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
//	OutputDebugStringA("Erdas_of_Gladness::Init() ȣ���\n");
//
//	CMonster::Init();
//
//	m_pTransform = this->GetComponent<CTransform>();
//	m_pTransform->SetLocalScale(Vector3(87.0f, 108.0f, 0.0f));
//}

//OutputDebugStringA("Erdas_of_Gladness::Update() ȣ���\n");

//CMonster::Update();

/*CGravity* gravity = this->GetComponent<CGravity>();
if (!gravity)
    OutputDebugStringA("ERROR: Gravity not found on Erdas_of_Gladness\n");
else
    OutputDebugStringA("INFO: Gravity found on Erdas_of_Gladness\n");*/