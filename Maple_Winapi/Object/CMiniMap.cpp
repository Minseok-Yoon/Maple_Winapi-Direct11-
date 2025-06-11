#include "CMiniMap.h"
#include "../Core/CCore.h"
#include "../Component/CTransform.h"
#include "../Object/CGameObject.h"
#include "../Manager/CResourceManager.h"
#include "../Resource/CTexture.h"
#include "../Component/CSpriteRenderer.h"
#include "../Manager/CSceneManager.h"
#include "../Object/CObject.h"

extern CCore core;

CMiniMap::CMiniMap() :
	CUI(UI_TYPE::UT_MiniMap),
    m_pTexture(nullptr)
{
    CResourceManager::Load<CTexture>(L"MiniMap_Player", L"../Resources/Texture/MiniMap/Player_MiniMap.png");
}

CMiniMap::~CMiniMap()
{
}

void CMiniMap::OnInit()
{
    SetLayerType(LAYER_TYPE::LT_UI);
    CreateMiniMap(L"Lake of Oblivion_MiniMap");
}

void CMiniMap::OnActive()
{
}

void CMiniMap::OnInActive()
{
}

void CMiniMap::OnUpdate()
{
}

void CMiniMap::OnLateUpdate()
{
    UpdateMiniMapPlayer();
}

void CMiniMap::OnRender()
{
}

void CMiniMap::OnClear()
{
}

void CMiniMap::CreateMiniMap(const std::wstring& mapTextureName)
{
    if (m_tElements.Background || m_tElements.Frame || m_tElements.PlayerIcon)
        return;

    SetName(mapTextureName);

    UINT screenWidth = core.GetWidth();
    UINT screenHeight = core.GetHeight();

    // Load texture
    m_pTexture = CResourceManager::Find<CTexture>(mapTextureName);
    CTexture::TextureSize textureSize = m_pTexture->GetTextureSize();
    float miniMapWidth = textureSize.width;
    float miniMapHeight = textureSize.height;

    Vector3 localPos(
        (-static_cast<float>(screenWidth) / 2.0f + miniMapWidth / 2.0f) + 20.0f,
        (static_cast<float>(screenHeight) / 2.0f - miniMapHeight) - 20.0f,
        0.0f
    );

    // Background
    m_tElements.Background = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
    m_tElements.Background->SetName(L"MiniMap_Background");
    m_tElements.Background->SetZOrder(0);
    auto bgTr = m_tElements.Background->AddComponent<CTransform>();
    bgTr->SetLocalPosition(localPos);
    bgTr->SetLocalScale(Vector3(miniMapWidth, miniMapHeight, 0.0f));
    m_tElements.Background->AddComponent<CSpriteRenderer>()->SetTexture(m_pTexture);

    // Frame
    //m_tElements.Frame = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
    //m_tElements.Frame->SetName(L"MiniMap_Frame");
    //auto frTr = m_tElements.Frame->AddComponent<CTransform>();
    //frTr->SetLocalPosition(localPos);
    //frTr->SetLocalScale(Vector3(miniMapWidth + 10.0f, miniMapHeight + 10.0f, 0.0f));
    //m_tElements.Frame->AddComponent<CSpriteRenderer>()->SetTexture(CResourceManager::Find<CTexture>(L"Minimap_Frame"));

    // Player icon
    m_tElements.PlayerIcon = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
    m_tElements.PlayerIcon->SetName(L"MiniMap_Player");
    m_tElements.PlayerIcon->SetZOrder(1);
    auto plTr = m_tElements.PlayerIcon->AddComponent<CTransform>();
    //plTr->SetParent(bgTr);
    //plTr->SetIgnoreParentScale(true);
    plTr->SetLocalScale(Vector3(7.0f, 10.0f, 0.0f));
    m_tElements.PlayerIcon->AddComponent<CSpriteRenderer>()->SetTexture(
        CResourceManager::Find<CTexture>(L"MiniMap_Player")
    );

    // Load real world map size
    CTexture* worldTex = CResourceManager::Find<CTexture>(L"Lake of Oblivion_2");
    m_vRealMapScale = Vector3(
        static_cast<float>(worldTex->GetTextureSize().width),
        static_cast<float>(worldTex->GetTextureSize().height),
        0.0f
    );

    // Optional: Map name
    /*m_tElements.MapName = Instantiate<CTextUI>(LAYER_TYPE::LT_UI);
    m_tElements.MapName->SetText(L"Lake of Oblivion", 14.0f, D2D1::ColorF::White);
    m_tElements.MapName->GetComponent<CTransform>()->SetLocalPosition(
        Vector3(localPos.x, localPos.y + miniMapHeight / 2.0f + 15.0f, 0.0f)
    );*/

    /*wstringstream ss;
    ss << L"MiniMap WorldPosition: (" << m_tElements.Background->GetComponent<CTransform>()->GetWorldPosition().x
        << L", " << m_tElements.Background->GetComponent<CTransform>()->GetWorldPosition().y << L")\n";
    OutputDebugStringW(ss.str().c_str());*/
}

