#include "CAnimation.h"
#include "CAnimator.h"
#include "../Object/CGameObject.h"
#include "../Manager/CTimeManager.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"
#include "../Component/CTransform.h"

CAnimation::CAnimation() :
	CResource(RESOURCE_TYPE::RT_Animation),
	m_pAnimator(nullptr),
	m_pTexture(nullptr),
	m_vecAnimationFrm{},
	m_iCurFrm(-1),
	m_fAccTime(0.0f),
	m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}


void CAnimation::Update()
{
	if (m_bFinish)
		return;

	// 애니메이션 진행 시간 축적
	m_fAccTime += CTimeManager::GetfDeltaTime();

	if (m_iCurFrm >= 0 && m_iCurFrm < m_vecAnimationFrm.size())
	{
		float frameDuration = m_vecAnimationFrm[m_iCurFrm].fDuration;
	
		if (m_fAccTime > frameDuration)
		{
			m_fAccTime = 0.0f;
			if (m_iCurFrm < m_vecAnimationFrm.size() - 1)
				m_iCurFrm++;
			else
				m_bFinish = true;
		}
	}

	////// 프레임의 지속 시간이 초과되면 다음 프레임으로 넘어가기
	//if (m_fAccTime > m_vecAnimationFrm[m_iCurFrm].fDuration)
	//{
	//	m_fAccTime = 0.0f;
	//
	//	// 현재 프레임이 마지막 프레임이 아니면, 다음 프레임으로 진행
	//	if (m_iCurFrm > m_vecAnimationFrm.size() - 1)
	//	{
	//		m_iCurFrm++;
	//	}
	//	else
	//	{
	//		// 마지막 프레임에 도달하면 애니메이션 종료
	//		m_bFinish = true;
	//	}
	//}
}

void CAnimation::Render(const Matrix& view, const Matrix& projection)
{
	if (m_pTexture == nullptr)
		return;
}

HRESULT CAnimation::Load(const wstring& _strPath)
{
	return E_NOTIMPL;
}

// Atlas 애니메이션. 큰 이미지에서 여러 프레임을 잘라내어 사용하는 애니메이션 방식
// _vLT, _vSliceSize, _vStep을 사용하여 프레임의 시작 위치와 크기, 간격 설정
void CAnimation::CreateAtlasAnimation(const wstring& _strName, CTexture* _pTexture,
	Vector2 _vLeftTop, Vector2 _vSize, Vector2 _vOffset, UINT _iFrameCount, float _fDuration)
{
	// 기존 텍스처들을 저장할 멤버 변수
	m_pTexture = _pTexture;
	for (size_t i = 0; i < _iFrameCount; i++)
	{
		tAnimFrm animFrm = {};
		animFrm.vLeftTop.x = _vLeftTop.x + (_vSize.x * i);
		animFrm.vLeftTop.y = _vLeftTop.y;
		animFrm.vSize = _vSize;
		animFrm.vOffSet = _vOffset;
		animFrm.fDuration = _fDuration;

		m_vecAnimationFrm.push_back(animFrm);
	}
}

// Frame 애니메이션. 여러 개의 텍스처를 순차적으로 사용하는 방식.
void CAnimation::CreateFrameAnimation(const wstring& _strName, const vector<CTexture*>& _vecTextures,
	Vector2 _vLeftTop, Vector2 _vSize, Vector2 _vOffset, float _fDuration)
{
	m_vecAnimationFrm.clear();

	for (size_t i = 0; i < _vecTextures.size(); i++)
	{
		tAnimFrm animFrm = {};
		animFrm.pTexture = _vecTextures[i];
		animFrm.vLeftTop = _vLeftTop;
		animFrm.vSize = _vSize;
		animFrm.vOffSet = _vOffset;
		animFrm.fDuration = _fDuration;

		m_vecAnimationFrm.push_back(animFrm);
	}
}

void CAnimation::ResetAnimation()
{
	m_fAccTime = 0.0f;	// 시간 누적 초기화
	m_iCurFrm = 0;		// 첫 번째 프레임으로 설정
	m_bFinish = false;	// 재생 끝 상태 초기화
}

CTexture* CAnimation::GetCurrentFrameTexture() const
{
	if (m_iCurFrm >= 0 && m_iCurFrm < m_vecAnimationFrm.size())
	{
		return m_vecAnimationFrm[m_iCurFrm].pTexture; // 해당 프레임의 텍스처 반환
	}
	return nullptr;
}

void CAnimation::SetCurrentFrame(int _iFrame)
{
	if (_iFrame >= 0 && _iFrame < m_vecAnimationFrm.size())  // 유효한 프레임 번호 체크
	{
		m_iCurFrm = _iFrame;  // 프레임 설정
		m_fAccTime = 0.0f;   // 시간 리셋
	}
}


//if (m_iCurFrm >= 0 && m_iCurFrm < m_vecAnimationFrm.size())
//{
//	float frameDuration = m_vecAnimationFrm[m_iCurFrm].fDuration;
//	/*OutputDebugStringW((L"[Update] AccTime: " + to_wstring(m_fAccTime) +
//		L", Frame: " + to_wstring(m_iCurFrm) +
//		L", Duration: " + to_wstring(frameDuration) + L"\n").c_str());*/

//	if (m_fAccTime > frameDuration)
//	{
//		m_fAccTime = 0.0f;
//		if (m_iCurFrm < m_vecAnimationFrm.size() - 1)
//			m_iCurFrm++;
//		else
//			m_bFinish = true;
//	}
//}