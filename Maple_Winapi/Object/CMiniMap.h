#pragma once
#include "CUI.h"
#include "../Resource/CTexture.h"

struct MiniMapElements
{
    CGameObject* Frame = nullptr;
    CGameObject* Background = nullptr;
    CGameObject* PlayerIcon = nullptr;
    CGameObject* MapName = nullptr;
};

class CTexture;
class CMiniMap : public CUI
{
public:
	CMiniMap();
	virtual ~CMiniMap();

    virtual void OnInit() override;
    virtual void OnActive() override;
    virtual void OnInActive() override;
    virtual void OnUpdate() override;
    virtual void OnLateUpdate() override;
    virtual void OnRender() override;
    virtual void OnClear() override;

    void CreateMiniMap(const std::wstring& mapTextureName);
    void UpdateMiniMapPlayer();

private:
    CGameObject*    m_pPlayerIcon;
    CTexture*       m_pTexture;
    Vector3         m_vRealMapScale;
    MiniMapElements m_tElements;
};