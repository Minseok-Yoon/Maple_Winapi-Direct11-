#pragma once
#include "../Component/CCamera.h"

class CCameraManager
{
public:
	void RegisterCamera(int _iOrder, CCamera* _pCamera);
	void SetViewCameraSelect(int _iOrder);

	void SetCameraOrder(CAMERA_ORDER _eOrder)
	{
		SetCameraOrder(static_cast<int>(_eOrder));
	}

	void SetCameraOrder(int _iOrder);

private:
	CCamera* MainCamera = nullptr;
};

