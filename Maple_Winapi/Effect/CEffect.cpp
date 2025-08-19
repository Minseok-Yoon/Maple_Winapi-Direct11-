#include "CEffect.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CTimeManager.h"
#include "../Resource/CTexture.h"
#include "../Component/CRenderer.h"

CEffect::CEffect() :
    m_pTextureSRV(nullptr),
    m_iFrameX(1), m_iFrameY(1),
    m_iCurrentFrame(0),
    m_fFrameTime(0.1f), m_fFrameAcc(0.0f),
    m_fLifeTime(1.0f), m_fAccTime(0.0f),
    m_bLoop(false), m_bDead(false)
{
}

CEffect::~CEffect()
{
}

void CEffect::Init()
{
    if (!m_pTransform)
        m_pTransform = AddComponent<CTransform>();
}

void CEffect::Update()
{
    if (m_bDead) return;

    float deltaTime = CTimeManager::GetfDeltaTime();
    m_fAccTime += deltaTime;

    // 생존 시간 체크
    if (m_fAccTime >= m_fLifeTime)
    {
        SetActive(false);
        m_bDead = true;
        return;
    }

    // 프레임 업데이트
    m_fFrameAcc += deltaTime;
    if (m_fFrameAcc >= m_fFrameTime)
    {
        m_fFrameAcc = 0.0f;
        m_iCurrentFrame++;

        int totalFrames = m_iFrameX * m_iFrameY;
        if (m_iCurrentFrame >= totalFrames)
        {
            if (m_bLoop)
                m_iCurrentFrame = 0;
            else
                m_iCurrentFrame = totalFrames - 1;
        }
    }
}

void CEffect::Render(const Matrix& view, const Matrix& projection)
{
    //if (!m_pTextureSRV || m_bDead) return;

    //// 프레임 UV 계산
    //int curX = m_iCurrentFrame % m_iFrameX;
    //int curY = m_iCurrentFrame / m_iFrameX;

    //Vector2 uvStart(curX * m_vFrameSize.x, curY * m_vFrameSize.y);
    //Vector2 uvEnd(uvStart.x + m_vFrameSize.x, uvStart.y + m_vFrameSize.y);

    //// 스프라이트 렌더링 (CRenderer 사용 가정)
    //CRenderer::DrawSprite(m_pTextureSRV, m_pTransform->GetWorldMatrix(), view, projection, uvStart, uvEnd);
}

void CEffect::SetEffect(const wstring& textureName, int frameX, int frameY, float frameTime, float lifeTime)
{
    /*m_strEffectName = textureName;
    m_pTextureSRV = CResourceManager::Load<CTexture>(textureName, L"../Resources/Effect/" + textureName + L".png")->GetSRV();

    m_iFrameX = frameX;
    m_iFrameY = frameY;
    m_fFrameTime = frameTime;
    m_fLifeTime = lifeTime;

    m_vFrameSize.x = 1.0f / frameX;
    m_vFrameSize.y = 1.0f / frameY;*/
}