void CMiniMap::UpdateMiniMapPlayer()
{
    // ���� �÷��̾� ��ġ
    CGameObject* pPlayer = CSceneManager::GetCurScene()->GetPlayer();
    Vector3 playerWorldPos = pPlayer->GetComponent<CTransform>()->GetWorldPosition();

    // �̴ϸ� �ؽ�ó ������
    Vector3 realMapSize = m_vRealMapScale; // ���� ����� ������
    Vector3 miniMapSize = m_tElements.Background->GetComponent<CTransform>()->GetLocalScale(); // �̴ϸ� ������
    Vector3 miniMapPos = m_tElements.Background->GetComponent<CTransform>()->GetWorldPosition(); // �̴ϸ� ��ġ

    // --- 1. �÷��̾� ��ġ�� ����ȭ ---
    float normX = std::clamp(playerWorldPos.x / realMapSize.x, -1.0f, 1.0f);
    float normY = std::clamp(playerWorldPos.y / realMapSize.y, -1.0f, 1.0f);

    // --- 2. ����ȭ�� ��ġ�� �̴ϸ� ���� ��ǥ�� ȯ�� ---
    float iconX = miniMapPos.x + normX * miniMapSize.x;
    float iconY = miniMapPos.y + normY * miniMapSize.y;

    // --- 3. ������ ��ġ ���� ---
    m_tElements.PlayerIcon->GetComponent<CTransform>()->SetLocalPosition(Vector3(iconX, iconY, -1.0f));
}

/*wstringstream ss;
   ss << L"PlayerIcon Final Local: (" << m_tElements.PlayerIcon->GetComponent<CTransform>()->GetLocalPosition().x
       << L", " << m_tElements.PlayerIcon->GetComponent<CTransform>()->GetLocalPosition().y << L")\n";
   ss << L"PlayerIcon Final World: (" << m_tElements.PlayerIcon->GetComponent<CTransform>()->GetWorldPosition().x
       << L", " << m_tElements.PlayerIcon->GetComponent<CTransform>()->GetWorldPosition().y << L")\n";
   OutputDebugStringW(ss.str().c_str());*/
   /*wstringstream ss;
   *
   ss << L"[PlayerPos] World: (" << playerWorldPos.x << L", " << playerWorldPos.y << L")\n";
   ss << L"MiniMap Pos: , Size: (" << miniMapPos.x << L", " << miniMapPos.y << L")\n";
   ss << L"MiniMap Size: (" << miniMapSize.x << L", " << miniMapSize.y << L")\n";
   ss << L"IconPos: (" << iconX << L", " << iconY << L")\n";
   OutputDebugStringW(ss.str().c_str());*/

