#include "CCameraManager.h"
#include "CSceneManager.h"
#include "../Scene/CScene.h"
#include "..\Component\CRenderer.h"

void CCameraManager::RegisterCamera(int _iOrder, CCamera* _pCamera)
{
	CScene::m_mapCamera[_iOrder] = _pCamera;
}

void CCameraManager::SetViewCameraSelect(int _iOrder)
{
	/*CScene* curScene = CSceneManager::GetCurScene();

    auto iter = cameraMap.find(_iOrder);
    if (iter == cameraMap.end())
        return;

    CCamera* FindCamera = iter->second;

    if (MainCamera != nullptr)
    {
        MainCamera->SetCameraType(CCamera::CAMERA_TYPE::CT_Unknown);
    }

    MainCamera = FindCamera;
    MainCamera->SetCameraType(CCamera::CAMERA_TYPE::CT_Main);
    renderer::mainCamera = MainCamera;*/
}

void CCameraManager::SetCameraOrder(int _iOrder)
{
	SetViewCameraSelect(_iOrder);
}
