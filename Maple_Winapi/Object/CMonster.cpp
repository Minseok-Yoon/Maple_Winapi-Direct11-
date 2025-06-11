#include "CMonster.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CGravity.h"
#include "../Component/CTransform.h"
#include "../Component/CSpriteRenderer.h"
#include "../Scene/CScene.h"
#include "../Resource/CTexture.h"
#include "../Object/CMonsterScript.h"
#include "../Object/CEtcItem.h"
#include "../Object/CObject.h"

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	delete m_pCollider;
}

void CMonster::Init()
{
    OutputDebugStringA("CMonster::Init() ȣ���\n");

    CGameObject::Init();

    // Transform ������Ʈ Ȯ�� �� �߰�
    if (!m_pTransform) {
        m_pTransform = this->AddComponent<CTransform>();
        if (!m_pTransform) {
            OutputDebugStringA("ERROR: Transform creation failed in CMonster::Init()\n");
        }
    }

    // Collider ������Ʈ Ȯ�� �� �߰�
    if (!m_pCollider) {
        m_pCollider = this->AddComponent<CCollider>();
        if (m_pCollider) {
            m_pCollider->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
            m_pCollider->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));
            OutputDebugStringA("INFO: Collider successfully added to Monster\n");
        }
        else {
            OutputDebugStringA("ERROR: Collider creation failed in CMonster::Init()\n");
        }
    }

    // Gravity ������Ʈ Ȯ�� �� �߰� (�ߺ� ����)
    CGravity* existingGravity = GetComponent<CGravity>();
    if (!existingGravity) {
        CGravity* gravity = AddComponent<CGravity>();
        if (gravity) {
            char buffer[256];
            sprintf_s(buffer, "[MONSTER INIT] Gravity �ּ�: %p, Monster �ּ�: %p\n", gravity, this);
            OutputDebugStringA(buffer);

            gravity->SetMonster(this);
            OutputDebugStringA("[MONSTER INIT] SetMonster ȣ���\n");
        }
        else {
            OutputDebugStringA("ERROR: Gravity creation failed in CMonster::Init()\n");
        }
    }
    else {
        char buffer[256];
        sprintf_s(buffer, "[MONSTER INIT] Gravity �̹� ������: %p, Monster �ּ�: %p\n", existingGravity, this);
        OutputDebugStringA(buffer);
    }

    // MonsterScript ������Ʈ Ȯ�� �� �߰�
    m_pMonsterScript = this->GetComponent<CMonsterScript>();
    if (!m_pMonsterScript)
    {
        m_pMonsterScript = this->AddComponent<CMonsterScript>();
        if (!m_pMonsterScript)
        {
            OutputDebugStringA("ERROR: MonsterScript creation failed in CMonster::Init()\n");
        }
    }
    m_pMonsterScript->SetMonOwner(this); // ������ ���� (���� ����)
}

void CMonster::Update()
{
    //OutputDebugStringA("CMonster::Update() ȣ���\n");
	CGameObject::Update();
}

void CMonster::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CMonster::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

bool CMonster::CheckGround(Vector3 _fPlusCheckPos)
{
    if (IsGroundCheck == false) return false;

    float CurYPos = m_pTransform->GetWorldPosition().y;

    if (SkipGround != 0.0f && SkipGround <= CurYPos) return false;
    else if (SkipGround != 0.0f && SkipGround > CurYPos) SkipGround = 0.0f;

    set<float>::iterator StartIter = NotGround.begin();
    set<float>::iterator EndIter = NotGround.end();

    for (float CheckPos : NotGround)
    {
        if (CheckPos != 0.0f && (CheckPos + 1.0f >= CurYPos && CheckPos - 2.0f <= CurYPos))
        {
            return false;
        }
    }

    // ���� ���� ����� �����ͼ� �Ѱ���
    CScene* pCurrentScene = CSceneManager::GetCurScene();
    if (!pCurrentScene)
    {
        OutputDebugStringA("ERROR: Current Scene is NULL!\n");
        return false;
    }

    CBackGround* pCurBackGround = pCurrentScene->GetBackGround();
    if (!pCurBackGround)
    {
        OutputDebugStringA("WARNING: BackGround is not set in current scene!\n");
        return false;
    }

    // ���� y ��ǥ�� �ùٸ��� �����ϰ� �ִ��� Ȯ��
    Vector3 checkPos = m_pTransform->GetWorldPosition() + _fPlusCheckPos;

    CGravity* gravityComponent = this->GetComponent<CGravity>();
    if (!gravityComponent)
    {
        OutputDebugStringA("ERROR: CGravity component not found in Monster!\n");
        return false;
    }

    Vector3 bottomPivot = _fPlusCheckPos + Vector3(0.0f, -54.0f, 0.0f);

    return pCurBackGround->CheckGround(bottomPivot);
}

void CMonster::Start(wstring _MonsterName)
{
}

