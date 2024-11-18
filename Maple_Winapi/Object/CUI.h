#pragma once
#include "../Entity/CEntity.h"

class CUI : public CEntity
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
    void Render(HDC _Hdc);
    void Active();
    void InActive();
    void UIClear();

    virtual void OnInit();
    virtual void OnActive();
    virtual void OnInActive();
    virtual void OnUpdate();
    virtual void OnLateUpdate();
    virtual void OnRender(HDC _Hdc);
    virtual void OnClear();

    void SetUIType(UI_TYPE _eUIType) { m_eUIType = _eUIType; }
    UI_TYPE GetUIType() { return m_eUIType; }
    void SetFullScreen(bool _bEnable) { m_bFullScreen = _bEnable; }
    bool IsFullScreen() { return m_bFullScreen; }
    void SetPos(Vector2 _vPosition) { m_vPosition = _vPosition; }
    Vector2 GetPos() { return m_vPosition; }
    void SetSize(Vector2 _vSize) { m_vSize = _vSize; }
    Vector2 GetSize() { return m_vSize; }

protected:
    Vector2	m_vPosition;
    Vector2	m_vSize;
    bool	m_bMouseOn;

private:
    UI_TYPE m_eUIType;
    bool	m_bFullScreen;
    bool	m_bEnable;
    CUI* m_pParent;
};

// UIŬ���� ���ο��� �̷� ���� UI���� �ֱ� ������.. �̰��� '�߻� Ŭ����'�� ���� '��üȭ'�� �� ���� ���� ���̴�.
// ���� ������ ����: �ڽ� UI�� �θ� UI�� �ݵ�� ���� Update�� �Ǿ�� �� �����͸� �����ٰ� ������ ���¸� ���� �� �� ����.