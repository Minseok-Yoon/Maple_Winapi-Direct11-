#pragma once
#include "../Object/CGameObject.h"

struct tMonInfo
{
    string  strTag;     // 이름
    float   fSpeed;     // 속도
    float   fHP;        // 체력
    float   fRecoRange; // 인지 범위
    float   fAttRange;  // 공격 범위
    float   fAtt;       // 공격력
};

class CMonster : public CGameObject
{
public:
    CMonster();
    ~CMonster();

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;
};