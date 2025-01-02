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
	// ��� Layer���� �޸𸮿��� ����
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

	// ��� ��ü ����
	m_pBackGround = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
	CTransform* backGroundTr = m_pBackGround->GetComponent<CTransform>();
	backGroundTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));  // ��� ��ġ ����

	// ��� �ؽ�ó �ε�
	CTexture* bgTexture = CResourceManager::Find<CTexture>(_strBackGroundName);
	if (bgTexture != nullptr)
	{
		// �ؽ�ó ����
		m_pBackGround->SetBackGroundTexture(bgTexture);

		// �ؽ�ó ũ�� �����ϱ�
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

	// ��������Ʈ ������ ����
	CSpriteRenderer* sr = m_pBackGround->AddComponent<CSpriteRenderer>();
	sr->SetTexture(bgTexture);

	// ����� ������ �� �ҽ� �߰�
	m_pBackGround->AddComponent<CAudioListener>();
	m_pAudioSource = m_pBackGround->AddComponent<CAudioSource>();

	// ��� ���� �ε� �� ���
	CAudioClip* ac = CResourceManager::Find<CAudioClip>(_strAudioName);
	m_pAudioSource->SetClip(ac);
	m_pAudioSource->Play();
}

void CScene::Exit()
{
	// ��� ���� ����
	if (m_pAudioSource)
	{
		m_pAudioSource->Stop(); // ���� ����
		m_pAudioSource = nullptr; // ������ �ʱ�ȭ
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
