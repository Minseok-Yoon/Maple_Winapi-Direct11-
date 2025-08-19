#include "CColliderManager.h"
#include "CSceneManager.h"
#include "../Scene/CScene.h"
#include "../Component/CCollider.h"
#include "../Component/CTransform.h"
#include "../Object/CGameObject.h"
#include "../Test/CDrawCollisionBox.h"
#include "../Component/CLineCollider.h"

bitset<(UINT)LAYER_TYPE::LT_End> CColliderManager::m_CollisionLayerMatrix[(UINT)LAYER_TYPE::LT_End] = {};
unordered_map<UINT64, bool> CColliderManager::m_CollisionMap = {};

CColliderManager::CColliderManager()
{
}

CColliderManager::~CColliderManager()
{
}

void CColliderManager::Init()
{
    int a = 1;
    int b = a << 1;
}

void CColliderManager::Update()
{
    for (UINT row = 0; row < (UINT)LAYER_TYPE::LT_End; row++)
    {
        for (UINT col = 0; col < (UINT)LAYER_TYPE::LT_End; col++)
        {
            if (m_CollisionLayerMatrix[row][col] == true)
            {
                LayerCollision((LAYER_TYPE)row, (LAYER_TYPE)col);
            }
        }
    }
}

void CColliderManager::LateUpdate()
{
}

void CColliderManager::Render()
{
}

void CColliderManager::Clear()
{
    m_CollisionMap.clear();
    m_CollisionLayerMatrix->reset();
}

void CColliderManager::CollisionLayerCheck(LAYER_TYPE _eLayerTypeLeft,
    LAYER_TYPE _eLayerTypeRight, bool _bEnable)
{
    int row = 0;
    int col = 0;

    if (_eLayerTypeLeft <= _eLayerTypeRight)
    {
        row = (UINT)_eLayerTypeLeft;
        col = (UINT)_eLayerTypeRight;
    }
    else
    {
        row = (UINT)_eLayerTypeRight;
        col = (UINT)_eLayerTypeLeft;
    }

    m_CollisionLayerMatrix[row][col] = _bEnable;
}

void CColliderManager::LayerCollision(LAYER_TYPE _eLayerTypeLeft, LAYER_TYPE _eLayerTypeRight)
{
    const vector<CGameObject*>& vecGameObjectLefts = CSceneManager::GetGameObjects(_eLayerTypeLeft);
    const vector<CGameObject*>& vecGameObjectRights = CSceneManager::GetGameObjects(_eLayerTypeRight);

    for (CGameObject* pLeft : vecGameObjectLefts)
    {
        if (pLeft->IsActive() == false)
            continue;

        CCollider* leftCol = pLeft->GetComponent<CCollider>();
        if (leftCol == nullptr)
            continue;

        for (CGameObject* pRight : vecGameObjectRights)
        {
            if (pRight->IsActive() == false)
                continue;

            CCollider* rightCol = pRight->GetComponent<CCollider>();
            if (rightCol == nullptr)
                continue;

            if (pLeft == pRight)
                continue;

            ColliderCollision(leftCol, rightCol);
        }
    }
}

void CColliderManager::ColliderCollision(CCollider* _pColLeft, CCollider* _pColRight)
{
    // 두 충돌체 번호로 가져온 ID 확인하여 CollisionID 세팅
    COLLIDER_ID id = {};
    id.Left_id = _pColLeft->GetID();
    id.Right_id = _pColRight->GetID();

    // 이전 충돌 정보를 검색한다.
    // 만약에 충돌정보가 없는 상태라면
    // 충돌정보를 생성해준다.

    auto iter = m_CollisionMap.find(id.ID);
    if (iter == m_CollisionMap.end())
    {
        m_CollisionMap.insert(make_pair(id.ID, false));
        iter = m_CollisionMap.find(id.ID);
    }

    // 충돌체크를 해준다
    if (Intersect(_pColLeft, _pColRight))
    {
        // 최초 충돌할 때
        if (iter->second == false)
        {
            _pColLeft->OnCollisionEnter(_pColRight);
            _pColRight->OnCollisionEnter(_pColLeft);
            iter->second = true;
        }
        else // 이미 충돌 중
        {
            _pColLeft->OnCollisionStay(_pColRight);
            _pColRight->OnCollisionStay(_pColLeft);
        }
    }
    else
    {
        // 충돌을 하지 않은 상태
        if (iter->second == true)
        {
            _pColLeft->OnCollisionExit(_pColRight);
            _pColRight->OnCollisionExit(_pColLeft);

            iter->second = false;
        }
    }
}

