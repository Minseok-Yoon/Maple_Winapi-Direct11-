#include "CRectDrawScene.h"
#include "../Object/CGameObject.h"
#include "../Component/CLineRenderer.h"

CRectDrawScene::CRectDrawScene() :
    m_pLineRenderer(nullptr)
{
}

CRectDrawScene::~CRectDrawScene()
{
    delete m_pLineRenderer;
}

void CRectDrawScene::Enter()
{
	CScene::Enter();
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

    // 사각형의 네 꼭짓점 (예시)
    std::vector<Vector3> points = {
        Vector3(0.0f, 0.0f, 0.0f),        // 왼쪽 아래
        Vector3(1.0f, 0.0f, 0.0f),        // 오른쪽 아래
        Vector3(1.0f, 1.0f, 0.0f),        // 오른쪽 위
        Vector3(0.0f, 1.0f, 0.0f),        // 왼쪽 위
        Vector3(0.0f, 0.0f, 0.0f)         // 다시 왼쪽 아래로 돌아옴 (닫힌 선)
    };

    // 선 색상 (예: 빨간색)
    Vector4 color(0.0f, 255.0f, 0.0f, 100.0f);

    // 선 렌더러에 사각형 데이터 전달
    m_pLineRenderer = new CLineRenderer();

    m_pLineRenderer->Init();
    m_pLineRenderer->SetLineData(points, color);

    // 선 렌더링
    m_pLineRenderer->Render();

    // 색상 값 OutputDebugString으로 출력
    char colorMessage[100];
    sprintf_s(colorMessage, "Line Color: R=%.2f, G=%.2f, B=%.2f, A=%.2f\n", color.x, color.y, color.z, color.w);
    OutputDebugStringA(colorMessage);  // 색상 값 출력
}