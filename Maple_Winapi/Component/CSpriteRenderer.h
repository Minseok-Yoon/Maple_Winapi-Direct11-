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
    // 2025-06-12 ���� �ؽ�ó�� �����ϸ� ��ŵ���� ���� ���� ����?
    void SetTexture(CTexture* _pTexture)
    {
        m_pTexture = _pTexture;

        // ��� ���̴��� ���ε� (������ �ð� �ƴ� ���� ��)
        if (_pTexture && _pTexture->GetSRV())
        {
            // ������ ���ؽ�Ʈ�� ��� ���ε�
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = _pTexture->GetSRV();
            GetDeviceContext()->PSSetShaderResources(5, 1, &srv);
        }
        else
        {
            // nullptr ���� �� ���̴� ���ҽ��� Ŭ����
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

// mesh�� material�� �̿��� ȭ�鿡 ����ϴ� ������ ������
// �ؽ�Ʈ ������ ���, �θ� ������ ����, ���̴� ���ҽ� ����, ��ǥ ��ȯ ����� ����