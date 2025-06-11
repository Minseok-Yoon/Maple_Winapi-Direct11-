#include "CGravity.h"
#include "../Manager/CTimeManager.h"
#include "../Object/CGameObject.h"
#include "../Component/CRigidBody.h"
#include "../Component/CTransform.h"
#include "../Resource/CTexture.h"
#include "../Manager/CSceneManager.h"
#include "../Object/CBackGround.h"
#include "../Object/CPlayer.h"

CGravity::CGravity() :
    m_bPlayerGround(true),
    m_bMonsterGround(true),
    m_bIsGravity(false),
    m_fGravityForce(0.0f),
    m_fGravitySpeed(15000.0f),
    m_vecMoveForce(Vector2(0.0f, 0.0f)),
    m_fMaxGravity(10.0f),
    m_vGravity(Vector2(0.0f, -800.0f)),
    m_pTransform(nullptr)
{
}

CGravity::~CGravity()
{
}

void CGravity::Init()
{
    CRigidBody::Init();

    m_pTransform = GetOwner()->GetComponent<CTransform>();

    //// 강제 확인
    //std::wstring name = GetOwner()->GetName();
    //OutputDebugStringW((L"Gravity Init Owner: " + name + L"\n").c_str());
    //OutputDebugStringA(typeid(*GetOwner()).name());  // RTTI 타입 이름 출력
}

void CGravity::Update()
{
    CRigidBody::Update();

    CScene* pCurScene = CSceneManager::GetCurScene();
    if (!pCurScene) return;

    CBackGround* pBackGround = pCurScene->GetBackGround();
    if (!pBackGround) return;

    // 플레이어는 항상 검사
    if (m_pPlayer)
    {
        m_bPlayerGround = m_pPlayer->CheckGround(m_pTransform->GetWorldPosition());
    }

    // 몬스터가 존재할 경우에만 검사
    if (m_pMonster)
    {
        m_bMonsterGround = m_pMonster->CheckGround(m_pTransform->GetWorldPosition());
    }

    /*std::ostringstream oss;
    oss << "[CGravity::Update] Owner: " << typeid(*GetOwner()).name() << " | 주소: " << GetOwner();
    OutputDebugStringA(oss.str().c_str());*/

    Gravity();
}

void CGravity::LateUpdate()
{
    CRigidBody::LateUpdate();
}

void CGravity::Render(const Matrix& view, const Matrix& projection)
{
    CRigidBody::Render(view, projection);
}

void CGravity::Jump(float _fJumpForce)
{
    if (!m_bPlayerGround) return; // 땅에 붙어 있지 않으면 점프 불가

    m_bPlayerGround = false;

    // Owner 객체의 Transform, RigidBody 컴포넌트를 가져온다.
    CTransform* tr = GetOwner()->GetComponent<CTransform>();
    CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();

    // RigidBody가 있는 경우에만 점프 속도 설정
    if (rb)
    {
        rb->SetVelocity(Vector2(rb->GetVelocity().x, -_fJumpForce));
    }
}

