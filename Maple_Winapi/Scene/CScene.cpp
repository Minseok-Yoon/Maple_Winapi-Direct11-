#include "CScene.h"

#include "../Core/CCore.h"
#include "../Object/CObject.h"

#include "../Component/CSpriteRenderer.h"
#include "../Component/CAudioListener.h"
#include "../Component/CAudioSource.h"

#include "../Manager/CCollisionManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CAudioClip.h"
#include "../Resource/CTexture.h"

extern CCore core;

CScene::CScene() :
	m_vecLayers{}
{
	createLayers();
}

CScene::~CScene()
{
	// 모든 Layer들을 메모리에서 삭제
	for (CLayer* layer : m_vecLayers)
	{
		delete layer;
		layer = nullptr;
	}
}

void CScene::Init()
{
	const wstring& sceneName = GetName();
	CSceneManager::SetCurScene(sceneName);
}

void CScene::Update()
{
	for (CLayer* layer : m_vecLayers)
	{
		if (layer == nullptr)
			continue;

		layer->Update();
	}
}

void CScene::LateUpdate()
{
	for (CLayer* layer : m_vecLayers)
	{
		if (layer == nullptr)
			continue;

		layer->LateUpdate();
	}
}

void CScene::Render()
{
	for (CLayer* layer : m_vecLayers)
	{
		if (layer == nullptr)
			continue;

		layer->Render();
	}
}

void CScene::Destroy()
{
	for (CLayer* layer : m_vecLayers)
	{
		if (layer == nullptr)
			continue;

		layer->Destroy();
	}
}

void CScene::Enter()
{
}

void CScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
	UINT iWidth = core.GetWidth();
	UINT iHeight = core.GetHeight();

	// 배경 객체 생성
	m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
	CTransform* backGroundTr = m_pBackGround->GetComponent<CTransform>();
	backGroundTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));  // 배경 위치 설정

	// 배경 텍스처 로드
	CTexture* bgTexture = CResourceManager::Find<CTexture>(_strBackGroundName);
	if (bgTexture != nullptr)
	{
		// 텍스처 설정
		m_pBackGround->SetBackGroundTexture(bgTexture);

		// 텍스처 크기 설정하기
		CTexture::TextureSize textureSize = bgTexture->GetTextureSize();
		if (textureSize.width <= iWidth || textureSize.height <= iHeight)
		{
			backGroundTr->SetScale(Vector3(iWidth, iHeight, 0.0f));
		}
		else
		{
			backGroundTr->SetScale(Vector3(textureSize.width, textureSize.height, 0.0f));
		}
	}

	// 스프라이트 렌더러 설정
	CSpriteRenderer* sr = m_pBackGround->AddComponent<CSpriteRenderer>();
	sr->SetTexture(bgTexture);

	// 오디오 리스너 및 소스 추가
	m_pBackGround->AddComponent<CAudioListener>();
	m_pAudioSource = m_pBackGround->AddComponent<CAudioSource>();

	// 배경 음악 로드 및 재생
	CAudioClip* ac = CResourceManager::Find<CAudioClip>(_strAudioName);
	m_pAudioSource->SetClip(ac);
	m_pAudioSource->Play();
}

void CScene::Exit()
{
	// 배경 음악 중지
	if (m_pAudioSource)
	{
		m_pAudioSource->Stop(); // 음악 정지
		m_pAudioSource = nullptr; // 포인터 초기화
	}

	m_pBackGround = nullptr;

	CColliderManager::Clear();
}

void CScene::AddGameObject(CGameObject* _pGameObj, const LAYER_TYPE _eLayerType)
{
	m_vecLayers[static_cast<UINT>(_eLayerType)]->AddGameObject(_pGameObj);
}

void CScene::EraseGameObject(CGameObject* _pGameObj)
{
	LAYER_TYPE layerType = _pGameObj->GetLayerType();
	m_vecLayers[static_cast<UINT>(layerType)]->EraseGameObject(_pGameObj);
}

void CScene::createLayers()
{
	m_vecLayers.resize(static_cast<UINT>(LAYER_TYPE::LT_End));

	for (size_t i = 0; i < static_cast<UINT>(LAYER_TYPE::LT_End); i++)
	{
		m_vecLayers[i] = new CLayer();
	}
}
