#include "CMonster.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Component/CTransform.h"
#include "../Resource/CTexture.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CSpriteRenderer.h"

CMonster::CMonster()
{
	CTexture* monster = CResourceManager::Load<CTexture>(L"Monster", L"../Resources/Texture/Monster/OrangeMushroom.bmp");
}

CMonster::~CMonster()
{
	delete m_pCollider;
}

void CMonster::Init()
{
    CGameObject::Init();

    SetName(L"Monster");

    // CTransform�� ���� �߰�
    CTransform* monsterTransform = GetTransform();

    // SpriteRenderer ����
    CTexture* monsterTexture = CResourceManager::Find<CTexture>(L"Monster");

    CTexture::TextureSize textureSize = monsterTexture->GetTextureSize();
    float textureWidth = static_cast<float>(textureSize.width);
    float textureHeight = static_cast<float>(textureSize.height);

    // �θ��� ������ ��������
    CTransform* parentTransform = monsterTransform->GetParent();
    Vector3 parentScale = parentTransform ? parentTransform->GetWorldScale() : Vector3(1.0f, 1.0f, 1.0f);

    // �θ��� �������� �����Ͽ� �ڽ��� ���� �������� ����
    monsterTransform->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));

    CSpriteRenderer* sr = this->AddComponent<CSpriteRenderer>();
    sr->SetTexture(monsterTexture);
}

void CMonster::Update()
{
	CGameObject::Update();
}

void CMonster::LateUpdate()
{
	CGameObject::LateUpdate();
}

void CMonster::Render(const Matrix& view, const Matrix& projection)
{
	CGameObject::Render(view, projection);
}

//CCollider* monsterCollision = this->AddComponent<CCollider>();
	//
	//Vector3 monsterScale = monsterTransform->GetScale();
	//Vector3 halfSize = monsterScale * 0.5f;
	//halfSize.z = monsterScale.z;

	//// �浹 �ڽ��� ��� ����
	//monsterCollision->m_vBottomLeft = Vector3(-halfSize.x, -halfSize.y, -1.0f);
	//monsterCollision->m_vTopRight = Vector3(halfSize.x, halfSize.y, -1.0f);