void CGravity::Gravity()
{
    bool isGrounded = false;

    if (m_pPlayer && m_bPlayerGround)
        isGrounded = true;

    if (m_pMonster && m_bMonsterGround)
        isGrounded = true;

    // 땅에 닿아 있는 경우 수직 속도를 0으로 설정하여 중력 영향을 받지 않게 한다.
    if (isGrounded)
    {
        // 땅에 있을 경우 수직 속도를 0으로 설정
        m_vVelocity.y = 0.0f;
    }
    else
    {
        // 공중에 있을 경우 중력 적용
        m_vVelocity += m_vGravity * CTimeManager::GetfDeltaTime();
    }

    // 최대 속도 제한
    Vector2 gravity = m_vGravity;
    gravity.Normalize();
    float dot = m_vVelocity.Dot(gravity);
    gravity = gravity * dot;
    Vector2 sideVelocity = m_vVelocity - gravity;

    if (m_vMaxVelocity.y < gravity.Length())
    {
        gravity.Normalize();
        gravity *= m_vMaxVelocity.y;
    }

    if (m_vMaxVelocity.x < sideVelocity.Length())
    {
        sideVelocity.Normalize();
        sideVelocity *= m_vMaxVelocity.x;
    }

    m_vVelocity = sideVelocity + gravity;
}
/*if (m_bIsGravity == false) { return; }

    m_fGravityForce += m_fGravitySpeed * CTimeManager::GetfDeltaTime();
    if (m_fGravityForce >= 1400.0f)
    {
        m_fGravityForce = 1400.0f;
    }

    m_vecMoveForce.y -= m_fGravityForce * CTimeManager::GetfDeltaTime();
    float MoveVectorForceDelta = m_vecMoveForce.y * CTimeManager::GetfDeltaTime();
    if (-m_fMaxGravity >= MoveVectorForceDelta)
    {
        MoveVectorForceDelta = -m_fMaxGravity;
    }

    if (m_vecMoveForce.y < 0.0f  && MoveVectorForceDelta  < -1.0f)
    {
        float Count = 0.0f;
        for (; ; Count -= 1.0f)
        {
            if (Count <= MoveVectorForceDelta)
            {
                Count = MoveVectorForceDelta;
                break;
            }

            if (true == CheckGround(Vector3(0.0f, Count, 0.0f)))
            {
                break;
            }
        }
        m_pTransform->AddLocalPosition(Vector3(0.0f, Count, 0.0f));
    }
    else
    {
        m_pTransform->AddLocalPosition(Vector3(0.0f, MoveVectorForceDelta, 0.0f));
    }*/

    //CScene* pCurrentScene = CSceneManager::GetCurScene();
        //if (!pCurrentScene)
        //{
        //    OutputDebugStringA("ERROR: Current Scene is NULL!\n");
        //    return;
        //}
        //CBackGround* pCurBackGround = pCurrentScene->GetBackGround();
        //if (!pCurBackGround)
        //{
        //    OutputDebugStringA("WARNING: BackGround is not set in current scene!\n");
        //    return;
        //}

        //// ✅ AABB + 픽셀 충돌 검사
        //m_bGround = CheckGround(pCurBackGround, Vector3(0.0f, -1.0f, 0.0f));

        //// ✅ 충돌 상태에 따라 중력 처리
        //if (m_bGround)
        //{
        //    m_vVelocity.y = 0.0f; // 충돌 시 중력 제거
        //}
        //else
        //{
        //    m_vVelocity.y -= m_fGravityForce * CTimeManager::GetfDeltaTime(); // 중력 적용
        //}

        //SetVelocity(Vector2(m_vVelocity.x, m_vVelocity.y));

//bool CGravity::CheckGround(CBackGround* pGround, Vector3 _fPlusCheckPos)
//{
//    /*if (!m_pTransform)
//    {
//        OutputDebugStringA("ERROR: CGravity::CheckGround() - Transform is NULL!\n");
//        return false;
//    }*/
//
//    if (this == nullptr) // ✅ `this`가 유효한지 먼저 체크
//    {
//        OutputDebugStringA("ERROR: CGravity::CheckGround() - this is NULL!\n");
//        return false;
//    }
//
//    // ✅ 충돌 박스 가져오기
//    CCollider* collider = m_pOwner->GetComponent<CCollider>();
//    if (!collider)
//    {
//        OutputDebugStringA("WARNING: Collider not found in Player!\n");
//        return false;
//    }
//
//    // ✅ AABB 충돌 범위 가져오기
//    Vector3 bottomLeft = collider->m_vBottomLeft + _fPlusCheckPos;
//    Vector3 topRight = collider->m_vTopRight + _fPlusCheckPos;
//
//    // ✅ AABB 박스 영역 내 픽셀 충돌 검사
//    for (int x = static_cast<int>(bottomLeft.x); x <= static_cast<int>(topRight.x); x++)
//    {
//        for (int y = static_cast<int>(topRight.y); y <= static_cast<int>(bottomLeft.y); y++)
//        {
//            TextureColor CheckColor = pGround->GetColor(
//                float4(static_cast<float>(x), static_cast<float>(y), 0.0f, 1.0f),
//                TextureColor(0.0f, 0.0f, 0.0f, 255.0f));
//
//            // ✅ 충돌 픽셀 색상 확인 (예: R=255, G=0, B=255는 땅)
//            if (CheckColor.R == 255 && CheckColor.G == 0 && CheckColor.B == 255)
//            {
//                return true; // 땅에 닿음
//            }
//        }
//    }
//
//    return false; // 땅이 아님
//}

//void CGravity::Update()
//{
//    CRigidBody::Update();
//
//    // 현재 씬에서 배경 가져오기
//    CScene* pCurScene = CSceneManager::GetCurScene();
//    if (!pCurScene) return;
//
//    CBackGround* pBackGround = pCurScene->GetBackGround();
//    if (!pBackGround) return;
//
//    // 플레이어의 위치에서 충돌 검사
//    m_bPlayerGround = m_pPlayer->CheckGround(m_pTransform->GetWorldPosition());
//    //m_bMonsterGround = m_pMonster->CheckGround(m_pTransform->GetWorldPosition());
//
//    Gravity();
//}