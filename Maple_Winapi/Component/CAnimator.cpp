#include "CAnimator.h"

CAnimator::CAnimator() :
	CComponent(COMPONENT_TYPE::CT_Animator),
	m_mapAnimations{},
	m_pCurAnimation(nullptr),
	m_bRepeat(false)
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
		m_pCurAnimation->Update();

		tEvents* events = FindEvents(m_pCurAnimation->GetName());

		if (m_pCurAnimation->IsFinish() == true)
		{
			if (events)
				events->tCompleteEvent();

			if (m_bRepeat == true)
				m_pCurAnimation->ResetAnimation();
		}
	}
}

void CAnimator::LateUpdate()
{
}

void CAnimator::Render()
{
	// 현재 애니메이션이 존재하는 경우
	if (nullptr != m_pCurAnimation)
		m_pCurAnimation->Render();	// 현재 애니메이션을 렌더링합니다.
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

//void CAnimator::CreateAnimationByFolder(const wstring& _strName, const wstring& _strPath,
//	Vector2 _vOffset, float _fDuration)
//{
//	CAnimation* animation = nullptr;
//	animation = FindAnimation(_strName);
//	if (animation != nullptr)
//		return;
//
//	int fileCount = 0;
//	filesystem::path fs(_strPath);
//	vector<CTexture*> images = {};
//	for (auto& p : filesystem::recursive_directory_iterator(fs))
//	{
//		wstring fileName = p.path().filename();
//		wstring fullName = p.path();
//
//		CTexture* texture = CResourceManager::Load<CTexture>(fileName, fullName);
//		images.push_back(texture);
//		fileCount++;
//	}
//
//	UINT sheetWidth = images[0]->GetWidth() * fileCount;
//	UINT sheetHeight = images[0]->GetHeight();
//	CTexture* spriteSheet = CTexture::Create(_strName, sheetWidth, sheetHeight);
//
//	UINT imageWidth = images[0]->GetWidth();
//	UINT imageHeight = images[0]->GetHeight();
//
//	for (size_t i = 0; i < images.size(); i++)
//	{
//		BitBlt(spriteSheet->GetHdc(), i * imageWidth, 0,
//			imageWidth, imageHeight, images[i]->GetHdc(), 0, 0, SRCCOPY);
//	}
//
//	CreateAltasAnimation(_strName, spriteSheet,
//		Vector2(0.0f, 0.0f), Vector2(imageWidth, imageHeight),
//		_vOffset, fileCount, _fDuration);
//}

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
	m_pCurAnimation->ResetAnimation();
	m_bRepeat = _bRepeat;
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