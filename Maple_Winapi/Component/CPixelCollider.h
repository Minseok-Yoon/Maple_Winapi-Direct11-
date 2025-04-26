#pragma once
#include "CCollider.h"
#include "../Resource/CTexture.h"
#include "../Graphics/CGraphicsDevice_DX11.h"

class CPixelCollider : public CCollider
{
public:
	CPixelCollider();
	virtual ~CPixelCollider();

private:

	int count = 0;

	int				m_iOriginX;
	int				m_iOriginY;

	vector<PIXEL>	m_vecPixel;
	int				m_iWidth;
	int				m_iHeight;
	Vector2			m_vHitPoint;

	float SRGBToUNORM(float srgbValue) {
		if (srgbValue <= 0.04045f) {
			return srgbValue / 12.92f;
		}
		return powf((srgbValue + 0.055f) / 1.055f, 2.4f);
	}

	// DirectX11 ���� ��� ����
	std::shared_ptr<CTexture> m_pTexture; // CTexture Ÿ��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pStagingTexture; // CPU���� ���� ������ Staging Texture

public:
	const vector<PIXEL>& GetPixelData()	const { return m_vecPixel; }

	int GetWidth()	const { return m_iWidth; }
	int GetHeight()	const { return m_iHeight; }

	void SetHitPoint(const Vector2& _vHitPos) { m_vHitPoint = _vHitPos; }
	Vector2 GetHitPoint() const { return m_vHitPoint; }

	void SetOrigin(int originX, int originY) {
		m_iOriginX = originX;
		m_iOriginY = originY;
	}

public:
	// �ȼ� ������ �����ϴ� �Լ�
	bool SetPixelInfo(const wstring& _pFileName);
	PIXEL GetPixelColor(int x, int y);

	bool IsColliding(int worldX, int worldY);

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	// �浹 ���� �Լ�
	virtual void OnCollisionEnter(class CCollider* _pOther) final;	// �浹 ���� ��
	virtual void OnCollisionStay(class CCollider* _pOther)	final;		// �浹 ��
	virtual void OnCollisionExit(class CCollider* _pOther)	final;		// �浹 ���� ��
};