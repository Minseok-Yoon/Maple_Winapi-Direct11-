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

    int m_iFrameX;        // 가로 프레임 개수
    int m_iFrameY;        // 세로 프레임 개수
    int m_iCurrentFrame;  // 현재 프레임
    float m_fFrameTime;   // 프레임 전환 시간
    float m_fFrameAcc;    // 프레임 타이머 누적

    float m_fLifeTime;    // 전체 생존 시간
    float m_fAccTime;     // 누적 시간

    bool m_bLoop;
    bool m_bDead;

    Vector2 m_vFrameSize; // 한 프레임 크기
};