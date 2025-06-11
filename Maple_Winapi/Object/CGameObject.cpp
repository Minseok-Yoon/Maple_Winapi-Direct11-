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
	// ������ ü���� 0�� �Ǹ� ������Ʈ ��������� ����
	// �÷��̾ ���� �ٽ� �̵� �� �ʺ��� �ڿ� ��µǴ� ���� ����.
	// ���� ������ ������ �޼� ����߸���.
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

	// ��ġ �ʱ�ȭ, ������Ʈ �ʱ�ȭ ��
	CTransform* tr = GetComponent<CTransform>();
	if (tr) tr->SetLocalPosition(Vector3::Zero);
}

void CGameObject::initializeTransform()
{
	// CTransform�� �����Ͽ� m_pTransform�� �Ҵ�
	m_pTransform = this->AddComponent<CTransform>();

	// ���� CTransform�� ����� �߰����� �ʾ����� ���� �α׸� ���
	if (!m_pTransform)
	{
		OutputDebugStringA("Error: Failed to initialize CTransform!\n");
	}
}
