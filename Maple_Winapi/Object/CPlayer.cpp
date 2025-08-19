#include "CPlayer.h"
#include "../Component/CCollider.h"
#include "../Component/CBoxCollider2D.h"
#include "../Object/CGameObject.h"
#include "../Component/CComponent.h"
#include "../Component/CTransform.h"
#include "../Component/CRigidBody.h"
#include "../Component/CGravity.h"
#include "../Resource/CTexture.h"
#include "../Object/CPlayerScript.h"
#include "../Object/CPlayerBehavious.h"
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CUIManager.h"
#include "../Component/CSpriteRenderer.h"
#include "../Scene/CScene.h"
#include "../Quest/CQuest.h"
#include "..\Test\CDebugDraw.h"
#include "../Component/CPixelCollider.h"
#include "CObject.h"
#include "Trinity.h"
#include "../Manager/CSkillManager.h"

CPlayer::CPlayer() :
	m_fAttackDelayTime(1.2f),  // 공격 후 0.2초 동안 대기
	m_fElapsedTime(0.0f),
	m_bAttackCycle(false)
{
	SetName(L"Player");
    CTexture* pHpBar = CResourceManager::Load<CTexture>(L"Player_HpBar", L"../Resources/Texture/NPC/NpcMent_BG.png");
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
    CGameObject::Init();

    m_pTransform = this->AddComponent<CTransform>();
    m_pTransform->SetLocalPosition(Vector3(-100.0f, 0.0f, -1.0f));
    m_pTransform->SetLocalScale(Vector3(54.0f, 65.0f, 0.0f));

    if (m_pCollider == nullptr)
    {
        m_pCollider = this->AddComponent<CCollider>();
        m_pCollider->SetBottomLeft(Vector3(-0.5f, -0.5f, -1.0f));
        m_pCollider->SetTopRight(Vector3(0.5f, 0.5f, -1.0f));
    }

    //CGravity* gravity = this->AddComponent<CGravity>();
    //if (gravity) gravity->SetPlayer(this);

    CPlayerScript* pScript = this->AddComponent<CPlayerScript>(); 
}

void CPlayer::Update()
{
    CGameObject::Update();

    Vector3 pos = m_pTransform->GetWorldPosition();
    Vector3 prevPos = pos; // 이전 위치 저장

    CPixelCollider* pixelCollider = GetComponent<CPixelCollider>();
    if (pixelCollider)
    {
        m_bOnGround = pixelCollider->CheckGround(pos);

        if (m_bOnGround)
        {
            float groundY = pixelCollider->FindGroundY(pos, prevPos);
            float desiredOffset = 1.0f;
            float currentOffset = pos.y - groundY;

            if (fabsf(currentOffset - desiredOffset) > 0.1f)
            {
                pos.y = groundY + desiredOffset;
                m_pTransform->SetWorldPosition(pos);
            }
        }
    }
}

void CPlayer::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CPlayer::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

//bool CPlayer::CheckGround(Vector3 _fPlusCheckPos)
//{
//    CScene* pScene = CSceneManager::GetCurScene();
//    if (!pScene) return false;
//
//    CBackGround* pBg = pScene->GetBackGround();
//    if (!pBg) return false;
//
//    float checkWidth = 6.0f;
//    float checkDepth = 3.0f;
//
//    // 플레이어의 발 위치로 보정
//    Vector3 footPos = _fPlusCheckPos;
//    footPos.y -= m_pTransform->GetLocalScale().y * 0.5f; // 스케일의 절반만큼 아래로
//
//    // 이후 footPos를 기준으로 충돌 체크
//    for (float dx = -checkWidth; dx <= checkWidth; dx += 2.0f)
//    {
//        for (float dy = -checkDepth; dy <= 1.0f; dy += 1.0f)
//        {
//            Vector3 checkPos(footPos.x + dx, footPos.y + dy, 0.0f);
//            if (pBg->CheckGround(checkPos))
//            {
//                return true;
//            }
//        }
//    }
//
//    return false;
//}
//
//float CPlayer::FindGroundForMovement(Vector3 currentPos, Vector3 prevPos)
//{
//    CScene* pScene = CSceneManager::GetCurScene();
//    if (!pScene) 
//        return currentPos.y;
//
//    CBackGround* pBG = pScene->GetBackGround();
//    if (!pBG) 
//        return currentPos.y;
//
//    float finalGroundY = currentPos.y;
//
//    // 이전 이동 거리 계산
//    Vector3 moveDir = currentPos - prevPos;
//    float moveDistance = moveDir.Length();
//
//    const float searchRange = 10.0f;
//    const float step = 1.0f;
//
//    // 찾은 지면 정보
//    float bestY = currentPos.y;
//    float minDistance = FLT_MAX;
//    bool bFound = false;
//
//    // 위에서부터 아래로 스캔 (우선 위쪽 우선으로 → 경사면 올라타기 안정화)
//    for (float dy = searchRange; dy >= -searchRange; dy -= step)
//    {
//        Vector3 checkPos(currentPos.x, currentPos.y + dy, 0.0f);
//        if (pBG->CheckGround(checkPos))
//        {
//            float dist = fabsf(dy);
//            if (dist < minDistance)
//            {
//                minDistance = dist;
//                bestY = checkPos.y;
//                bFound = true;
//
//                // 아주 가까운 거리는 바로 탈출
//                if (dist < 0.5f)
//                    break;
//            }
//        }
//    }
//
//    if (bFound)
//    {
//        finalGroundY = bestY;
//
//        // 부드럽게 이동: 이전 y와의 차가 크면 중간값으로 이동 (lerp)
//        float currentY = currentPos.y;
//        float targetY = bestY;
//
//        float diffY = fabsf(currentY - targetY);
//        if (diffY > 1.0f)
//        {
//            finalGroundY = (currentY + targetY) * 0.5f;  // 스무딩
//        }
//
//        // 픽셀 스냅: float 오차로 인한 흔들림 방지
//        finalGroundY = roundf(finalGroundY);
//    }
//
//    return finalGroundY;
//}

