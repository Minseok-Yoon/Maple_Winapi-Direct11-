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
	vector<PIXEL>	m_vecPixel;
	int				m_iWidth;
	int				m_iHeight;
	Vector2			m_vHitPoint;

	// DirectX11 관련 멤버 변수
	std::shared_ptr<CTexture> m_pTexture; // CTexture 타입
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pStagingTexture; // CPU에서 접근 가능한 Staging Texture

public:
	const vector<PIXEL>& GetPixel()	const { return m_vecPixel; }

	int GetWidth()	const { return m_iWidth; }
	int GetHeight()	const { return m_iHeight; }

	void SetHitPoint(const Vector2& _vHitPos) { m_vHitPoint = _vHitPos; }
	Vector2 GetHitPoint() const { return m_vHitPoint; }

public:
	// 픽셀 정보를 설정하는 함수
	bool SetPixelInfo(const wstring& _pFileName);
	PIXEL GetPixelColor(int x, int y);

public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	// 충돌 시점 함수
	virtual void OnCollisionEnter(class CCollider* _pOther) final;	// 충돌 진입 시
	virtual void OnCollisionStay(class CCollider* _pOther)	final;		// 충돌 중
	virtual void OnCollisionExit(class CCollider* _pOther)	final;		// 충돌 해제 시
};