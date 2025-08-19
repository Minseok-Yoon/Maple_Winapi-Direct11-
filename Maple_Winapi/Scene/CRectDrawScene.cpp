#include "CRectDrawScene.h"
#include "../Object/CGameObject.h"
#include "../Component/CLineRenderer.h"
#include "../Manager/CResourceManager.h"
#include "../Component/CSpriteRenderer.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"
#include "../Component/CTransform.h"
#include "../Object/CBackGround.h"
#include "../Core/CCore.h"
#include "../Component/CCamera.h"
#include "../Component/CRenderer.h"
#include "../Component/CCameraScript.h"
#include "../Object/CPlayer.h"
#include "../Object/CPlayerScript.h"
#include "../Component/CAnimator.h"
#include "../Component/CRigidBody.h"
#include "../Manager/CColliderManager.h"

extern CCore core;

CRectDrawScene::CRectDrawScene() :
    m_pLineRenderer(nullptr)
{
    CResourceManager::Load<CTexture>(L"Main", L"../Resources/Texture/Main.bmp");
	CTexture* player = CResourceManager::Load<CTexture>(L"PLAYER", L"../Resources/Texture/Player/Player.bmp");
}

CRectDrawScene::~CRectDrawScene()
{
    delete m_pLineRenderer;
}

void CRectDrawScene::Enter()
{
	CScene::Enter();

    CBackGround* bgbg = Instantiate<CBackGround>(LAYER_TYPE::LT_BackGround);
    CTransform* bgTr = bgbg->GetComponent<CTransform>();
    bgTr->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

    CTexture* bgTexture = CResourceManager::Find<CTexture>(L"Main");
    if (bgTexture != nullptr)
    {
        // �ؽ�ó ����
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
        bgTr->SetLocalScale(Vector3(textureWidth, textureHeight, 1.0f));
        //bgbg->SetBackGroundTexture(bgTexture);
    }

    // ��������Ʈ ������ ����
    CSpriteRenderer* bgsr = bgbg->AddComponent<CSpriteRenderer>();
    bgsr->SetTexture(bgTexture);
}

void CRectDrawScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CRectDrawScene::Exit()
{
	CScene::Exit();
}

void CRectDrawScene::Init()
{
    CScene::Init();

    // ī�޶� ����
    CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
    CCamera* cameraComp = camera->AddComponent<CCamera>();
    cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

    CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
    renderer::mainCamera = cameraComp;

    // ��� ����
    CBackGround* bg = Instantiate<CBackGround>(LAYER_TYPE::LT_PixelBackGround);
    CTransform* bgTransform = bg->GetComponent<CTransform>();
    bgTransform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

    // ��� �ؽ�ó ����
    bg->Init();
    bg->CreateTextureFromPixel();

    // �ؽ�ó �ҷ�����
    CTexture* bgText = bg->GetBackGroundTexture();
    if (bgText != nullptr)
    {
        CTexture::TextureSize textureSize = bgText->GetTextureSize();
        float textureWidth = static_cast<float>(textureSize.width);
        float textureHeight = static_cast<float>(textureSize.height);

        // ȭ�� �ػ� ���ϱ�
        float resolutionWidth = core.GetWidth();
        float resolutionHeight = core.GetHeight();

        // �ؽ�ó�� ȭ���� ������ ������ ���
        float scaleX = resolutionWidth / textureWidth;
        float scaleY = resolutionHeight / textureHeight;

        // �� ū ������ �����Ͽ� �ؽ�ó�� ȭ���� ������ ����
        float scale = scaleX > scaleY ? scaleX : scaleY;

        // ��� �ؽ�ó�� ȭ�� ũ�⿡ ���߱� ���� ������ ����
        bgTransform->SetLocalScale(Vector3(textureWidth * scale, textureHeight * scale, 1.0f));

        // ��� �ؽ�ó�� �߽��� ȭ�� �߾����� �̵�
        bgTransform->SetLocalPosition(Vector3(resolutionWidth / 2.0f, resolutionHeight / 2.0f, 0.0f));
    }


    // �÷��̾� ����
    CGameObject* pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player);
    CTransform* plTr = pPlayer->GetComponent<CTransform>();
    plTr->SetLocalPosition(Vector3(0.0f, 0.0f, -1.0f));
    DontDestroyOnLoad(pPlayer);

    // �÷��̾� �ؽ�ó ����
    CTexture* playerTexture = CResourceManager::Find<CTexture>(L"PLAYER");
    CTransform* playerTransform = pPlayer->GetComponent<CTransform>();
    playerTransform->SetLocalScale(Vector3(54.0f, 65.0f, 1.0f));
    CSpriteRenderer* sr = pPlayer->AddComponent<CSpriteRenderer>();
    sr->SetTexture(playerTexture);
    CAnimator* animator = pPlayer->AddComponent<CAnimator>();
    animator->SetSpriteRenderer(sr);
    pPlayer->AddComponent<CPlayerScript>();

	CPlayerScript* plSr = pPlayer->GetComponent<CPlayerScript>();
	//plSr->SetPixelCollider(bg->GetPixelCollider());

    pPlayer->AddComponent<CRigidBody>();

    pPlayer->AddComponent<CCollider>();

    renderer::selectedObject = pPlayer;

	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_BackGround, true);
}

void CRectDrawScene::Update()
{
	CScene::Update();
}

void CRectDrawScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CRectDrawScene::Render()
{
	CScene::Render();
}