#pragma once
#include "../Object/CGameObject.h"
#include "../Component/CTransform.h"

class CUI : public CGameObject
{
public:
    struct tUIEvent
    {
        void operator=(function<void()> func)
        {
            m_tUIEvent = move(func);
        }

        void operator()()
        {
            if (m_tUIEvent)
                m_tUIEvent();
        }

        function<void()> m_tUIEvent;
    };

    CUI(UI_TYPE _eUIType);
    virtual ~CUI();

    void Init();
    void Update();
    void LateUpdate();
    void Render();
    void Active();
    void InActive();
    void UIClear();

    virtual void OnInit();
    virtual void OnActive();
    virtual void OnInActive();
    virtual void OnUpdate();
    virtual void OnLateUpdate();
    virtual void OnRender();
    virtual void OnClear();

    // 2025-07-13
    Vector2 GetScreenPosition();

    void SetUIType(UI_TYPE _eUIType) { m_eUIType = _eUIType; }
    UI_TYPE GetUIType() { return m_eUIType; }
    void SetFullScreen(bool _bEnable) { m_bFullScreen = _bEnable; }
    bool IsFullScreen() { return m_bFullScreen; }

    void SetUILocalPos(Vector2 _vPosition);
    Vector2 GetUILocalPos();

    void SetUILocalScale(Vector2 _vScale);
    Vector2 GetUILocalScale();


    void SetZOrder(int _iZOrder) { m_iZOrder = _iZOrder; }
    int GetZOrder() const { return m_iZOrder; }

    // 2025-06-10
    bool IsEnable() const { return m_bEnable; }

    void SetEnable(bool _bEnable) { m_bEnable = _bEnable; }

    // 2025-07-16 부모-자식 관계 추가
    void SetParent(CUI* pParent);
    void AddChild(CUI* pChild);
    void RemoveChild(CUI* pChild);
    CUI* GetParent() const { return m_pParent; }
    const vector<CUI*>& GetChildren() const { return m_vecChildren; }

    // 상대좌표
    void SetRelativePos(const Vector2& relativePos);
    Vector2 GetRelativePos() const { return m_vRelativePos; }

    void UpdateAbsolutePosition();

private:
    void updateChildrenPosition();

protected:
    Vector2	    m_vLocalPosition;
    Vector2	    m_vFinalPosition;
    Vector2	    m_vLocalScale;
    bool	    m_bMouseOn;
    int         m_iZOrder = 0;

    Vector2     m_vRelativePos; // 부모 기준 상대 좌표

private:
    UI_TYPE m_eUIType;
    bool	m_bFullScreen;
    bool	m_bEnable;


    CUI*            m_pParent;
    vector<CUI*>    m_vecChildren;
};

// UI클래스 내부에는 이런 저런 UI들이 있기 때문에.. 이것을 '추상 클래스'로 만들어서 '객체화'할 수 없게 만들 것이다.
// 계층 구조의 장점: 자식 UI는 부모 UI가 반드시 먼저 Update가 되어야 그 데이터를 가져다가 본인의 상태를 결정 할 수 있음.

// 2025-07-16
// 2025-07-14
//Vector2 ScreenToNDC(const Vector2& screenPos);
///*void SetScreenPos(const Vector2& screenPos)
//{
//    Vector2 ndcPos = ScreenToNDC(screenPos);
//    SetPos(ndcPos);
//
//    char dbgs[128];
//    sprintf_s(dbgs, "Actual ndcPos: (%.2f, %.2f)\n", ndcPos.x, ndcPos.y);
//    OutputDebugStringA(dbgs);
//}*/