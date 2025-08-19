#pragma once
#include "CComponent.h"

class CTransform;
class CBackGround;
class CPixelCollider : public CComponent
{
public:
	CPixelCollider();
	virtual ~CPixelCollider();

	virtual void Init();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render(const Matrix& view, const Matrix& projection);

	bool CheckGround(const Vector3& worldPos);
	float FindGroundY(const Vector3& currentPos, const Vector3& prevPos);

	bool IsColliding(int worldX, int worldY);

	void SetCheckSize(float width, float depth) {
		m_fCheckWidth = width;
		m_fCheckDepth = depth;
	}

	const vector<PIXEL>& GetPixelData()	const { return m_vecPixel; }

	int GetWidth()	const { return m_iWidth; }
	int GetHeight()	const { return m_iHeight; }

private:
	float			m_fCheckWidth;
	float			m_fCheckDepth;

	int				m_iOriginX;
	int				m_iOriginY;
	
	vector<PIXEL>	m_vecPixel;
	int				m_iWidth;
	int				m_iHeight;

	CTransform*		m_pTransform;
	CBackGround*	m_pBackGround;
};

//class CPixelCollider : public CCollider
//{
//public:
//	CPixelCollider();
//	virtual ~CPixelCollider();
//
//private:
//	int count = 0;
//
//	int				m_iOriginX;
//	int				m_iOriginY;
//
//	vector<PIXEL>	m_vecPixel;
//	int				m_iWidth;
//	int				m_iHeight;
//	Vector2			m_vHitPoint;
//
//	float SRGBToUNORM(float srgbValue) {
//		if (srgbValue <= 0.04045f) {
//			return srgbValue / 12.92f;
//		}
//		return powf((srgbValue + 0.055f) / 1.055f, 2.4f);
//	}
//
//	// DirectX11 관련 멤버 변수
//	std::shared_ptr<CTexture> m_pTexture; // CTexture 타입
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pStagingTexture; // CPU에서 접근 가능한 Staging Texture
//
//public:
//	const vector<PIXEL>& GetPixelData()	const { return m_vecPixel; }
//
//	int GetWidth()	const { return m_iWidth; }
//	int GetHeight()	const { return m_iHeight; }
//
//	void SetHitPoint(const Vector2& _vHitPos) { m_vHitPoint = _vHitPos; }
//	Vector2 GetHitPoint() const { return m_vHitPoint; }
//
//	void SetOrigin(int originX, int originY) {
//		m_iOriginX = originX;
//		m_iOriginY = originY;
//	}
//
//public:
//	// 픽셀 정보를 설정하는 함수
//	bool SetPixelInfo(const wstring& _pFileName);
//	PIXEL GetPixelColor(int x, int y);
//
//	bool IsColliding(int worldX, int worldY);
//
//	// 2025-06-30
//	bool IsPixelColliding(const Vector3& worldPos);
//
//public:
//	void Init() override;
//	void Update() override;
//	void LateUpdate() override;
//	void Render(const Matrix& view, const Matrix& projection) override;
//
//	// 충돌 시점 함수
//	virtual void OnCollisionEnter(class CCollider* _pOther) final;	// 충돌 진입 시
//	virtual void OnCollisionStay(class CCollider* _pOther)	final;		// 충돌 중
//	virtual void OnCollisionExit(class CCollider* _pOther)	final;		// 충돌 해제 시
//};