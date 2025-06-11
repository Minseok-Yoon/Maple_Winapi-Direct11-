#include "CGameObject.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CSceneManager.h"
#include "../Component/CTransform.h"
#include "..\Scene\CScene.h"

void Destroy(CGameObject* _pGameObject)
{
	if (_pGameObject != nullptr)
		_pGameObject->death();
}

CGameObject::CGameObject() :
	m_eObjectState(OBJECT_STATE::OS_Active),
	m_eLayerType(LAYER_TYPE::LT_None)
{
	m_vecComponents.resize((UINT)COMPONENT_TYPE::CT_End);
	initializeTransform();
}

CGameObject::~CGameObject()
{
	for (CComponent* comp : m_vecComponents)
	{
		if (comp == nullptr)
			continue;

		SAFE_DELETE(comp);
	}
}

void CGameObject::Init()
{
	for (CComponent* comp : m_vecComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Init();
	}
}

void CGameObject::Update()
{
	if (IsDead()) return;

	for (CComponent* comp : m_vecComponents)
	{
		if (comp == nullptr)
			continue;
		
		comp->Update();
	}
}

void CGameObject::LateUpdate()
{
	for (CComponent* comp : m_vecComponents)
	{
		if (comp == nullptr)
			continue;

		comp->LateUpdate();
	}
}

void CGameObject::Render(const Matrix& view, const Matrix& projection)
{
	// 몬스터의 체력이 0이 되면 오브젝트 사라지도록 구현
	// 플레이어가 맵을 다시 이동 시 맵보다 뒤에 출력되는 현상 수정.
	// 몬스터 죽으면 아이템 메소 떨어뜨리기.
	if (!IsActive()) return;

	for (CComponent* comp : m_vecComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Render(view, projection);
	}
}

void CGameObject::Reset()
{
	SetActive(false);
	SetObjectState(OBJECT_STATE::OS_None);

	// 위치 초기화, 컴포넌트 초기화 등
	CTransform* tr = GetComponent<CTransform>();
	if (tr) tr->SetLocalPosition(Vector3::Zero);
}

void CGameObject::initializeTransform()
{
	// CTransform을 생성하여 m_pTransform에 할당
	m_pTransform = this->AddComponent<CTransform>();

	// 만약 CTransform이 제대로 추가되지 않았으면 오류 로그를 출력
	if (!m_pTransform)
	{
		OutputDebugStringA("Error: Failed to initialize CTransform!\n");
	}
}