//float CPlayer::FindClosestGroundY(Vector3 _pos)
//{
//    CScene* pScene = CSceneManager::GetCurScene();
//    if (!pScene) return _pos.y;
//
//    CBackGround* pBg = pScene->GetBackGround();
//    if (!pBg) return _pos.y;
//
//    float maxSearchDist = 15.0f;
//    float step = 1.0f;
//
//    // 좌측 발 / 우측 발 기준 위치
//    float footOffset = m_pTransform->GetLocalScale().x * 0.5f * 0.8f; // 약간 안쪽
//
//    float leftY = _pos.y;
//    float rightY = _pos.y;
//
//    bool bLeftFound = false;
//    bool bRightFound = false;
//
//    // 먼저 위쪽 검색 (경사면 올라가기)
//    for (float dy = 0.0f; dy <= maxSearchDist; dy += step)
//    {
//        Vector3 checkPosUp(_pos.x, _pos.y + dy, 0.0f);
//        if (pBg->CheckGround(checkPosUp))
//        {
//            return checkPosUp.y;
//        }
//    }
//
//    // 위쪽에서 못 찾으면 아래쪽 검색
//    for (float dy = 0.0f; dy <= maxSearchDist; dy += step)
//    {
//        Vector3 checkPosDown(_pos.x, _pos.y - dy, 0.0f);
//        if (pBg->CheckGround(checkPosDown))
//        {
//            return checkPosDown.y;
//        }
//    }
//
//    return _pos.y;
//}

void CPlayer::OnMonsterKilled(const wstring& _monsterName)
{
    wchar_t buf[256];
    swprintf_s(buf, 256, L"[DEBUG] Player 퀘스트 수: %d\n", (int)m_vecQuests.size());
    OutputDebugStringW(buf);

    swprintf_s(buf, 256, L"[Player::OnMonsterKilled] this: %p\n", this);
    OutputDebugStringW(buf);

    for (CQuest* quest : m_vecQuests)
    {
        if (!quest->IsQuestCompleted())
        {
            int before = quest->GetCurrentCount();
            quest->TryProgress(_monsterName);
            if (quest->IsQuestCompleted())
            {
                CUIManager::PushWithQuest(UI_TYPE::UT_MentBox, L"퀘스트 완료! - " + quest->GetTitle(), quest);
            }
            else if (before != quest->GetCurrentCount())
            {
                wchar_t szLog[256];
                swprintf_s(szLog, 256, L"진행도: %d / %d\n", quest->GetCurrentCount(), quest->GetGoalCount());
                OutputDebugStringW(szLog);
            }
        }
    }
}

