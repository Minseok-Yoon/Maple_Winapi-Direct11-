#pragma once

class CGameEngineScene
{
	friend class CGameEngineCamera;
	friend class CGameEngineCCollider;

public:
	CGameEngineScene();
	~CGameEngineScene();

	// delete Function
	// ����ϴ� ����? �̱���, ���� �ý���, ������ ��ü ���� ���, ���糪 �̵��� ����ϸ� �ǵ�ġ �ʰ� ���� �ν��Ͻ��� ���� �� ���� 
	// �� ���� ���� ������ ���� �����ؾ� ��. ���� delete�� ����� �����ؾ� ��
	CGameEngineScene(const CGameEngineScene& _Other) = delete;					// ���� ������ ����
	CGameEngineScene(CGameEngineScene&& _Other) noexcept = delete;				// �̵� ������ ����
	CGameEngineScene& operator=(const CGameEngineScene& _Other) = delete;			// ���� ���� ������ ����
	CGameEngineScene& operator=(CGameEngineScene&& _Other) noexcept = delete;		// �̵� ���� ������ ����

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