//void CMiniMap::OnLateUpdate()
//{
//    //// 1. �÷��̾� ã��
//    //CGameObject* pPlayer = CSceneManager::GetCurScene()->GetPlayer();
//    //if (pPlayer == nullptr || m_pTexture == nullptr)
//    //    return;
//
//    //// 2. ���� & �̴ϸ� �ؽ�ó ũ��
//    //CTexture* pBackGroundTex = CResourceManager::Find<CTexture>(L"Lake of Oblivion_2");
//    //if (pBackGroundTex == nullptr)
//    //    return;
//
//    //Vector2 worldSize = {
//    //    (float)pBackGroundTex->GetTextureSize().width,
//    //    (float)pBackGroundTex->GetTextureSize().height
//    //};
//
//    //Vector2 miniMapSize = {
//    //    (float)m_pTexture->GetTextureSize().width,
//    //    (float)m_pTexture->GetTextureSize().height
//    //};
//
//    //// 3. �÷��̾� ��ġ ���� (�߾� ���� �� �»�� �������� ������ ����)
//    //Vector3 playerWorldPos = pPlayer->GetComponent<CTransform>()->GetWorldPosition();
//    //Vector3 mapOffset(worldSize.x / 2.0f, worldSize.y / 2.0f, 0.0f);
//    //Vector3 adjustedPos = playerWorldPos + mapOffset;
//
//    //float ratioX = adjustedPos.x / worldSize.x;
//    //float ratioY = adjustedPos.y / worldSize.y;
//
//    //// 4. ������ 0~1 ���̷� clamp
//    //ratioX = std::clamp(ratioX, 0.0f, 1.0f);
//    //ratioY = std::clamp(ratioY, 0.0f, 1.0f);
//
//    //// 5. �̴ϸ� ���� ��ǥ ��� (���� ����)
//    //float iconX = ratioX * miniMapSize.x - miniMapSize.x / 2.0f;
//    //float iconY = (1.0f - ratioY) * miniMapSize.y - miniMapSize.y / 2.0f;
//
//    //// 6. ������ ��ġ ����
//    //m_pPlayerIcon->GetComponent<CTransform>()->SetLocalPosition(Vector3(iconX, iconY, 0.0f));
//    //m_pPlayerIcon->GetComponent<CTransform>()->SetLocalScale(Vector3(7.0f, 10.0f, 0.0f));
//
//    //// 7. ����� �α�
//    //wstringstream ss;
//    //ss << L"[PlayerPos] World: (" << playerWorldPos.x << L", " << playerWorldPos.y << L")\n";
//    //ss << L"[Ratio] X: " << ratioX << L", Y: " << ratioY << L"\n";
//    //ss << L"[MiniMapSize] " << miniMapSize.x << L", " << miniMapSize.y << L"\n";
//    //ss << L"[Icon] LocalPos: (" << iconX << L", " << iconY << L")\n";
//    //OutputDebugStringW(ss.str().c_str());
//}

//if (m_pTexture != nullptr)
    //{
    //    // ��������Ʈ ������ �߰� �� �ؽ�ó ����
    //    CSpriteRenderer* sr = AddComponent<CSpriteRenderer>();
    //    sr->SetTexture(m_pTexture);
    //}

/* wstringstream ss;
    ss << L"MiniMap Pos: (" << localPos.x << L", " << localPos.y << L")\n";
    OutputDebugStringW(ss.str().c_str());

    wstringstream ws;
    ws << L"MiniMap World Pos: (" << worldPos.x << L", " << worldPos.y << L", " << worldPos.z << L")\n";
    OutputDebugStringW(ws.str().c_str());*/

    //m_pPlayerIcon->GetComponent<CTransform>()->SetParent(GetComponent<CTransform>());

//void CMiniMap::CreateMiniMap(const std::wstring& mapTextureName)
//{
    //SetName(_MiniMapName);

    //// ȭ�� ũ�� ��������
    //UINT screenWidth = core.GetWidth();
    //UINT screenHeight = core.GetHeight();

    //// �ؽ�ó �ε�
    //m_pTexture = CResourceManager::Find<CTexture>(_MiniMapName);
    //CTexture::TextureSize textureSize = m_pTexture->GetTextureSize();

    //float miniMapWidth = textureSize.width;
    //float miniMapHeight = textureSize.height;

    //// ����׷��� ī�޶� ����: �߽��� (0, 0)�̹Ƿ� ����
    //Vector3 localPos(
    //    -static_cast<float>(screenWidth) / 2.0f + miniMapWidth / 2.0f,
    //    static_cast<float>(screenHeight) / 2.0f - miniMapHeight,
    //    0.0f
    //);

    //CTransform* tr = GetComponent<CTransform>();
    //tr->SetLocalPosition(localPos);
    //tr->SetLocalScale(Vector3(miniMapWidth, miniMapHeight, 0.0f));
    //Vector3 worldPos = tr->GetWorldPosition();
    //this->SetZOrder(0);

    //if (m_pTexture != nullptr)
    //{
    //    // ��������Ʈ ������ �߰� �� �ؽ�ó ����
    //    CSpriteRenderer* sr = AddComponent<CSpriteRenderer>();
    //    sr->SetTexture(m_pTexture);
    //}

    //CTexture* pBackGroundTex = CResourceManager::Find<CTexture>(L"Lake of Oblivion_2");
    //Vector3 worldSize = {
    //    (float)pBackGroundTex->GetTextureSize().width,
    //    (float)pBackGroundTex->GetTextureSize().height,
    //    0.0f
    //};

    //m_vRealMapScale = worldSize;

    //m_pPlayerIcon = Instantiate<CBackGround>(LAYER_TYPE::LT_UI);
    //m_pPlayerIcon->SetName(L"MiniMap_Player");
    //m_pPlayerIcon->AddComponent<CTransform>();
    //CSpriteRenderer* iconRenderer = m_pPlayerIcon->AddComponent<CSpriteRenderer>();
    //iconRenderer->SetTexture(CResourceManager::Find<CTexture>(L"MiniMap_Player"));

    //m_pPlayerIcon->GetComponent<CTransform>()->SetLocalScale(Vector3(7.0f, 10.0f, 0.0f));
    //m_pPlayerIcon->GetComponent<CTransform>()->SetLocalPosition(Vector3(-400.0f, 330.0f, 0.0f));
    //m_pPlayerIcon->SetZOrder(1);
