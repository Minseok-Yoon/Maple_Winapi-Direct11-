#include "../pch.h"
#include "CTestScene.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Object/CObject.h"
#include "../Object/CPlayer.h"
#include "../Object/CPlayerScript.h"
#include "../Component/CCamera.h"
#include "../Component/CCameraScript.h"
#include "../Component/CRenderer.h"
#include "../Component/CSpriteRenderer.h"
#include "../Component/CRigidBody.h"
#include "../Component/CAnimator.h"
#include "../Core/CCore.h"

extern CCore core;

CTestScene::CTestScene()
{
	CResourceManager::Load<CTexture>(L"Stage02_BG", L"../Resources/Texture/Main.bmp");
	CTexture* player = CResourceManager::Load<CTexture>(L"PLAYER", L"../Resources/Texture/Player/Player.bmp");
}

CTestScene::~CTestScene()
{
}

void CTestScene::Enter()
{
	CScene::Enter();

	// ��� ��ü ����
	CBackGround* bg = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
	CTransform* bgTr = bg->GetComponent<CTransform>();
	bgTr->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

	// ��� �ؽ�ó �ε�
	CTexture* bgTexture = CResourceManager::Find<CTexture>(L"Stage02_BG");
	if (bgTexture != nullptr)
	{
		CTexture::TextureSize textureSize = bgTexture->GetTextureSize();
		float textureWidth = static_cast<float>(textureSize.width);
		float textureHeight = static_cast<float>(textureSize.height);

		// ȭ�� �ػ󵵸� ��������
		float resolutionWidth = core.GetWidth();
		float resolutionHeight = core.GetHeight();

		// �ؽ�ó�� ȭ���� ������ ������ ���� ���
		float scaleX = resolutionWidth / textureWidth;
		float scaleY = resolutionHeight / textureHeight;

		// �� ū ������ �����Ͽ� �ؽ�ó�� ȭ���� ������ ����
		float scale = scaleX > scaleY ? scaleX : scaleY;

		// ��� ũ�⸦ �����ϸ�
		bgTr->SetScale(Vector3(textureWidth, textureHeight, 1.0f));
		bg->SetBackGroundTexture(bgTexture);
	}

	// ��� ������ ����
	CSpriteRenderer* bgSr = bg->AddComponent<CSpriteRenderer>();
	bgSr->SetTexture(bgTexture);
}

void CTestScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CTestScene::Exit()
{
	CScene::Exit();
}

void CTestScene::Init()
{
    CScene::Init();

    // ī�޶� ����
    CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
    CCamera* cameraComp = camera->AddComponent<CCamera>();
    cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

    CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
    renderer::mainCamera = cameraComp;

    // �÷��̾� ����
    CGameObject* pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
    DontDestroyOnLoad(pPlayer);
    //cameraComp->SetTarget(pPlayer);

    // �÷��̾� �ؽ�ó �ε�
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");

    // �ؽ�ó ũ�� ��������
    CTexture::TextureSize playerTextureSize = playerTexture->GetTextureSize();
    float textureWidth = static_cast<float>(playerTextureSize.width);
    float textureHeight = static_cast<float>(playerTextureSize.height);

    // ȭ�� �ػ� ��������
    float resolutionWidth = core.GetWidth();
    float resolutionHeight = core.GetHeight();

    // �ؽ�ó�� ȭ���� ������ ������ ���� ���
    float scaleX = resolutionWidth / textureWidth;
    float scaleY = resolutionHeight / textureHeight;

    // �� ū ������ �����Ͽ� �ؽ�ó�� ȭ���� ������ ����
    float scale = scaleX > scaleY ? scaleX : scaleY;

    // �÷��̾��� ũ�⸦ ���� ũ���� 54x65�� ����
    CTransform* playerTransform = pPlayer->GetComponent<CTransform>();
    playerTransform->SetScale(Vector3(54.0f, 65.0f, 1.0f));

    // SpriteRenderer�� �ִϸ����� ������Ʈ �߰�
    CSpriteRenderer* sr = pPlayer->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);
    CAnimator* animator = pPlayer->AddComponent<CAnimator>();
    animator->SetSpriteRenderer(sr);

    // �÷��̾ CPlayerScript ������Ʈ �߰�
    CPlayerScript* playerScript = pPlayer->AddComponent<CPlayerScript>();

    pPlayer->AddComponent<CRigidBody>();

    renderer::selectedObject = pPlayer;
}

void CTestScene::Update()
{
	CScene::Update();
}

void CTestScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CTestScene::Render()
{
	CScene::Render();
}