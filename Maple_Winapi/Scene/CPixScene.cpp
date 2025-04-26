#include "../pch.h"
#include "CPixScene.h"
#include "../Object/CBackGround.h"
#include "../Object/CObject.h"
#include "../Component/CPixelCollider.h"
#include "../Graphics/CGraphicsDevice_DX11.h"
#include "../Component/CRenderer.h"
#include "../Component/CCameraScript.h"
#include "../Object/CPlayer.h"
#include "../Object/CPlayerScript.h"
#include "../Object/CMonster.h"
#include "../Object/CMonsterScript.h"
#include "../Component/CAnimator.h"
#include "../Component/CRigidBody.h"
#include "../Manager/CCollisionManager.h"
#include "../Core/CCore.h"
#include "../Test/CDrawCollisionBox.h"
#include "../Component/CLineRenderer.h"
#include "../Component/CLineCollider.h"
#include "../Object/CLineBackGround.h"

extern CCore core;

CPixScene::CPixScene()
{
	CResourceManager::Load<CTexture>(L"Lake of Oblivion_1", L"../Resources/Texture/Lake of Oblivion_1.png");
}

CPixScene::~CPixScene()
{
}

void CPixScene::Enter()
{
	CScene::Enter();
}

void CPixScene::Enter(const wstring& _strBackGroundName, const wstring& _strAudioName)
{
}

void CPixScene::Exit()
{
	CScene::Exit();
}

void CPixScene::Init()
{
	CScene::Init();

	// 카메라 설정
	CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
	camera->SetName(L"MainCamera");
	CCamera* cameraComp = camera->AddComponent<CCamera>();
	cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);

	CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
	renderer::mainCamera = cameraComp;
	renderer::activeCamera = renderer::mainCamera;

	CreateTestRect(0.0f);
}

void CPixScene::Update()
{
	CScene::Update();
}

void CPixScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CPixScene::Render()
{
	CScene::Render();
}

void CPixScene::CreateTestRect(float zValue)
{
	// 백그라운드 계층에 객체 생성
	CGameObject* testRect = Instantiate<CGameObject>(LAYER_TYPE::LT_BackGround);
	testRect->SetName(L"Lake of Oblivion_1");

	UINT iWidth = core.GetWidth();
	UINT iHeight = core.GetHeight();

	// Transform 설정
	CTransform* tr = testRect->AddComponent<CTransform>();
	tr->SetLocalPosition(Vector3(0.f, 0.f, zValue));            // Z 깊이 테스트용
	tr->SetLocalScale(Vector3(4780.f, 1452.f, 1.0f));              // 보기 좋게 크게

	// 스프라이트 렌더러 설정
	CSpriteRenderer* spriteRenderer = testRect->AddComponent<CSpriteRenderer>();

	// 텍스처 설정
	CTexture* tex = CResourceManager::Find<CTexture>(L"Lake of Oblivion_1");
	if (tex)
	{
		spriteRenderer->SetTexture(tex);
	}
}

// 새로운 메서드 추가: 배경 선분 생성
void CPixScene::CreateBackgroundLines()
{
	//// 라인 배경 객체 생성
	//CLineBackGround* lineBackground = Instantiate<CLineBackGround>(LAYER_TYPE::LT_BackGround);

	//// 기본 바닥 선분 생성 (호리즌탈 라인)
	//lineBackground->CreateHorizontalLine(-3.0f, -5.0f, 5.0f, Vector4(0.0f, 1.0f, 0.0f, 1.0f));  // 녹색 바닥

	//// 플랫폼 선분 생성
	//lineBackground->CreateHorizontalLine(0.0f, -3.0f, 2.0f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));   // 파란색 플랫폼
	//lineBackground->CreateHorizontalLine(3.0f, -2.0f, 3.0f, Vector4(1.0f, 0.0f, 0.0f, 1.0f));   // 빨간색 플랫폼

	//// 수직 기둥 생성
	//lineBackground->CreateVerticalLine(-3.0f, -3.0f, 0.0f, Vector4(1.0f, 1.0f, 0.0f, 1.0f));    // 노란색 기둥
}

// 몬스터 생성
	/*CGameObject* pMonster = Instantiate<CMonster>(LAYER_TYPE::LT_Monster, Vector3(0.0f, 0.0f, 0.0f));

	pMonster->AddComponent<CMonsterScript>();
	CMonsterScript* monSr = pMonster->GetComponent<CMonsterScript>();*/

	// 여기서의 Transform은 오브젝트의 위치가 아닌 오브젝트의 컴포넌트 위치 같다
	// 여기서 위치를 수정하면 오브젝트의 위치는 가만히 있는데 충돌박스의 위치가 달라짐

//void CPixScene::Init()
//{
//	CScene::Init();
//
//	// 카메라 설정
//	CGameObject* camera = Instantiate<CGameObject>(LAYER_TYPE::LT_None, Vector3(0.0f, 0.0f, -10.0f));
//	CCamera* cameraComp = camera->AddComponent<CCamera>();
//	cameraComp->SetProjectionType(CCamera::PROJECTION_TYPE::PT_Orthographic);
//
//	CCameraScript* cameraScript = camera->AddComponent<CCameraScript>();
//	renderer::mainCamera = cameraComp;
//
//	// 플레이어 생성
//	// Instantiate의 vector로 캐릭터의 위치가 변경이 되지 않음... 왜?
//	m_pPlayer = Instantiate<CPlayer>(LAYER_TYPE::LT_Player, Vector3(0.0f, 0.0f, 0.0f));
//
//	m_pPlayer->AddComponent<CPlayerScript>();
//	CPlayerScript* plSr = m_pPlayer->GetComponent<CPlayerScript>();
//	renderer::selectedObject = m_pPlayer;
//
//	// 배경 선분 오브젝트 생성 시
//	m_pLine = Instantiate<CGameObject>(LAYER_TYPE::LT_BackGround);
//	m_pLine->SetName(L"Line");
//	CTransform* lineTransform = m_pLine->GetComponent<CTransform>();
//	lineTransform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f)); // 원하는 위치로 설정
//	lineTransform->SetParent(nullptr); // 명시적으로 부모 없음 설정
//
//	// 라인 렌더러 추가
//	CLineRenderer* lineRenderer = m_pLine->AddComponent<CLineRenderer>();
//	lineRenderer->SetFollowParent(true); // 부모(즉, 자신의 GameObject)를 따르도록 설정
//	lineRenderer->Init();
//
//	// 로컬 좌표로 점 지정 (월드 공간 좌표가 아님)
//	std::vector<Vector3> linePoints = {
//		Vector3(0.0f, 0.0f, 0.0f),  // 시작점
//		Vector3(5.0f, 0.0f, 0.0f)   // 끝점
//	};
//	Vector4 lineColor(0.0f, 1.0f, 0.0f, 1.0f);  // 녹색 선
//	lineRenderer->SetLineData(linePoints, lineColor);
//
//	// 선분 충돌체 추가
//	CLineCollider* lineCollider = m_pLine->AddComponent<CLineCollider>();
//	lineCollider->SetLine(Vector2(-5.0f, 0.0f), Vector2(5.0f, 0.0f), 0.1f);
//
//	// 충돌 레이어 설정
//	CColliderManager::CollisionLayerCheck(LAYER_TYPE::LT_Player, LAYER_TYPE::LT_BackGround, true);
//}