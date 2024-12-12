#pragma once
#include "CGameObject.h"

class CDamageManager;

class CDamage : public CGameObject
{
public:
    CDamage();
    virtual ~CDamage();

public:
    vector<class CTexture*> m_vecDamage; // ������ �ؽ���
    vector<Vector2> m_vecPos; // ��ġ
    int iLength; // ������ ����
    float fDelay; // ������ ������
    bool bEnable; // �����̰� ������ Ȱ��ȭ������ �÷���
    float fExistTime; // ������ �ð�

public:
    void Init(int _iDamage, Vector2& _vPosition, bool _bIsCri, CDamageManager* _pManager, int i = 1);
    virtual void Update();
    virtual void Render();
};