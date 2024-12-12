#pragma once
#include "../Resource/CResource.h"
#include "../Resource/CTexture.h"

class CAnimation : public CResource
{
public:
	struct tAnimFrm
	{
		Vector2		vLeftTop;	// 애니메이션 텍스처에서의 시작지점
		Vector2		vSize;		// 프레임의 크기
		Vector2		vOffSet;	// 프레임의 오프셋(프레임의 상대적인 위치를 조정)
		float		fDuration;	// 프레임의 지속시간(해당 프레임이 얼마 동안 화면에 표시되는지 결정)
		CTexture*	pTexture;

		tAnimFrm() :
			vLeftTop(Vector2::Zero),
			vSize(Vector2::Zero),
			vOffSet(Vector2::Zero),
			fDuration(0.0f),
			pTexture(nullptr)
		{}
	};

	CAnimation();
	virtual ~CAnimation();

	void Update();
	void Render();

	virtual HRESULT Load(const wstring& _strPath) override;

	void CreateAtlasAnimation(const wstring& _strName,
		CTexture* _pTexture, Vector2 _vLeftTop,
		Vector2 _vSize, Vector2 _vOffset, UINT _iFrameCount, float _fDuration);
	void CreateFrameAnimation(const wstring& _strName,
		const vector<CTexture*>& _vecTextures, Vector2 _vLeftTop,
		Vector2 _vSize, Vector2 _vOffset, float _fDuration);
	void ResetAnimation();

	ANIMATION_STATE GetAnimationState() { return m_eAnimationState; }

	UINT GetMaxFrame() { return (UINT)m_vecAnimationFrm.size(); }

	bool IsFinish() { return m_bFinish; }

	void SetAnimator(class CAnimator* _pAnimator) { m_pAnimator = _pAnimator; }

private:
	ANIMATION_STATE		m_eAnimationState;
	vector<tAnimFrm>	m_vecAnimationFrm;		// 모든 프레임 정보
	int					m_iCurFrm;				// 현재 프레임
	float				m_fAccTime;				// 시간 누적
	bool				m_bFinish;				// 재생 끝에 도달여부

	class CAnimator* m_pAnimator;			// 애니메이션의 제어
	CTexture* m_pTexture;				// 애니메이션이 사용할 택스처
};