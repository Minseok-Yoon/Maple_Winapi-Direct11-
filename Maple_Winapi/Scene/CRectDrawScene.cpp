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

    // �簢���� �� ������ (����)
    std::vector<Vector3> points = {
        Vector3(0.0f, 0.0f, 0.0f),        // ���� �Ʒ�
        Vector3(1.0f, 0.0f, 0.0f),        // ������ �Ʒ�
        Vector3(1.0f, 1.0f, 0.0f),        // ������ ��
        Vector3(0.0f, 1.0f, 0.0f),        // ���� ��
        Vector3(0.0f, 0.0f, 0.0f)         // �ٽ� ���� �Ʒ��� ���ƿ� (���� ��)
    };

    // �� ���� (��: ������)
    Vector4 color(0.0f, 255.0f, 0.0f, 100.0f);

    // �� �������� �簢�� ������ ����
    m_pLineRenderer = new CLineRenderer();

    m_pLineRenderer->Init();
    m_pLineRenderer->SetLineData(points, color);

    // �� ������
    m_pLineRenderer->Render();

    // ���� �� OutputDebugString���� ���
    char colorMessage[100];
    sprintf_s(colorMessage, "Line Color: R=%.2f, G=%.2f, B=%.2f, A=%.2f\n", color.x, color.y, color.z, color.w);
    OutputDebugStringA(colorMessage);  // ���� �� ���
}