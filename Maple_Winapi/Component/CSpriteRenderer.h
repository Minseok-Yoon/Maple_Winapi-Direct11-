#pragma once
#include "CComponent.h"
#include "../Resource/CTexture.h"
#include "../Resource/CMaterial.h"
#include "../Resource/CMesh.h"
#include "../Component/CBaseRenderer.h"

class CSpriteRenderer : public CBaseRenderer
{
public:
    CSpriteRenderer();
    virtual ~CSpriteRenderer();

    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Render(const Matrix& view, const Matrix& projection) override;

    CTexture* GetTexture() { return m_pTexture; }
    // 2025-06-12 기존 텍스처와 동일하면 스킵하지 말고 강제 설정?
    void SetTexture(CTexture* _pTexture)
    {
        m_pTexture = _pTexture;

        // 즉시 셰이더에 바인딩 (렌더링 시가 아닌 설정 시)
        if (_pTexture && _pTexture->GetSRV())
        {
            // 렌더링 컨텍스트에 즉시 바인딩
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = _pTexture->GetSRV();
            GetDeviceContext()->PSSetShaderResources(5, 1, &srv);
        }
        else
        {
            // nullptr 설정 시 셰이더 리소스도 클리어
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> nullSRV = nullptr;
            GetDeviceContext()->PSSetShaderResources(5, 1, &nullSRV);
        }
    }

    // 2025-07-15
    void SetScreenSpace(bool bEnable) { m_bScreenSpace = bEnable; }

    // 2025-07-18
    void SetSprite(wstring strName, bool bScreenSpace /*= false*/);

private:
	CTexture*	m_pTexture;
    // 2025-07-15
    bool m_bScreenSpace = false;
};

// mesh와 material을 이용해 화면에 출력하는 역할을 하지만
// 텍스트 렌더링 기능, 부모 렌더러 연결, 셰이더 리소스 관리, 좌표 변환 기능은 없음