//    SetName(mapTextureName);
//
//    UINT screenWidth = core.GetWidth();
//    UINT screenHeight = core.GetHeight();
//
//    // Load texture
//    m_pTexture = CResourceManager::Find<CTexture>(mapTextureName);
//    CTexture::TextureSize textureSize = m_pTexture->GetTextureSize();
//    float miniMapWidth = textureSize.width;
//    float miniMapHeight = textureSize.height;
//
//    Vector3 localPos(
//        -static_cast<float>(screenWidth) / 2.0f + miniMapWidth / 2.0f,
//        static_cast<float>(screenHeight) / 2.0f - miniMapHeight,
//        0.0f
//    );
//
//    // Background
//    m_tElements.Background = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//    m_tElements.Background->SetName(L"MiniMap_Background");
//    auto bgTr = m_tElements.Background->AddComponent<CTransform>();
//    bgTr->SetLocalPosition(localPos);
//    bgTr->SetLocalScale(Vector3(miniMapWidth, miniMapHeight, 0.0f));
//    m_tElements.Background->AddComponent<CSpriteRenderer>()->SetTexture(m_pTexture);
//
//    // Frame
//    m_tElements.Frame = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//    m_tElements.Frame->SetName(L"MiniMap_Frame");
//    auto frTr = m_tElements.Frame->AddComponent<CTransform>();
//    frTr->SetLocalPosition(localPos);
//    frTr->SetLocalScale(Vector3(miniMapWidth + 10.0f, miniMapHeight + 10.0f, 0.0f));
//    m_tElements.Frame->AddComponent<CSpriteRenderer>()->SetTexture(CResourceManager::Find<CTexture>(L"Minimap_Frame"));
//
//    // Player icon
//    m_tElements.PlayerIcon = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//    m_tElements.PlayerIcon->SetName(L"MiniMap_Player");
//    auto plTr = m_tElements.PlayerIcon->AddComponent<CTransform>();
//    plTr->SetLocalScale(Vector3(7.0f, 10.0f, 0.0f));
//    m_tElements.PlayerIcon->AddComponent<CSpriteRenderer>()->SetTexture(
//        CResourceManager::Find<CTexture>(L"MiniMap_Player")
//    );
//
//    // Load real world map size
//    CTexture* worldTex = CResourceManager::Find<CTexture>(L"Lake of Oblivion_2");
//    m_vRealMapScale = Vector3(
//        static_cast<float>(worldTex->GetTextureSize().width),
//        static_cast<float>(worldTex->GetTextureSize().height),
//        0.0f
//    );
//
//    // Optional: Map name
//    /*m_tElements.MapName = Instantiate<CTextUI>(LAYER_TYPE::LT_UI);
//    m_tElements.MapName->SetText(L"Lake of Oblivion", 14.0f, D2D1::ColorF::White);
//    m_tElements.MapName->GetComponent<CTransform>()->SetLocalPosition(
//        Vector3(localPos.x, localPos.y + miniMapHeight / 2.0f + 15.0f, 0.0f)
//    );*/
//}
//
//void CMiniMap::UpdateMiniMapPlayer()
//{
//    CGameObject* pPlayer = CSceneManager::GetCurScene()->GetPlayer();
//    Vector3 playerPos = pPlayer->GetComponent<CTransform>()->GetWorldPosition();
//
//    Vector3 miniMapSize = m_tElements.Background->GetComponent<CTransform>()->GetLocalScale();
//    Vector3 miniMapPos = m_tElements.Background->GetComponent<CTransform>()->GetWorldPosition();
//    Vector3 topLeft = miniMapPos - Vector3(miniMapSize.x / 2.0f, miniMapSize.y / 2.0f, 0.0f);
//
//    float normX = playerPos.x / m_vRealMapScale.x;
//    float normY = playerPos.y / m_vRealMapScale.y;
//
//    float iconX = normX * miniMapSize.x;
//    float iconY = normY * miniMapSize.y;
//
//    Vector3 finalPos = topLeft + Vector3(iconX, iconY, 0.0f);
//    m_tElements.PlayerIcon->GetComponent<CTransform>()->SetWorldPosition(finalPos);
//}