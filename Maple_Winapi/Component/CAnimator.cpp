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
	// 관리하는 애니메이션 싹다 삭제.
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
		m_pCurAnimation->Update(); // 현재 애니메이션 업데이트

		if (m_pSpriteRenderer)
		{
			// 애니메이션에서 현재 프레임 텍스처 가져오기
			CTexture* curFrmTexture = m_pCurAnimation->GetCurrentFrameTexture();
			if (curFrmTexture)
			{
				m_pSpriteRenderer->SetTexture(curFrmTexture); // 현재 프레임 텍스처 설정
				//OutputDebugStringW(L"Frame updated\n"); // 디버그 출력
			}
		}

		// 애니메이션이 끝났다면
		if (m_pCurAnimation->IsFinish())
		{
			tEvents* events = FindEvents(m_pCurAnimation->GetName());

			if (events)
				events->tCompleteEvent();

			// 반복 재생 설정이 되어 있다면 애니메이션을 리셋하여 반복
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
	// 현재 애니메이션이 존재하는 경우
	if (m_pCurAnimation)
	{
		m_pCurAnimation->Render();  // 애니메이션 렌더링 호출
		//OutputDebugStringW(L"Rendering current animation\n");
	}
	else
	{
		//OutputDebugStringW(L"No animation to render\n");
	}
}

// 애니메이션 생성(애니메이션 이름, 택스쳐, 좌상단, 프레임 크기, 오프셋, 프레임카운트, 지속시간)
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
	// 이미 같은 이름의 애니메이션이 존재하는지 확인
	CAnimation* animation = nullptr;
	animation = FindAnimation(_strName);
	if (animation != nullptr)
		return;

	// 각 파일 이름에 대해 텍스처를 로드하여 vecTextures에 추가
	vector<CTexture*> vecTextures = {};
	for (const wstring& fileName : _vecFileNames)
	{
		// wstring 경로를 사용하여 텍스처를 로드
		CTexture* texture = CResourceManager::Load<CTexture>(fileName, fileName);
		vecTextures.push_back(texture);
	}

	animation = new CAnimation();
	animation->SetName(_strName);
	// 로드된 텍스처 벡터를 사용하여 프레임 애니메이션 생성
	animation->CreateFrameAnimation(_strName, vecTextures, _vLeftTop, _vSize,
		_vOffset, _fDuration);

	animation->SetAnimator(this);

	tEvents* events = new tEvents();
	m_mapEvents.insert(make_pair(_strName, events));

	// 애니메이션 맵에 추가
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

	// 생성된 파일 벡터를 사용하여 프레임 애니메이션을 생성
	CreateFrameAnimation(_strName, vecFile, Vector2(0.0f, 0.0f),
		Vector2(_fSizeX, _fSizeY), Vector2(_fOffsetX, _fOffsetY), _fDuration);
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	// map을 순회하면서 지정된 이름(_strName)에 해당하는 애니메이션을 찾는다.
	map<wstring, CAnimation*>::iterator iter = m_mapAnimations.find(_strName);

	// 찾은 결과가 맵의 끝을 가리키면 해당 애니메이션이 없으므로 nullptr를 반환
	if (iter == m_mapAnimations.end())
		return nullptr;

	// 찾은 애니메이션의 포인터를 반환
	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	CAnimation* animation = FindAnimation(_strName);
	if (animation == nullptr)
		return;

	// 애니메이션 프레임 갱신을 위한 코드
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
		m_pCurAnimation->ResetAnimation(); // 애니메이션을 초기화
		m_bRepeat = _bRepeat;
	}

	// 애니메이션 진행 중 텍스처 출력
	/*CTexture* currentTexture = m_pCurAnimation->GetCurrentFrameTexture();
	if (currentTexture)
	{
		wstring debugMessage = L"현재 출력 중인 텍스처: " + currentTexture->GetFilePath() + L"\n";
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
	// TODO: 여기에 return 문을 삽입합니다.
	tEvents* events = FindEvents(_strName);
	return events->tStratEvent.mEvent;
}

function<void()>& CAnimator::GetCompleteEvent(const wstring& _strName)
{
	// TODO: 여기에 return 문을 삽입합니다.
	tEvents* events = FindEvents(_strName);
	return events->tCompleteEvent.mEvent;
}

function<void()>& CAnimator::GetEndEvent(const wstring& _strName)
{
	// TODO: 여기에 return 문을 삽입합니다.
	tEvents* events = FindEvents(_strName);
	return events->tEndEvent.mEvent;
}