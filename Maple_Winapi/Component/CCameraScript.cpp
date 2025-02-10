#include "CCameraScript.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"
#include "../Component/CCamera.h"
#include "../Scene/CScene.h"
#include "../Resource/CTexture.h"
#include "../Core/CCore.h"

extern CCore core;

CCameraScript::CCameraScript() :
	m_vLookPosition(Vector2(0.0f, 0.0f))
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::Init()
{
}

void CCameraScript::Update()
{
    CTransform* tr = GetOwner()->GetComponent<CTransform>();
    math::Vector3 vPos = tr->GetPosition();

    if (KEY_HOLD(KEY_CODE::A))
        vPos += 200.0f * -tr->Right() * CTimeManager::GetfDeltaTime();
    if (KEY_HOLD(KEY_CODE::W))
        vPos += 200.0f * tr->Forward() * CTimeManager::GetfDeltaTime();
    if (KEY_HOLD(KEY_CODE::D))
        vPos += 200.0f * tr->Right() * CTimeManager::GetfDeltaTime();
    if (KEY_HOLD(KEY_CODE::S))
        vPos += 200.0f * -tr->Forward() * CTimeManager::GetfDeltaTime();
    if (KEY_HOLD(KEY_CODE::E))
        vPos += 200.0f * tr->Up() * CTimeManager::GetfDeltaTime();
    if (KEY_HOLD(KEY_CODE::Q))
        vPos += 200.0f * -tr->Up() * CTimeManager::GetfDeltaTime();

    CScene* pCurScene = CSceneManager::GetCurScene();
    if (pCurScene == nullptr)
        return;

    CBackGround* bg = pCurScene->GetBackGround();
    if (bg == nullptr)
        return;

    CTransform* bgtr = bg->GetComponent<CTransform>();
    if (bgtr == nullptr)
        return;

    // 배경 크기 가져오기
    Vector2 bgSize = bgtr->GetScale2D();
    float bgWidth = bgSize.x * 0.5f;  // 배경 너비 절반 (-bgWidth ~ bgWidth)
    float bgHeight = bgSize.y * 0.5f; // 배경 높이 절반 (-bgHeight ~ bgHeight)

    // 클라이언트(뷰포트) 크기 가져오기
    float viewportHalfWidth = core.GetWidth() * 0.5f;
    float viewportHalfHeight = core.GetHeight() * 0.5f;

    // 🔹 카메라 이동 가능 범위 설정
    float minX = -bgWidth + viewportHalfWidth;
    float maxX = bgWidth - viewportHalfWidth;
    float minY = -bgHeight + viewportHalfHeight;
    float maxY = bgHeight - viewportHalfHeight;

    // 만약 배경이 클라이언트보다 작다면 카메라를 고정
    if (bgWidth < viewportHalfWidth)
        minX = maxX = 0.0f;
    if (bgHeight < viewportHalfHeight)
        minY = maxY = 0.0f;

    // 카메라 위치 제한 적용
    vPos.x = Clamp(vPos.x, minX, maxX);
    vPos.y = Clamp(vPos.y, minY, maxY);

    // 최종 위치 적용
    tr->SetPosition(vPos);
}

void CCameraScript::LateUpdate()
{
}

void CCameraScript::Render()
{
}

float CCameraScript::Clamp(float value, float minVal, float maxVal)
{
	if (value < minVal) return minVal;
	if (value > maxVal) return maxVal;
	return value;
}