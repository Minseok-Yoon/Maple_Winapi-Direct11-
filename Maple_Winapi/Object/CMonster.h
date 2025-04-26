#pragma once
#include "../Object/CGameObject.h"

struct tMonInfo
{
    string  strTag;     // �̸�
    float   fSpeed;     // �ӵ�
    float   fHP;        // ü��
    float   fRecoRange; // ���� ����
    float   fAttRange;  // ���� ����
    float   fAtt;       // ���ݷ�
};

class CMonster : public CGameObject
{
public:
    CMonster();
    ~CMonster();

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render(const Matrix& view, const Matrix& projection) override;

private:
    class CCollider* m_pCollider;
};