/*CPlayer::~CPlayer()
{
    // 플레이어가 삭제 될 일은 없지만 플레이어가 없는 씬은 있으니
    // 플레이어가 존재하지 않는 다면 충돌 박스를 삭제하도록 구현
    delete m_pCollider;
}

void CPlayer::Init()
{
    /*CGameObject::Init();

    SetName(L"Player");

    // CTransform을 먼저 추가
    CTransform* playerTransform = this->AddComponent<CTransform>();
    playerTransform->SetScale(Vector3(54.0f, 65.0f, 1.0f));
    playerTransform->SetPosition(Vector3(100.0f, 100.0f, 0.0f));

    // SpriteRenderer 설정
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");
    CSpriteRenderer* sr = this->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);

    // 충돌 박스 설정 (객체 중심 기준)
    CCollider* playerCollision = this->AddComponent<CCollider>();

    // 충돌 박스 크기를 플레이어 스케일에 맞게 설정
    Vector3 playerScale = playerTransform->GetScale();
    Vector3 halfSize = playerScale * 0.5f;
    halfSize.z = playerScale.z;  // Z 값은 변하지 않도록 유지

    // 충돌 박스의 경계 지정 (로컬 좌표 기준)
    playerCollision->m_vBottomLeft = Vector3(-halfSize.x, -halfSize.y, -1.0f);
    playerCollision->m_vTopRight = Vector3(halfSize.x, halfSize.y, -1.0f);
    CGameObject::Init();
    SetName(L"Player");

    // CTransform을 먼저 추가
    CTransform* playerTransform = this->AddComponent<CTransform>();
    playerTransform->SetScale(Vector3(54.0f, 65.0f, 1.0f));
    playerTransform->SetPosition(Vector3(100.0f, 100.0f, 0.0f));

    // SpriteRenderer 설정
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");
    CSpriteRenderer* sr = this->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);

    // 충돌 박스 설정 (객체 중심 기준)
    CCollider* playerCollision = this->AddComponent<CCollider>();

    // 충돌 박스 크기를 플레이어 스케일에 맞게 설정
    Vector3 playerScale = playerTransform->GetScale();
    Vector3 halfSize = playerScale * 0.5f;
    halfSize.z = playerScale.z;  // Z 값은 변하지 않도록 유지

    // 충돌 박스의 경계 지정 (로컬 좌표 기준)
    playerCollision->m_vBottomLeft = Vector3(-halfSize.x, -halfSize.y, 1.0f);
    playerCollision->m_vTopRight = Vector3(halfSize.x, halfSize.y, -1.0f);

    this->AddComponent<CRigidBody>();
}*/

/*bool CPlayer::CheckGround(Vector3 _fPlusCheckPos)
{
    /*CGravity* gravity = GetComponent<CGravity>();
    if (!gravity)
    {
        OutputDebugStringA("ERROR: CPlayer::CheckGround() - CGravity component is missing!\n");
        return false;
    }

    CBackGround* pGround = CSceneManager::GetCurScene()->GetBackGround();
    return gravity->CheckGround(pGround, _fPlusCheckPos);
    CGravity* gravity = GetComponent<CGravity>();
    if (!gravity)
    {
        OutputDebugStringA("ERROR: CPlayer::CheckGround() - CGravity component is missing!\n");
        return false;
    }

    CBackGround* pGround = CSceneManager::GetCurScene()->GetBackGround();
    if (!pGround)
    {
        OutputDebugStringA("ERROR: CPlayer::CheckGround() - No background found!\n");
        return false;
    }

    // 플레이어의 충돌 박스 가져오기
    CCollider* playerCollision = GetComponent<CCollider>();
    if (!playerCollision)
    {
        OutputDebugStringA("ERROR: CPlayer::CheckGround() - No collider found!\n");
        return false;
    }

    // 충돌 박스의 위치 가져오기
    Vector3 playerPos = GetTransform()->GetWorldPosition() + _fPlusCheckPos;
    Vector3 bottomLeft = playerPos + playerCollision->m_vBottomLeft;
    Vector3 topRight = playerPos + playerCollision->m_vTopRight;

    // 충돌 박스의 각 좌표를 픽셀 좌표로 변환
    TextureColor defaultColor = { 0, 0, 0, 0 };

    // 바닥 부분의 픽셀 색상 확인
    for (float x = bottomLeft.x; x <= topRight.x; x += 1.0f)
    {
        TextureColor pixelColor = pGround->GetColor(Vector3(x, bottomLeft.y, 0.0f), defaultColor);
        if (pixelColor.R != 0 || pixelColor.G != 0 || pixelColor.B != 0) // 검은색이 아닐 경우 충돌
        {
            return true;
        }
    }

    return false;
}*/

