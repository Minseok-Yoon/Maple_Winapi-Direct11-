#pragma once

class CGameEngineScene
{
	friend class CGameEngineCamera;
	friend class CGameEngineCCollider;

public:
	CGameEngineScene();
	~CGameEngineScene();

	// delete Function
	// 사용하는 이유? 싱글톤, 레벨 시스템, 유일한 객체 같은 경우, 복사나 이동을 허용하면 의도치 않게 여러 인스턴스가 생길 수 있음 
	// → 게임 엔진 레벨은 보통 유일해야 함. 따라서 delete로 명시적 금지해야 함
	CGameEngineScene(const CGameEngineScene& _Other) = delete;					// 복사 생성자 삭제
	CGameEngineScene(CGameEngineScene&& _Other) noexcept = delete;				// 이동 생성자 삭제
	CGameEngineScene& operator=(const CGameEngineScene& _Other) = delete;			// 복사 대입 연산자 삭제
	CGameEngineScene& operator=(CGameEngineScene&& _Other) noexcept = delete;		// 이동 대입 연산자 삭제

	CGameEngineCamera* CreateCamera(int _iOrder, CAMERA_ORDER _eCameraOrder) {
		return CreateCamera(_iOrder, static_cast<int>(_eCameraOrder));
	}
	CGameEngineCamera* CreateCamera(int _iOrder, int _iCameraOrder);

	template <typename ObjectType, typename LayerType>
	ObjectType* CreateObject(LayerType _eOrder)
	{
		return CreateObject<ObjectType>(static_cast<int>(_eOrder));
	}

	template<typename ObjectType>
	ObjectType* CreateObject(int _iOrder = 0, wstring _strName)
	{
		ObjectType* NewChildObject = Instantiate<ObjectType>();
		ObjectInit(NecChildObject, _iOrder, _strName);
	}

protected:

private:
};