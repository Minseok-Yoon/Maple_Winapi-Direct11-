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
#include "../Manager/CResourceManager.h"
#include "../Manager/CSceneManager.h"
#include "../Component/CSpriteRenderer.h"
#include "../Scene/CScene.h"

CPlayer::CPlayer() :
	m_fAttackDelayTime(1.2f),  // 공격 후 0.2초 동안 대기
	m_fElapsedTime(0.0f),
	m_bAttackCycle(false)
{
	CTexture* player = CResourceManager::Load<CTexture>(L"Player", L"../Resources/Texture/Player/Player.bmp");
    SetName(L"Player");
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
    CGameObject::Init();

    m_pTransform = this->AddComponent<CTransform>();
    m_pTransform->SetLocalPosition(Vector3(-400.0f, 400.0f, -1.0f));
    m_pTransform->SetLocalScale(Vector3(54.0f, 65.0f, 0.0f));

    if (m_pCollider == nullptr)
    {
        m_pCollider = this->AddComponent<CCollider>();
        m_pCollider->SetBottomLeft(Vector3(-0.5f, -0.5f, 0.0f));
        m_pCollider->SetTopRight(Vector3(0.5f, 0.5f, 0.0f));

        if (!m_pCollider)
        {
            OutputDebugStringA("ERROR: Collider creation failed in CPlayer::Init()\n");
        }
        else
        {
            OutputDebugStringA("INFO: Collider successfully added to Player\n");
        }
    }

    // SpriteRenderer 설정
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"Player");

    CTexture::TextureSize textureSize = playerTexture->GetTextureSize();
    float textureWidth = static_cast<float>(textureSize.width);
    float textureHeight = static_cast<float>(textureSize.height);

    CSpriteRenderer* sr = this->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);
    CGravity* gravity = this->AddComponent<CGravity>();
    if (gravity) gravity->SetPlayer(this);

    CPlayerScript* plSr = this->AddComponent<CPlayerScript>();
}

void CPlayer::Update()
{
    CGameObject::Update();
}

void CPlayer::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CPlayer::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

bool CPlayer::CheckGround(Vector3 _fPlusCheckPos)
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