// 2025-03-31 
/*void CPlayer::Init()
{
    CGameObject::Init();
    SetName(L"Player");

    // CTransform을 먼저 추가
    // AddComponent나 GetComponent를 하면 Init을 빠져나갔을 때 0으로 초기화 될 수 있음.
    CTransform* playerTransform = GetTransform();

    if (!playerTransform) {
        OutputDebugStringA("Error: Failed to add CTransform to Player!\n");
        return;
    }
    this->AddComponent<CGravity>();

    CPlayerScript* plsr = this->AddComponent<CPlayerScript>();

    // SpriteRenderer 설정
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");
    CTexture::TextureSize textureSize = playerTexture->GetTextureSize();
    float textureWidth = static_cast<float>(textureSize.width);
    float textureHeight = static_cast<float>(textureSize.height);

    playerTransform->SetLocalScale(Vector3(1.0f, 1.0f, 0.0f));
    // 텍스처의 실제 크기에 맞게 월드 스케일 설정
    playerTransform->SetWorldScale(Vector3(textureWidth, textureHeight, 0.0f));

    playerTransform->SetLocalPosition(Vector3(-100.0f, 0.0f, 0.0f));

    CSpriteRenderer* sr = this->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);

    // 충돌 박스 설정 (객체 중심 기준)
    CCollider* playerCollision = this->AddComponent<CCollider>();

    // 충돌 박스 크기를 플레이어 스케일에 맞게 설정
    Vector3 playerScale = playerTransform->GetLocalScale();

    Vector3 parentScale = playerTransform->HasParent() ? playerTransform->GetParent()->GetWorldScale() : Vector3(1.0f, 1.0f, 1.0f);

    // ✅ 부모 스케일이 중복 적용되는지 확인 후 조정
    Vector3 adjustedScale = playerScale; // 기존 코드에서 parentScale을 곱한 부분 제거

    Vector3 halfSize = adjustedScale * 0.5f;
    playerCollision->m_vBottomLeft = Vector3(-2.0f, -2.0f, 1.0f);
    playerCollision->m_vTopRight = Vector3(2.0f, 2.0f, -1.0f);

    this->AddComponent<CRigidBody>();
}*/

//// SpriteRenderer 설정
    //CTexture* playerTexture = CResourceManager::Find<CTexture>(L"Player");

    //CTexture::TextureSize textureSize = playerTexture->GetTextureSize();
    //float textureWidth = static_cast<float>(textureSize.width);
    //float textureHeight = static_cast<float>(textureSize.height);

    //CSpriteRenderer* sr = this->AddComponent<CSpriteRenderer>();
    //sr->SetTexture(playerTexture);

/*bool CPlayer::CheckGround(Vector3 _fPlusCheckPos)
{
    if (IsGroundCheck == false) return false;

    float CurYPos = m_pTransform->GetWorldPosition().y;
    //OutputDebugStringA(("Current Player Y Position: " + std::to_string(CurYPos) + "\n").c_str());

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

    // ✅ 현재 씬의 배경을 가져와서 넘겨줌
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

    // ✅ 현재 y 좌표를 올바르게 전달하고 있는지 확인
    Vector3 checkPos = m_pTransform->GetWorldPosition() + _fPlusCheckPos;
    //OutputDebugStringA(("Checking color at~: (" + std::to_string(checkPos.x) + ", " + std::to_string(checkPos.y) + ")\n").c_str());

    CGravity* gravityComponent = this->GetComponent<CGravity>();
    if (!gravityComponent)
    {
        OutputDebugStringA("ERROR: CGravity component not found in Player!\n");
        return false;
    }

    Vector3 bottomPivot = _fPlusCheckPos + Vector3(0.0f, -32.5f, 0.0f);

    return pCurBackGround->CheckGround(bottomPivot);
}*/

// 2025-06-30 플레이어가 픽셀과 어떻게 충돌하는지 충돌 점을 확인하기
// 플레이어가 픽셀과 충돌 할 때 충돌 위치와 그 위치의 해당 색상의 픽셀이 같은 값인지 확인하기

