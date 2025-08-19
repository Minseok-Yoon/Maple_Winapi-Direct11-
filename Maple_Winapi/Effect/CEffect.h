#pragma once
#include "../Object/CGameObject.h"

class CEffect : public CGameObject
{
public:
    CEffect();
    virtual ~CEffect();

    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(const Matrix& view, const Matrix& projection) override;

    void SetEffect(const wstring& textureName, int frameX, int frameY, float frameTime, float lifeTime);
    void SetLoop(bool loop) { m_bLoop = loop; }

private:
    ID3D11ShaderResourceView* m_pTextureSRV;
    wstring m_strEffectName;

    int m_iFrameX;        // ���� ������ ����
    int m_iFrameY;        // ���� ������ ����
    int m_iCurrentFrame;  // ���� ������
    float m_fFrameTime;   // ������ ��ȯ �ð�
    float m_fFrameAcc;    // ������ Ÿ�̸� ����

    float m_fLifeTime;    // ��ü ���� �ð�
    float m_fAccTime;     // ���� �ð�

    bool m_bLoop;
    bool m_bDead;

    Vector2 m_vFrameSize; // �� ������ ũ��
};