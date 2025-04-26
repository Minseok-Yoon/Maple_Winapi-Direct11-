#include "CAnimator.h"
#include "../Component/CSpriteRenderer.h"
#include "../Object/CGameObject.h"

CAnimator::CAnimator() :
	CComponent(COMPONENT_TYPE::CT_Animator),
	m_mapAnimations{},
	m_pCurAnimation(nullptr),
	m_bRepeat(false),
	m_pSpriteRenderer(nullptr)
{
}

CAnimator::~CAnimator()
{
	// �����ϴ� �ִϸ��̼� �ϴ� ����.
	for (auto& iter : m_mapAnimations)
	{
		delete iter.second;
		iter.second = nullptr;
	}

	for (auto& iter : m_mapEvents)
	{
		delete iter.second;
		iter.second = nullptr;
	}
}

void CAnimator::Init()
{
}

void CAnimator::Update()
{
	if (m_pCurAnimation)
	{
		m_pCurAnimation->Update(); // ���� �ִϸ��̼� ������Ʈ

		if (m_pSpriteRenderer)
		{
			// �ִϸ��̼ǿ��� ���� ������ �ؽ�ó ��������
			CTexture* curFrmTexture = m_pCurAnimation->GetCurrentFrameTexture();
			if (curFrmTexture)
			{
				m_pSpriteRenderer->SetTexture(curFrmTexture); // ���� ������ �ؽ�ó ����
				//OutputDebugStringW(L"Frame updated\n"); // ����� ���
			}
		}

		// �ִϸ��̼��� �����ٸ�
		if (m_pCurAnimation->IsFinish())
		{
			tEvents* events = FindEvents(m_pCurAnimation->GetName());

			if (events)
				events->tCompleteEvent();

			// �ݺ� ��� ������ �Ǿ� �ִٸ� �ִϸ��̼��� �����Ͽ� �ݺ�
			if (m_bRepeat)
			{
				m_pCurAnimation->ResetAnimation();
			}
		}
	}
}

void CAnimator::LateUpdate()
{
}

void CAnimator::Render(const Matrix& view, const Matrix& projection)
{
	// ���� �ִϸ��̼��� �����ϴ� ���
	if (m_pCurAnimation)
	{
		m_pCurAnimation->Render();  // �ִϸ��̼� ������ ȣ��
		//OutputDebugStringW(L"Rendering current animation\n");
	}
	else
	{
		//OutputDebugStringW(L"No animation to render\n");
	}
}

// �ִϸ��̼� ����(�ִϸ��̼� �̸�, �ý���, �»��, ������ ũ��, ������, ������ī��Ʈ, ���ӽð�)
void CAnimator::CreateAltasAnimation(const wstring& _strName, CTexture* _pTexture,
	Vector2 _vLeftTop, Vector2 _vSize, Vector2 _vOffset, UINT _iFrameCount, float _fDuration)
{
	CAnimation* animation = nullptr;
	animation = FindAnimation(_strName);
	if (animation != nullptr)
		return;

	animation = new CAnimation();
	animation->SetName(_strName);
	animation->CreateAtlasAnimation(_strName, _pTexture, _vLeftTop,
		_vSize, _vOffset, _iFrameCount, _fDuration);

	animation->SetAnimator(this);

	tEvents* events = new tEvents();
	m_mapEvents.insert(make_pair(_strName, events));

	m_mapAnimations.insert(make_pair(_strName, animation));
}

void CAnimator::CreateFrameAnimation(const wstring& _strName, const vector<wstring>& _vecFileNames,
	Vector2 _vLeftTop, Vector2 _vSize, Vector2 _vOffset, float _fDuration)
{
	// �̹� ���� �̸��� �ִϸ��̼��� �����ϴ��� Ȯ��
	CAnimation* animation = nullptr;
	animation = FindAnimation(_strName);
	if (animation != nullptr)
		return;

	// �� ���� �̸��� ���� �ؽ�ó�� �ε��Ͽ� vecTextures�� �߰�
	vector<CTexture*> vecTextures = {};
	for (const wstring& fileName : _vecFileNames)
	{
		// wstring ��θ� ����Ͽ� �ؽ�ó�� �ε�
		CTexture* texture = CResourceManager::Load<CTexture>(fileName, fileName);
		vecTextures.push_back(texture);
	}

	animation = new CAnimation();
	animation->SetName(_strName);
	// �ε�� �ؽ�ó ���͸� ����Ͽ� ������ �ִϸ��̼� ����
	animation->CreateFrameAnimation(_strName, vecTextures, _vLeftTop, _vSize,
		_vOffset, _fDuration);

	animation->SetAnimator(this);

	tEvents* events = new tEvents();
	m_mapEvents.insert(make_pair(_strName, events));

	// �ִϸ��̼� �ʿ� �߰�
	m_mapAnimations.insert(make_pair(_strName, animation));
}

