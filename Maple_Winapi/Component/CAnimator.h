#pragma once
#include "CComponent.h"
#include "CAnimation.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CSpriteRenderer.h"

class CTexture;
class CAnimator : public CComponent
{
public:
	struct tEvent
	{
		void operator=(function<void()> func)
		{
			mEvent = move(func);
		}

		void operator()() const
		{
			if (mEvent)
				mEvent();
		}

		function<void()> mEvent;
	};

	struct tEvents
	{
		tEvent	tStratEvent;
		tEvent	tCompleteEvent;
		tEvent	tEndEvent;
	};

public:
	CAnimator();
	virtual ~CAnimator();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;

	void CreateAltasAnimation(const wstring& _strName,
		CTexture* _pTexture, Vector2 _vLeftTop,
		Vector2 _vSize, Vector2 _vOffset,
		UINT _iFrameCount, float _fDuration);
	void CreateFrameAnimation(const wstring& _strName,
		const vector<wstring>& _vecFileNames, Vector2 _vLeftTop,
		Vector2 _vSize, Vector2 _vOffset, float _fDuration);

	void AddFrameAnimation(const wstring& _strName, const wchar_t* _pFilePath, int _iFrameMax,
		Vector2 _vLeftTop, float _fSizeX, float _fSizeY, float _fOffsetX, float _fOffsetY, float _fDuration);

	CAnimation* FindAnimation(const wstring& _strName);

	void Play(const wstring& _strName, bool _bRepeat = true);
	bool End() const;
	void ResetAnimation();
	bool IsFinish() const { return m_pCurAnimation->IsFinish(); }

	tEvents* FindEvents(const wstring& _strName);
	function<void()>& GetStartEvent(const wstring& _strName);
	function<void()>& GetCompleteEvent(const wstring& _strName);
	function<void()>& GetEndEvent(const wstring& _strName);

	CAnimation* GetCurrentAnimation() { return m_pCurAnimation; }

	void SetSpriteRenderer(CSpriteRenderer* _pSpriteRenderer) { m_pSpriteRenderer = _pSpriteRenderer; }

private:
	map<wstring, CAnimation*>	m_mapAnimations;	// 모든 Animation
	CAnimation* m_pCurAnimation;	// 현재 재생중인 Animation
	bool						m_bRepeat;			// 반복재생 여부

	CSpriteRenderer*			m_pSpriteRenderer;
	map<wstring, tEvents*>		m_mapEvents;
};

// 현재 애니메이션의 프레임 정보를 이용해 애니메이션이 끝났는지 확인