#pragma once
#include "../Object/CGameObject.h"
#include "../Object/CMonsterScript.h"

class CItem;

struct tMonInfo
{
    CMonster*   pMonster;
    string      strTag;     // 이름
    float       fSpeed;     // 속도
    float       fHP;        // 체력
    float       fRecoRange; // 인지 범위
    float       fAttRange;  // 공격 범위
    float       fAtt;       // 공격력
    wstring     strDropItme;
};

struct FieldMonsterInfo
{
    Vector3     fRespawnPos = Vector3(0.0f, 0.0f, 0.0f);
    CMonster*   pMonster;
    float       fRespawnTimer;
};

class CMonster : public CGameObject
{
public:
    CMonster();
    virtual ~CMonster();

    virtual void Init() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;

    bool CheckGround(Vector3 _fPlusCheckPos);

    void Start(wstring _MonsterName);
    void Respawn();

    void ReduceHP(int _iDamage);
    void DropItem();
    //void OnDeath();

public:
    void SetCurState(MON_STATE _eMonState) { m_eMonState = _eMonState; }
    MON_STATE GetCurState() const { return m_eMonState; }

    void SetMonInfo(const tMonInfo& _tInfo) 
    { 
        m_tMonInfo = _tInfo; 
        m_iMaxHP = _tInfo.fHP;
        m_fCurHP = _tInfo.fHP;
    }
    tMonInfo& GetMonInfo() { return m_tMonInfo; }

    void SetFieldMonInfo(const FieldMonsterInfo& _tFieldMonInfo) { m_tFieldMonInfo = _tFieldMonInfo; }
    FieldMonsterInfo& GetFieldMonInfo() { return m_tFieldMonInfo; }

    // 2025-05-30
    void SetDropItem(const wstring& _strDrop) { m_strDropItem = _strDrop; }

    // 2025-06-06
    float GetCurHp() const { return m_fCurHP; }

private:
    class CCollider* m_pCollider;
    tMonInfo m_tMonInfo;

    set<float> NotGround;
    float SkipGround = 0.0f;
    bool IsGroundCheck = true;

    // 2025-05-26
    MON_STATE			m_eMonState;
    CMonsterScript*     m_pMonsterScript = nullptr;

    // 2025-05-29
    int                 m_iMaxHP;
    FieldMonsterInfo    m_tFieldMonInfo;
    float               m_fCurHP;

    // 2025-05-30
    wstring             m_strDropItem;
    CItem*              m_pItem;
};