bool CColliderManager::Intersect(CCollider* _pColLeft, CCollider* _pColRight)
{
    CTransform* leftTr = _pColLeft->GetOwner()->GetComponent<CTransform>();
    CTransform* rightTr = _pColRight->GetOwner()->GetComponent<CTransform>();

    Vector2 leftPos = leftTr->GetLocalPosition() + _pColLeft->GetOffsetPos();
    Vector2 rightPos = rightTr->GetLocalPosition() + _pColRight->GetOffsetPos();

    // size 1, 1 일때 기본크기가 100px
    // 충돌체의 크기 계산 (스케일 고려)
    Vector2 leftSize = Vector2(
        _pColLeft->GetScale().x * _pColLeft->GetOwner()->GetComponent<CTransform>()->GetLocalScale().x,
        _pColLeft->GetScale().y * _pColLeft->GetOwner()->GetComponent<CTransform>()->GetLocalScale().y
    );

    Vector2 rightSize = Vector2(
        _pColRight->GetScale().x * _pColRight->GetOwner()->GetComponent<CTransform>()->GetLocalScale().x,
        _pColRight->GetScale().y * _pColRight->GetOwner()->GetComponent<CTransform>()->GetLocalScale().y
    );

    // 충돌체 타입 확인
    COLLIDER_TYPE leftColType = _pColLeft->GetColliderType();
    COLLIDER_TYPE rightColType = _pColRight->GetColliderType();

    if (leftColType == COLLIDER_TYPE::CT_Rect2D && rightColType == COLLIDER_TYPE::CT_Rect2D)
    {
        // 각 충돌체의 중심점 계산
        Vector3 leftCenter = leftTr->GetWorldPosition();
        Vector3 rightCenter = rightTr->GetWorldPosition();

        // 충돌 검사 (크기를 기반으로)
        bool xOverlap = std::abs(leftCenter.x - rightCenter.x) <= (leftSize.x + rightSize.x) / 2.0f;
        bool yOverlap = std::abs(leftCenter.y - rightCenter.y) <= (leftSize.y + rightSize.y) / 2.0f;

        // 충돌 판정 (Z축은 무시)
        return xOverlap && yOverlap;
    }

    // 라인 충돌체와 박스 충돌체 간의 충돌 검사 (2D 충돌 검사 적용)
    else if (leftColType == COLLIDER_TYPE::CT_Line2D && rightColType == COLLIDER_TYPE::CT_Rect2D)
    {
        CLineCollider* lineCollider = static_cast<CLineCollider*>(_pColLeft);
        return lineCollider->LineToBoxCollision(Vector2(rightPos.x - rightSize.x / 2.0f, rightPos.y - rightSize.y / 2.0f),
            Vector2(rightPos.x + rightSize.x / 2.0f, rightPos.y + rightSize.y / 2.0f));
    }
    else if (leftColType == COLLIDER_TYPE::CT_Rect2D && rightColType == COLLIDER_TYPE::CT_Line2D)
    {
        CLineCollider* lineCollider = static_cast<CLineCollider*>(_pColRight);
        return lineCollider->LineToBoxCollision(Vector2(leftPos.x - leftSize.x / 2.0f, leftPos.y - leftSize.y / 2.0f),
            Vector2(leftPos.x + leftSize.x / 2.0f, leftPos.y + leftSize.y / 2.0f));
    }

    return false;
}
