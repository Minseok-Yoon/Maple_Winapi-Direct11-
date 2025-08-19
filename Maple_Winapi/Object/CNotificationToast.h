#pragma once
#include "CUI.h"

class CNotificationToast : public CUI
{
public:
    CNotificationToast();
    virtual ~CNotificationToast();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender() override;
    virtual void OnClear() override;


private:
    float m_fElapsed = 0.0f;
    float m_fLifeTime = 5.0f;
};

// 2025-06-27 일단은 퀘스트지만 나중에 추가 알림 설정하기
