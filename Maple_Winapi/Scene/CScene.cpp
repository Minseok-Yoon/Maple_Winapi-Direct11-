#include "CScene.h"

//#include "../Object/CBackGround.h"
#include "../Object/CObject.h"

#include "../Component/CSpriteRenderer.h"
#include "../Component/CAudioListener.h"
#include "../Component/CAudioSource.h"

#include "../Manager/CCollisionManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CAudioClip.h"

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
	//// 배경 객체 생성
	m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);

	// 스프라이트 렌더러 설정
	CSpriteRenderer* bgSR = m_pBackGround->AddComponent<CSpriteRenderer>();

	CTexture* bgTexture = CResourceManager::Find<CTexture>(_strBackGroundName);
	if (bgTexture)
	{
		bgSR->SetTexture(bgTexture);
		OutputDebugStringW(L"Texture loaded successly!\n");
	}
	else
	{
		OutputDebugStringA("Texture not found for Key!\n");
	}

	// 오디오 리스너 및 소스 추가
	m_pBackGround->AddComponent<CAudioListener>();
	m_pAudioSource = m_pBackGround->AddComponent<CAudioSource>();

	// 배경 음악 로드 및 재생
	CAudioClip* ac = CResourceManager::Find<CAudioClip>(_strAudioName);
	if (ac != nullptr)
	{
		OutputDebugStringW(L"Load Sound!\n");
	}
	else
	{
		OutputDebugStringW(L"Not found Sound!\n");
	}
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

	//m_vMapSize = Vector2(0.0f, 0.0f);

	CColliderManager::Clear();
}

void CScene::AddGameObject(CGameObject* _pGameObj, const LAYER_TYPE _eLayerType)
{
	m_vecLayers[(UINT)_eLayerType]->AddGameObject(_pGameObj);
}

void CScene::EraseGameObject(CGameObject* _pGameObj)
{
	LAYER_TYPE layerType = _pGameObj->GetLayerType();
	m_vecLayers[(UINT)layerType]->EraseGameObject(_pGameObj);
}

void CScene::createLayers()
{
	m_vecLayers.resize((UINT)LAYER_TYPE::LT_End);

	for (size_t i = 0; i < (UINT)LAYER_TYPE::LT_End; i++)
	{
		m_vecLayers[i] = new CLayer();
	}
}
