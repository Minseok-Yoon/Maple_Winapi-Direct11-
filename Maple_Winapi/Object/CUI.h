#pragma once
#include "../Object/CGameObject.h"

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

    void SetUIType(UI_TYPE _eUIType) { m_eUIType = _eUIType; }
    UI_TYPE GetUIType() { return m_eUIType; }
    void SetFullScreen(bool _bEnable) { m_bFullScreen = _bEnable; }
    bool IsFullScreen() { return m_bFullScreen; }
    void SetPos(Vector2 _vPosition) { m_vPosition = _vPosition; }
    Vector2 GetPos() { return m_vPosition; }
    void SetSize(Vector2 _vSize) { m_vSize = _vSize; }
    Vector2 GetSize() { return m_vSize; }

    void SetZOrder(int _iZOrder) { m_iZOrder = _iZOrder; }
    int GetZOrder() const { return m_iZOrder; }

    // 2025-06-10
    bool IsEnable() const { return m_bEnable; }

    void SetEnable(bool _bEnable) { m_bEnable = _bEnable; }

protected:
    Vector2	m_vPosition;
    Vector2	m_vSize;
    bool	m_bMouseOn;
    int     m_iZOrder = 0;

private:
    UI_TYPE m_eUIType;
    bool	m_bFullScreen;
    bool	m_bEnable;
    CUI*    m_pParent;
};

// UI클래스 내부에는 이런 저런 UI들이 있기 때문에.. 이것을 '추상 클래스'로 만들어서 '객체화'할 수 없게 만들 것이다.
// 계층 구조의 장점: 자식 UI는 부모 UI가 반드시 먼저 Update가 되어야 그 데이터를 가져다가 본인의 상태를 결정 할 수 있음.
