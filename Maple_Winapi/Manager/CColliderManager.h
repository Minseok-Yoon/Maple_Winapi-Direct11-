#pragma once
#include "../pch.h"
#include "../Component/CBoxCollider2D.h"
#include "../Component/CCircleCollider2D.h"

union COLLIDER_ID
{
    struct {
        UINT32 Left_id;
        UINT32 Right_id;
    };
    UINT64 ID;
};

class CColliderManager
{
    SINGLETON(CColliderManager);

public:
    static void Init();
    static void Update();
    static void LateUpdate();
    static void Render();
    static void Clear();

    static void CollisionLayerCheck(LAYER_TYPE _eLayerTypeLeft, LAYER_TYPE _eLayerTypeRight, bool _bEnable);
    static void LayerCollision(LAYER_TYPE _eLayerTypeLeft, LAYER_TYPE _eLayerTypeRight);
    static void ColliderCollision(CCollider* _pColLeft, CCollider* _pColRight);
    static bool Intersect(CCollider* _pColLeft, CCollider* _pColRight);

private:
    static bitset<(UINT)LAYER_TYPE::LT_End>	m_CollisionLayerMatrix[(UINT)LAYER_TYPE::LT_End];
    static unordered_map<UINT64, bool>		m_CollisionMap;
};