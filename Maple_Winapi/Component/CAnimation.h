#pragma once
#include "../Resource/CResource.h"
#include "../Resource/CTexture.h"

class CAnimation : public CResource
{
public:
	struct tAnimFrm
	{
		Vector2		vLeftTop;	// �ִϸ��̼� �ؽ�ó������ ��������
		Vector2		vSize;		// �������� ũ��
		Vector2		vOffSet;	// �������� ������(�������� ������� ��ġ�� ����)
		float		fDuration;	// �������� ���ӽð�(�ش� �������� �� ���� ȭ�鿡 ǥ�õǴ��� ����)
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
	vector<tAnimFrm>	m_vecAnimationFrm;		// ��� ������ ����
	int					m_iCurFrm;				// ���� ������
	float				m_fAccTime;				// �ð� ����
	bool				m_bFinish;				// ��� ���� ���޿���

	class CAnimator* m_pAnimator;			// �ִϸ��̼��� ����
	CTexture* m_pTexture;				// �ִϸ��̼��� ����� �ý�ó
};