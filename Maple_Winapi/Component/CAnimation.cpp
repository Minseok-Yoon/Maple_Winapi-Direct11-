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

	m_fAccTime += fDeltaTime;

	if (m_fAccTime >= m_vecAnimationFrm[m_iCurFrm].fDuration)
	{
		m_fAccTime = 0.0f;
		if (m_iCurFrm <= m_vecAnimationFrm.size() - 1)
		{
			++m_iCurFrm;
		}
		else
		{
			if (m_eAnimationState == ANIMATION_STATE::AS_Finish)
			{
				m_bFinish = true;
			}
		}
	}
}

void CAnimation::Render()
{
	if (m_pTexture == nullptr)
		return;
}

HRESULT CAnimation::Load(const wstring& _strPath)
{
	return E_NOTIMPL;
}

// Atlas �ִϸ��̼�. ū �̹������� ���� �������� �߶󳻾� ����ϴ� �ִϸ��̼� ���
// _vLT, _vSliceSize, _vStep�� ����Ͽ� �������� ���� ��ġ�� ũ��, ���� ����
void CAnimation::CreateAtlasAnimation(const wstring& _strName, CTexture* _pTexture,
	Vector2 _vLeftTop, Vector2 _vSize, Vector2 _vOffset, UINT _iFrameCount, float _fDuration)
{
	// ���� �ؽ�ó���� ������ ��� ����
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

// Frame �ִϸ��̼�. ���� ���� �ؽ�ó�� ���������� ����ϴ� ���.
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
	m_fAccTime = 0.0f;	// �ð� ���� �ʱ�ȭ
	m_iCurFrm = 0;		// ù ��° ���������� ����
	m_bFinish = false;	// ��� �� ���� �ʱ�ȭ
}