void CAnimator::AddFrameAnimation(const wstring& _strName, const wchar_t* _pFilePath, int _iFrameMax,
	Vector2 _vLeftTop, float _fSizeX, float _fSizeY, float _fOffsetX, float _fOffsetY, float _fDuration)
{
	vector<wstring> vecFile;

	for (int i = 1; i <= _iFrameMax; i++)
	{
		wchar_t strFileName[MAX_PATH] = {};
		wsprintf(strFileName, _pFilePath, i);
		vecFile.push_back(strFileName);
	}

	// ������ ���� ���͸� ����Ͽ� ������ �ִϸ��̼��� ����
	CreateFrameAnimation(_strName, vecFile, Vector2(0.0f, 0.0f),
		Vector2(_fSizeX, _fSizeY), Vector2(_fOffsetX, _fOffsetY), _fDuration);
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	// map�� ��ȸ�ϸ鼭 ������ �̸�(_strName)�� �ش��ϴ� �ִϸ��̼��� ã�´�.
	map<wstring, CAnimation*>::iterator iter = m_mapAnimations.find(_strName);

	// ã�� ����� ���� ���� ����Ű�� �ش� �ִϸ��̼��� �����Ƿ� nullptr�� ��ȯ
	if (iter == m_mapAnimations.end())
		return nullptr;

	// ã�� �ִϸ��̼��� �����͸� ��ȯ
	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	CAnimation* animation = FindAnimation(_strName);
	if (animation == nullptr)
		return;

	// �ִϸ��̼� ������ ������ ���� �ڵ�
	if (m_pCurAnimation != animation)
	{
		if (m_pCurAnimation)
		{
			tEvents* currentEvents = FindEvents(m_pCurAnimation->GetName());
			if (currentEvents)
				currentEvents->tEndEvent();
		}

		tEvents* nextEvents = FindEvents(animation->GetName());
		if (nextEvents)
			nextEvents->tStratEvent();

		m_pCurAnimation = animation;
		m_pCurAnimation->ResetAnimation(); // �ִϸ��̼��� �ʱ�ȭ
		m_bRepeat = _bRepeat;
	}

	// �ִϸ��̼� ���� �� �ؽ�ó ���
	/*CTexture* currentTexture = m_pCurAnimation->GetCurrentFrameTexture();
	if (currentTexture)
	{
		wstring debugMessage = L"���� ��� ���� �ؽ�ó: " + currentTexture->GetFilePath() + L"\n";
		OutputDebugString(debugMessage.c_str());
	}*/
}

bool CAnimator::End() const
{
	if (m_pCurAnimation != nullptr)
		return m_pCurAnimation->IsFinish();

	return false;
}

void CAnimator::ResetAnimation()
{
	if (m_pCurAnimation != nullptr)
	{
		m_pCurAnimation->SetCurrentFrame(0);
		m_pCurAnimation->ResetAnimation();
	}
}

CAnimator::tEvents* CAnimator::FindEvents(const wstring& _strName)
{
	auto iter = m_mapEvents.find(_strName);
	if (iter == m_mapEvents.end())
		return nullptr;

	return iter->second;
}

function<void()>& CAnimator::GetStartEvent(const wstring& _strName)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	tEvents* events = FindEvents(_strName);
	return events->tStratEvent.mEvent;
}

function<void()>& CAnimator::GetCompleteEvent(const wstring& _strName)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	tEvents* events = FindEvents(_strName);
	return events->tCompleteEvent.mEvent;
}

function<void()>& CAnimator::GetEndEvent(const wstring& _strName)
{
	// TODO: ���⿡ return ���� �����մϴ�.
	tEvents* events = FindEvents(_strName);
	return events->tEndEvent.mEvent;
}