void CMonster::Respawn()
{
    m_fCurHP = static_cast<float>(m_iMaxHP);;
    SetActive(true);
    SetIsDead(false);
    SetIsUpdate(true);
    SetCurState(MON_STATE::MS_Idle);
    //m_strCurAniName.clear();

    char buffer[128];
    sprintf_s(buffer, "[Respawn] HP �ʱ�ȭ: %.1f / %d\n", m_fCurHP, m_iMaxHP);
    OutputDebugStringA(buffer);
}

void CMonster::ReduceHP(int _iDamage)
{
    m_fCurHP -= _iDamage;
    if (m_fCurHP <= 0 && m_eMonState != MON_STATE::MS_Dead) {
        m_eMonState = MON_STATE::MS_Dead;
    }
}

void CMonster::DropItem()
{
    CScene* pScene = CSceneManager::GetCurScene();
    if (!pScene)
    {
        OutputDebugStringA("Failed to drop item: Scene not set.\n");
        return;
    }

    // ��ġ ��������
    CTransform* tr = this->GetComponent<CTransform>(); // GetOwner()�� �ʿ� ����, �ڽ��� �ǹ���
    Vector3 vPos = tr ? tr->GetWorldPosition() : Vector3(0.f, 0.f, 0.f);

    // 50% Ȯ���� ������ ���
    int itemDropChance = std::rand() % 100;
    if (itemDropChance < 100 && !m_strDropItem.empty())
    {
        m_pItem = Instantiate<CItem>(LAYER_TYPE::LT_Item);
        m_pItem->SetDropItem(m_strDropItem);

        // ��ġ ����
        CTransform* itemTr = m_pItem->GetComponent<CTransform>();
        if (itemTr)
            itemTr->SetWorldPosition(vPos);

        OutputDebugStringA(("Item dropped: " + std::string(m_strDropItem.begin(), m_strDropItem.end()) + "\n").c_str());
    }

    //    // 100% Ȯ���� �� ���
    //    CMeso* pMoney = new CMeso();
    //    Vector3 vMoneyOffset = Vector3(20.f, 0.f, 0.0f); // ���� ������ ���� (���������� 20����)
    //    Vector3 vMoneyPos = vPos + vMoneyOffset;
    //    pMoney->SetPos(vMoneyPos); // ������ ��ġ���� �������� ���Ͽ� ����
    //    pMoney->SetMoneyAmount(100);
    //    pScene->AddObject(pMoney, OBJECT_TYPE::MESO);
    //    OutputDebugStringA("Money dropped!\n");
    //}
}

//void CMonster::OnDeath()
//{
//    // ���¸� DEAD�� ����
//    m_eCurMonState = MON_STATE::DEAD;
//
//    GetAnimator()->Play(m_iDir == -1 ? L"DeadLeft" : L"DeadRight", false); // �״� �ִϸ��̼� ��� (�ݺ� ����)
//    m_bDeadAniFin = false; // �״� �ִϸ��̼� �Ϸ� �÷��� �ʱ�ȭ
//}

//CCollider* monsterCollision = this->AddComponent<CCollider>();
	//
	//Vector3 monsterScale = monsterTransform->GetScale();
	//Vector3 halfSize = monsterScale * 0.5f;
	//halfSize.z = monsterScale.z;

	//// �浹 �ڽ��� ��� ����
	//monsterCollision->m_vBottomLeft = Vector3(-halfSize.x, -halfSize.y, -1.0f);
	//monsterCollision->m_vTopRight = Vector3(halfSize.x, halfSize.y, -1.0f);

//void CMonster::Init()
//{
//    OutputDebugStringA("CMonster::Init() ȣ���\n");
//
//    CGameObject::Init();
//
//    // CTransform�� ���� �߰�
//    //CTransform* monsterTransform = GetTransform();
//    m_pTransform = this->AddComponent<CTransform>();
//
//    if (m_pCollider == nullptr)
//    {
//        m_pCollider = this->AddComponent<CCollider>();
//        m_pCollider->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
//        m_pCollider->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));
//
//        if (!m_pCollider)
//        {
//            OutputDebugStringA("ERROR: Collider creation failed in CPlayer::Init()\n");
//        }
//        else
//        {
//            OutputDebugStringA("INFO: Collider successfully added to Player\n");
//        }
//    }
//
//    if (GetComponent<CGravity>() == nullptr)
//    {
//        CGravity* gravity = AddComponent<CGravity>();
//
//        char buffer[256];
//        sprintf_s(buffer, "[MONSTER INIT] Gravity �ּ�: %p\n", gravity);
//        OutputDebugStringA(buffer);
//
//        gravity->SetMonster(this);
//        OutputDebugStringA("[MONSTER INIT] SetMonster ȣ���\n");
//    }
//
//    //CGravity* gravity = this->AddComponent<CGravity>();
//    ////OutputDebugStringA(typeid(*this).name()); // Ȯ��
//    ////OutputDebugStringA("\n"); // Ȯ��
//    ////OutputDebugStringA(typeid(*gravity->GetOwner()).name()); // �̰� CMonster���� ��
//    ////OutputDebugStringA("\n"); // Ȯ��
//    //if (gravity) gravity->SetMonster(this);
//
//    CMonsterScript* monSr = this->AddComponent<CMonsterScript>();
//}