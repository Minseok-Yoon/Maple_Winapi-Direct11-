#include "CScene.h"

#include "../Core/CCore.h"
#include "../Object/CObject.h"

#include "../Component/CSpriteRenderer.h"
#include "../Component/CAudioListener.h"
#include "../Component/CAudioSource.h"

#include "../Manager/CColliderManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CAudioClip.h"
#include "../Resource/CTexture.h"
#include <imgui.h>
#include "../Component/CBaseRenderer.h"
#include "..\Component\CRenderer.h"

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
	for (CCamera* camera : m_vecCameras)
	{
		if (camera == nullptr) continue;

		renderer::activeCamera = camera;

		Matrix viewMatrix = camera->GetViewMatrix();
		Matrix projectionMatrix = camera->GetProjectionMatrix();
		Vector3 cameraPos = camera->GetOwner()->GetComponent<CTransform>()->GetWorldPosition();

		vector<CGameObject*> opaqueList = {};
		vector<CGameObject*> cutoutList = {};
		vector<CGameObject*> transparentList = {};
		vector<CGameObject*> uiList = {};

		// 기존 방식대로 수집
		CollectRenderables(camera, opaqueList, cutoutList, transparentList);

		// UI 오브젝트 따로 수집
		for (CGameObject* obj : m_vecLayers[static_cast<UINT>(LAYER_TYPE::LT_UI)]->GetGameObjects())
		{
			if (obj == nullptr) continue;
			if (obj->GetComponent<CBaseRenderer>() == nullptr) continue;

			// 카메라 마스크 체크
			UINT camMask = camera->GetCameraMask();
			UINT objMask = (1 << static_cast<UINT>(obj->GetLayerType()));
			if ((camMask & objMask) == 0)
				continue;

			uiList.push_back(obj);
		}

		// 💡 UI는 ZOrder로 정렬
		std::ranges::sort(uiList, [](CGameObject* a, CGameObject* b) {
			return a->GetZOrder() < b->GetZOrder();
		});

		RenderRenderables(uiList, viewMatrix, projectionMatrix);

		// 기존 렌더링
		SortByDistance(opaqueList, cameraPos, true);
		SortByDistance(cutoutList, cameraPos, true);
		SortByDistance(transparentList, cameraPos, false);

		RenderRenderables(opaqueList, viewMatrix, projectionMatrix);
		RenderRenderables(cutoutList, viewMatrix, projectionMatrix);
		RenderRenderables(transparentList, viewMatrix, projectionMatrix);
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
	backGroundTr->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));  // 배경 위치 설정

	// 배경 텍스처 로드
	CTexture* bgTexture = CResourceManager::Find<CTexture>(_strBackGroundName);
	if (bgTexture != nullptr)
	{
		// 텍스처 설정
		//m_pBackGround->SetBackGroundTexture(bgTexture);

		// 텍스처 크기 설정하기
		CTexture::TextureSize textureSize = bgTexture->GetTextureSize();
		if (textureSize.width <= iWidth || textureSize.height <= iHeight)
		{
			backGroundTr->SetLocalScale(Vector3(iWidth, iHeight, 0.0f));
		}
		else
		{
			backGroundTr->SetLocalScale(Vector3(textureSize.width, textureSize.height, 0.0f));
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
	if (m_pAudioSource)
	{
		m_pAudioSource->Stop();
		m_pAudioSource = nullptr;
	}

	if (m_pBackGround)
	{
		m_pBackGround->death();
		m_pBackGround = nullptr;
	}

	DeleteAll(); // 모든 오브젝트, 카메라, 컴포넌트 제거

	CColliderManager::Clear();
}

void CScene::DeleteAll()
{
	for (CLayer* layer : m_vecLayers)
	{
		if (layer == nullptr)
			continue;

		vector<CGameObject*>& objs = layer->GetGameObjects();
		for (CGameObject* obj : objs)
		{
			if (obj)
				obj->death(); // 상태만 Dead로 변경
		}

		layer->Destroy(); // 죽은 오브젝트 삭제
	}

	m_vecCameras.clear(); // 씬에 등록된 카메라도 정리
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

void CScene::AddCamera(CCamera* _pCamera)
{
	if (_pCamera == nullptr) return;

	m_vecCameras.push_back(_pCamera);
}

void CScene::RemoveCamera(CCamera* _pCamera)
{
	if (_pCamera == nullptr) return;

	auto iter
		= find(m_vecCameras.begin(), m_vecCameras.end(), _pCamera);

	if (iter != m_vecCameras.end())
		m_vecCameras.erase(iter);
}

void CScene::CollectRenderables(CCamera* _pCamera, vector<CGameObject*>& opaqueList,
	vector<CGameObject*>& cutoutList, vector<CGameObject*>& transparentList) const
{
	UINT camMask = _pCamera->GetCameraMask();

	for (CLayer* layer : m_vecLayers)
	{
		if (layer == nullptr)
			continue;

		vector<CGameObject*>& gameObjects = layer->GetGameObjects();

		for (CGameObject* gameObj : gameObjects)
		{
			if (gameObj == nullptr)
				continue;

			UINT objLayerMask = (1 << static_cast<UINT>(gameObj->GetLayerType()));

			// 카메라 마스크에 포함 안되면 제외
			if ((camMask & objLayerMask) == 0)
				continue;

			CBaseRenderer* renderer = gameObj->GetComponent<CBaseRenderer>();
			if (renderer == nullptr)
				continue;

			switch (renderer->GetMaterial()->GetRenderingMode())
			{
			case RENDERING_MODE::RM_Opaque:
				opaqueList.push_back(gameObj);
				break;
			case RENDERING_MODE::RM_CutOut:
				cutoutList.push_back(gameObj);
				break;
			case RENDERING_MODE::RM_Transparent:
				transparentList.push_back(gameObj);
				break;
			}
		}
	}

	// ZOrder 기준으로 각 렌더링 리스트 정렬
	std::ranges::sort(opaqueList, [](CGameObject* a, CGameObject* b) {
		return a->GetZOrder() < b->GetZOrder();
	});

	std::ranges::sort(cutoutList, [](CGameObject* a, CGameObject* b) {
		return a->GetZOrder() < b->GetZOrder();
	});

	std::ranges::sort(transparentList, [](CGameObject* a, CGameObject* b) {
		return a->GetZOrder() < b->GetZOrder();
	});
}

void CScene::SortByDistance(std::vector<CGameObject*>& renderList, 
	const Vector3& cameraPos, bool bAscending) const
{
	auto comparator = [cameraPos, bAscending](CGameObject* a, CGameObject* b)
	{
		float distA = Vector3::Distance(a->GetComponent<CTransform>()->GetWorldPosition(), cameraPos);
		float distB = Vector3::Distance(b->GetComponent<CTransform>()->GetWorldPosition(), cameraPos);
		return bAscending ? (distA < distB) : (distA > distB);
	};

	std::ranges::sort(renderList, comparator);
}

void CScene::RenderRenderables(const vector<CGameObject*>& renderList, 
	const Matrix& view, const Matrix& projection) const
{
	for (auto* obj : renderList)
	{
		if (obj == nullptr)
			continue;

		obj->Render(view, projection);
	}
}

CGameObject* CScene::FindObjectByName(const wstring& _strName)
{
	for (CLayer* layer : m_vecLayers)
	{
		CGameObject* found = layer->FindObjectByName(_strName);
		if (found)
			return found;
	}
	return nullptr;
}

void CScene::createLayers()
{
	m_vecLayers.resize(static_cast<UINT>(LAYER_TYPE::LT_End));

	for (size_t i = 0; i < static_cast<UINT>(LAYER_TYPE::LT_End); i++)
	{
		m_vecLayers[i] = new CLayer();
	}
}

/*for (CLayer* layer : m_vecLayers)
	{
		if (layer == nullptr)
			continue;

		vector<CGameObject*>& gameObjects = layer->GetGameObjects();

		for (CGameObject* gameObj : gameObjects)
		{
			if (gameObj == nullptr)
				continue;

			CBaseRenderer* renderer = gameObj->GetComponent<CBaseRenderer>();
			if (renderer == nullptr)
				continue;

			switch (renderer->GetMaterial()->GetRenderingMode())
			{
			case RENDERING_MODE::RM_Opaque:
				opaqueList.push_back(gameObj);
				break;

			case RENDERING_MODE::RM_CutOut:
				cutoutList.push_back(gameObj);
				break;

			case RENDERING_MODE::RM_Transparent:
				transparentList.push_back(gameObj);
				break;
			}
		}
	}*/