// 2025-07-04
//bool CPlayer::CheckGround(Vector3 _fPlusCheckPos)
//{
//    CScene* pScene = CSceneManager::GetCurScene();
//    if (!pScene) return false;
//
//    CBackGround* pBg = pScene->GetBackGround();
//    if (!pBg) return false;
//
//    float checkWidth = 6.0f;
//    float checkDepth = 3.0f;
//
//    for (float dx = -checkWidth; dx <= checkWidth; dx += 2.0f)
//    {
//        for (float dy = -checkDepth; dy <= 1.0f; dy += 1.0f)
//        {
//            Vector3 checkPos(_fPlusCheckPos.x + dx, _fPlusCheckPos.y + dy, 0.0f);
//            if (pBg->CheckGround(checkPos))
//            {
//                return true;
//            }
//        }
//    }
//
//    return false;
//    //if (IsGroundCheck == false) return false;
//
//    //float CurYPos = m_pTransform->GetWorldPosition().y;
//    //
//    //if (SkipGround != 0.0f && SkipGround <= CurYPos) return false;
//    //else if (SkipGround != 0.0f && SkipGround > CurYPos) SkipGround = 0.0f;
//
//    //set<float>::iterator StartIter = NotGround.begin();
//    //set<float>::iterator EndIter = NotGround.end();
//
//    //for (float CheckPos : NotGround)
//    //{
//    //    if (CheckPos != 0.0f && (CheckPos + 1.0f >= CurYPos && CheckPos - 2.0f <= CurYPos))
//    //    {
//    //        return false;
//    //    }
//    //}
//
//    //// 현재 씬의 배경을 가져와서 넘겨줌
//    //CScene* pCurrentScene = CSceneManager::GetCurScene();
//    //if (!pCurrentScene)
//    //{
//    //    OutputDebugStringA("ERROR: Current Scene is NULL!\n");
//    //    return false;
//    //}
//
//    //CBackGround* pCurBackGround = pCurrentScene->GetBackGround();
//    //if (!pCurBackGround)
//    //{
//    //    OutputDebugStringA("WARNING: BackGround is not set in current scene!\n");
//    //    return false;
//    //}
//
//    //CGravity* gravityComponent = this->GetComponent<CGravity>();
//    //if (!gravityComponent)
//    //{
//    //    OutputDebugStringA("ERROR: CGravity component not found in Player!\n");
//    //    return false;
//    //}
//
//    //// 2025-07-03
//    //// 플레이어의 실제 발 위치 계산
//    //Vector3 playerScale = m_pTransform->GetLocalScale();
//    //Vector3 playerPos = m_pTransform->GetWorldPosition();
//
//    ////// 중앙 지점만 체크하되, 좌우로 약간씩 흔들어가며 체크
//    //float baseFootY = playerPos.y - (playerScale.y * 0.5f);
//
//    ////// 중앙과 좌우 약간씩 체크
//    //vector<Vector3> checkPoints = {
//    //    Vector3(playerPos.x, baseFootY, 0.0f),                    // 중앙
//    //    Vector3(playerPos.x - 5.0f, baseFootY, 0.0f),           // 좌측 5픽셀
//    //    Vector3(playerPos.x + 5.0f, baseFootY, 0.0f),           // 우측 5픽셀
//    //    Vector3(playerPos.x, baseFootY - 3.0f, 0.0f),           // 중앙 아래 3픽셀
//    //    Vector3(playerPos.x, baseFootY + 3.0f, 0.0f)            // 중앙 위 3픽셀
//    //};
//
//    //// 기존 바닥 체크 로직...
//    //for (const Vector3& checkPos : checkPoints)
//    //{
//    //    if (pCurBackGround->CheckGround(checkPos))
//    //    {
//    //        return true;
//    //    }
//    //}
//
//    //return false;
//
//    /*std::ostringstream oss;
//    oss << "CheckGround - Pos: (" << checkPos.x << ", " << checkPos.y << ")\n";
//    OutputDebugStringA(oss.str().c_str());*/
//}

// 2025-07-07 최적화 전 FindGroundForMoveMent()
/*float CPlayer::FindGroundForMovement(Vector3 currentPos, Vector3 prevPos)
{
    CScene* pScene = CSceneManager::GetCurScene();
    if (!pScene)
        return currentPos.y;

    CBackGround* pBG = pScene->GetBackGround();
    if (!pBG)
        return currentPos.y;

    // 이동 방향 계산
    Vector3 moveDir = currentPos - prevPos;
    float moveDistance = sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);

    if (moveDistance < 0.1f) // 거의 움직이지 않음
    {
        return FindClosestGroundY(currentPos);
    }

    // 이동 방향으로 지면 검색 (경사면 예측)
    float searchRange = 20.0f; // 위아래 검색 범위
    float bestY = currentPos.y;
    float minDistance = searchRange;

    for (float dy = -searchRange; dy <= searchRange; dy += 1.0f)
    {
        Vector3 checkPos(currentPos.x, currentPos.y + dy, 0.0f);
        if (pBG->CheckGround(checkPos))
        {
            float distance = abs(dy);
            if (distance < minDistance)
            {
                minDistance = distance;
                bestY = checkPos.y;
            }
        }
    }

    return bestY;
}*/