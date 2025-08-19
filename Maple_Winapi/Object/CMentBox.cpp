#include "../pch.h"
#include "../Object/CObject.h"
#include "CMentBox.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CFontManager.h"
#include "../Component/CUITransform.h"
#include "../Component/CSpriteRenderer.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CUIManager.h"
#include "../Manager/CQuestManager.h"
#include "../Manager/CSceneManager.h"
#include "../Manager/CResourceManager.h"
#include "../Quest/CQuest.h"
#include "../Object/CPlayer.h"
#include "../Core/CCore.h"
#include "CUIText.h"
#include "CBtnUI.h"

extern CCore core;

CMentBox::CMentBox() :
	CUI(UI_TYPE::UT_MentBox)
{
	CTexture* pMentBox = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
	CTexture* pBtnOK = CResourceManager::Load<CTexture>(L"BtnOK", L"../Resources/Texture/NPC/Quest_BtOK.png");

	float screenWidth = (float)core.GetWidth();
	float screenHeight = (float)core.GetHeight();
	SetUILocalPos(Vector2(screenWidth * 0.5f, screenHeight * 0.5f));// 이렇게 구현했을 때 현재 오른쪽 위에 존재한다는 것은 지금 스크린 좌표가 아닌 월드 좌표라는 점.
	SetUILocalScale(Vector2(600.0f, 200.0f));
}

CMentBox::~CMentBox()
{
}

void CMentBox::OnInit()
{
	CUI::OnInit();

	// 디버깅: Transform 설정 후 값 확인
	Vector2 setPos = GetUILocalPos();
	Vector3 worldPos = GetTransform()->GetWorldPosition();
	char dbg[128];
	sprintf_s(dbg, "Width: %d, Height: %d\n", core.GetWidth(), core.GetHeight());
	OutputDebugStringA(dbg);

	char buf[128];
	sprintf_s(buf, "Actual LocalPos: (%.2f, %.2f)\n", setPos.x, setPos.y);
	OutputDebugStringA(buf);
	sprintf_s(buf, "Actual WorldPos: (%.2f, %.2f)\n", worldPos.x, worldPos.y);
	OutputDebugStringA(buf);

	// SpriteRenderer 세팅
	CSpriteRenderer* renderer = GetComponent<CSpriteRenderer>();
	if (!renderer)
		renderer = AddComponent<CSpriteRenderer>();

	// 텍스처 로드
	CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
	if (!pMentBoxTex)
		pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");

	if (pMentBoxTex)
	{
		renderer->SetTexture(pMentBoxTex);
		renderer->SetScreenSpace(true);
	}
	else
	{
		OutputDebugStringA("ERROR: MentBox texture could not be loaded\n");
	}

	renderer->Init();

	// 콜라이더 (선택사항)
	if (!m_pMentBoxCol) {
		m_pMentBoxCol = AddComponent<CCollider>();
		if (m_pMentBoxCol) {
			m_pMentBoxCol->SetBottomLeft(Vector3(-0.5f, -0.5f, -1.0f));
			m_pMentBoxCol->SetTopRight(Vector3(0.5f, 0.5f, -1.0f));
		}
	}
	
	// 2025-07-17
	// 텍스트 UI 자식 생성
	if (CUIManager::GetUI(UI_TYPE::UT_Text) == nullptr)
	{
		m_pTextUI = Instantiate<CUIText>(LAYER_TYPE::LT_UI);
		m_pTextUI->SetRelativePos(Vector2(-200.0f, -10.0f));
		m_pTextUI->Init();

		AddChild(m_pTextUI);
		CUIManager::RegisterUI(UI_TYPE::UT_Text, m_pTextUI);
		CUIManager::OnComplete(m_pTextUI);
	}

	if (CUIManager::GetUI(UI_TYPE::UT_Button) == nullptr)
	{
		m_pBtnUI = Instantiate<CBtnUI>(LAYER_TYPE::LT_UI);

		AddChild(m_pBtnUI); // 자식으로 먼저 추가해야 transform 상속이 제대로 작동함

		// Transform 및 위치 설정은 AddChild 후에 해야 반영됨
		m_pBtnUI->SetRelativePos(Vector2(200.0f, -60.0f));
		//m_pBtnUI->GetTransform()->SetWorldPosition(Vector3(0.0f, 0.0f, -0.5f));
		m_pBtnUI->Init();

		// 버튼에 SpriteRenderer 설정
		CSpriteRenderer* BtnSr = m_pBtnUI->GetComponent<CSpriteRenderer>();
		if (!BtnSr)
			BtnSr = m_pBtnUI->AddComponent<CSpriteRenderer>();

		CTexture* pBtnTex = CResourceManager::Find<CTexture>(L"BtnOK");
		if (!pBtnTex)
			pBtnTex = CResourceManager::Load<CTexture>(L"BtnOK", L"../Resources/Texture/NPC/Quest_BtOK.png");

		if (pBtnTex)
		{
			BtnSr->SetTexture(pBtnTex);
			BtnSr->SetScreenSpace(true);
		}
		else
		{
			OutputDebugStringA("ERROR: BtnOK texture could not be loaded\n");
		}

		BtnSr->Init();
		m_pBtnUI->SetActive(true);

		CUIManager::RegisterUI(UI_TYPE::UT_Button, m_pBtnUI);
		CUIManager::OnComplete(m_pBtnUI);

		/*m_pBtnUI->SetButtonClickEndEvent([&]()
		{
			OnInActive();
		});*/
	}
}

void CMentBox::OnActive()
{
	SetActive(true);
}

void CMentBox::OnInActive()
{
	SetActive(false);
}

void CMentBox::OnUpdate()
{
	Vector3 setPos = GetTransform()->GetLocalPosition();
	GetTransform()->SetLocalPosition(Vector3(setPos.x, setPos.y, 0.0f));

	if (m_charIndex < m_strFullText.length())
	{
		m_fAccTime += CTimeManager::GetfDeltaTime();
		if (m_fAccTime >= m_fTypingSpeed)
		{
			m_strCurrentText += m_strFullText[m_charIndex++];
			m_fAccTime = 0.0f;

			if (m_pTextUI)
				m_pTextUI->SetText(m_strCurrentText);
		}
	}

	if (KEY_TAP(KEY_CODE::SPACE)) OnAcceptQuest();
	if (KEY_TAP(KEY_CODE::ESC)) CUIManager::Pop(UI_TYPE::UT_MentBox);

	// 텍스트 출력 처리
	//if (m_charIndex < m_strFullText.length())
	//{
	//	m_fAccTime += CTimeManager::GetfDeltaTime();

	//	if (m_fAccTime >= m_fTypingSpeed)
	//	{
	//		m_strCurrentText += m_strFullText[m_charIndex];
	//		m_charIndex++;
	//		m_fAccTime = 0.0f;
	//	}
	//}
	//else if (KEY_TAP(KEY_CODE::SPACE))
	//{
	//	OutputDebugStringA("퀘스트 space바 누름\n");
	//	OnAcceptQuest();
	//}

	//// ESC 눌렀을 때 UI 닫기
	//if (KEY_TAP(KEY_CODE::ESC))
	//{
	//	if (CUIManager::IsActive(UI_TYPE::UT_MentBox))
	//	{
	//		CUIManager::Pop(UI_TYPE::UT_MentBox);
	//	}
	//}
}

void CMentBox::OnLateUpdate()
{
}

void CMentBox::OnRender()
{
	CUI::OnRender();

	ID3D11DeviceContext* context = GetDeviceContext();
	if (!context) return;

	// Scissor 영역 설정
	D3D11_RECT scissor;
	Vector3 pos = GetTransform()->GetWorldPosition();
	Vector3 size = GetTransform()->GetLocalScale();

	scissor.left = static_cast<LONG>(pos.x);
	scissor.top = static_cast<LONG>(pos.y);
	scissor.right = static_cast<LONG>(pos.x + size.x);
	scissor.bottom = static_cast<LONG>(pos.y + size.y);

	context->RSSetScissorRects(1, &scissor);

	// 자식 요소 렌더
	for (CUI* child : GetChildren())
	{
		if (child && child->IsEnable())
			child->Render();
	}
}

void CMentBox::OnClear() 
{
}

void CMentBox::SetText(const wstring& _strText)
{
	if (m_pTextUI)
	{
		m_pTextUI->SetFullText(_strText); // 직접 전달
	}
}

void CMentBox::OnAcceptQuest()
{
	if (m_pQuest)
	{
		CQuestManager::GetInst()->AcceptQuest(m_pQuest);
		OutputDebugStringW((L"수락한 퀘스트: " + m_pQuest->GetTitle() + L"\n").c_str());
		if (CUIManager::IsActive(UI_TYPE::UT_MentBox))
		{
			CUIManager::Pop(UI_TYPE::UT_MentBox);
		}
	}
}

// 2025-06-24
//CFontManager::GetInst()->DrawText(
		//	GetDeviceContext(),  // 디바이스 컨텍스트
		//	m_strCurrentText,                  // 출력할 텍스트
		//	100.0f, 100.0f,                    // 출력 위치
		//	24.0f,                             // 폰트 크기
		//	0x00000000,                        // 색상 (흰색)
		//	L"Arial"
		//);

//// 화면 중앙에 테스트 텍스트 출력
		//CFontManager::GetInst()->DrawText(
		//	deviceContext,
		//	m_strCurrentText,
		//	-100.0f, -300.0f,  // 고정 위치
		//	15.0f,            // 큰 폰트 크기
		//	0xFFFF0000,       // 빨간색
		//	L"Arial"
		//);

	//// 텍스트 출력용 함수 호출
	//if (!m_strCurrentText.empty())
	//{
	//	Vector3 pos = GetComponent<CTransform>()->GetWorldPosition();
	//	ID3D11DeviceContext* deviceContext = GetDeviceContext();

	//	if (deviceContext)
	//	{
	//		CFontManager::GetInst()->DrawText(
	//			deviceContext,
	//			m_strCurrentText,
	//			pos.x + 20.0f, 
	//			pos.y + 20.0f,
	//			24.0f,
	//			0xFFFFFFFF,
	//			L"Arial"
	//		);
	//	}
	//}
//if (m_charIndex < m_strFullText.length())
	//{
	//	m_fAccTime += CTimeManager::GetfDeltaTime();

	//	if (m_fAccTime >= m_fTypingSpeed)
	//	{
	//		m_strCurrentText += m_strFullText[m_charIndex];
	//		m_charIndex++;
	//		m_fAccTime = 0.0f;
	//	}
	//}
	//else
	//{
	//	// 모든 텍스트 출력 후에만 스페이스바 입력 감지
	//	if (KEY_TAP(KEY_CODE::SPACE))
	//	{
	//		//OnAcceptQuest();
	//	}
	//}

	//CTransform* tr = GetComponent<CTransform>();
	//if(!tr) 
	//	return;

	//Vector3 pos = tr->GetLocalPosition();
	//Vector3 scale = tr->GetLocalScale();

	//Vector2 mousePos = CKeyManager::GetInst()->GetMousePos(); // 현재 마우스 위치
	//Vector2 mouse(mousePos.x, mousePos.y);

	//// 마우스 클릭 체크
	//if (KEY_TAP(KEY_CODE::LBTN) && KEY_HOLD(KEY_CODE::LBTN))
	//{
	//	// 마우스가 UI 범위 안에 있는지 확인
	//	if (mouse.x >= pos.x && mouse.x <= pos.x + scale.x &&
	//		mouse.y >= pos.y && mouse.y <= pos.y + scale.y)
	//	{
	//		m_bDragging = true;
	//		m_vDragOffset = Vector2(mouse.x - pos.x, mouse.y - pos.y);
	//	}
	//}

	//if (KEY_HOLD(KEY_CODE::LBTN) && m_bDragging)
	//{
	//	// 드래그 중 → 위치 갱신
	//	tr->SetLocalPosition(Vector3(mouse.x - m_vDragOffset.x,
	//		mouse.y - m_vDragOffset.y,
	//		pos.z));
	//}

	//if (KEY_AWAY(KEY_CODE::LBTN))
	//{
	//	m_bDragging = false;
	//}

// 2025-07-11
//void CMentBox::OnInit()
//{
//	CUI::OnInit();
//
//	// Transform 세팅
//	CTransform* tr = GetComponent<CTransform>();
//	if (!tr)
//		tr = AddComponent<CTransform>();
//
//	tr->SetLocalPosition(Vector3(m_vPosition.x, m_vPosition.y, 0.0f));
//	tr->SetLocalScale(Vector3(m_vSize.x, m_vSize.y, 1.0f));
//
//	// SpriteRenderer 세팅
//	CSpriteRenderer* renderer = GetComponent<CSpriteRenderer>();
//	if (!renderer)
//		renderer = AddComponent<CSpriteRenderer>();
//
//	// 텍스처 로드
//	CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//	if (!pMentBoxTex)
//		pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
//
//	if (pMentBoxTex)
//	{
//		renderer->SetTexture(pMentBoxTex);
//	}
//	else
//	{
//		OutputDebugStringA("ERROR: MentBox texture could not be loaded\n");
//	}
//
//	renderer->Init();
//
//	// 콜라이더 (선택사항)
//	if (!m_pMentBoxCol) {
//		m_pMentBoxCol = AddComponent<CCollider>();
//		if (m_pMentBoxCol) {
//			m_pMentBoxCol->SetBottomLeft(Vector3(-0.5f, -0.5f, -1.0f));
//			m_pMentBoxCol->SetTopRight(Vector3(0.5f, 0.5f, -1.0f));
//		}
//	}
//
//	// 디버그 정보
//	OutputDebugStringA("MentBox::OnInit completed - Renderer and Transform set\n");
//	//CUI::OnInit();
//
//	////CUITransform* pMentBoxTr = GetComponent<CUITransform>();
//	////if (!pMentBoxTr)
//	////	pMentBoxTr = AddComponent<CUITransform>();
//
//	////CSpriteRenderer* pMentBoxSr = GetComponent<CSpriteRenderer>();
//	////if (!pMentBoxSr)
//	////	pMentBoxSr = AddComponent<CSpriteRenderer>();
//
//	////CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//	////if(!pMentBoxTex)
//	////	pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
//
//	////if (pMentBoxTex)
//	////{
//	////	pMentBoxSr->SetTexture(pMentBoxTex);
//
//	////	// 화면 사이즈 기준 위치 지정
//	////	UINT screenWidth = core.GetWidth();
//	////	UINT screenHeight = core.GetHeight();
//
//	////	Vector2 size(300.f, 200.f);
//	////	//Vector2 position(static_cast<float>(screenWidth) * 0.5f, static_cast<float>(screenHeight) * 0.5f);
//	////	Vector2 pivot(0.5f, 0.5f); // 중심 기준
//	////	Vector2 position(1200.0f, 0.0f);
//
//	////	pMentBoxTr->SetSize(size);
//	////	pMentBoxTr->SetPivot(pivot);
//	////	pMentBoxTr->SetPosition(position);
//	////}
//
//	////Vector2 uiPos = pMentBoxTr->GetWorldPosition();
//	////wchar_t buf[128];
//	////swprintf_s(buf, 128, L"Screen Size: %dx%d, UI Pos = (%.2f, %.2f)\n",
//	////	core.GetWidth(), core.GetHeight(), uiPos.x, uiPos.y);
//	////OutputDebugStringW(buf);
//
//	////// 월드 행렬 확인
//	////Matrix worldMat = pMentBoxTr->GetWorldMatrix();
//	////wchar_t bufs[256];
//	////swprintf_s(bufs, 256, L"World Matrix: [%.2f, %.2f, %.2f, %.2f]\n",
//	////	worldMat.m[3][0], worldMat.m[3][1], worldMat.m[3][2], worldMat.m[3][3]);
//	////OutputDebugStringW(bufs);
//
//	//CTransform* parentTr = this->GetComponent<CTransform>();
//	//Vector3 basePos = parentTr->GetLocalPosition();
//
//	//Vector3 pMentPos = Vector3(0.0f, 0.0f, 0.0f);
//
//	//CGameObject* pMentBox = Instantiate<CGameObject>(LAYER_TYPE::LT_UI);
//
//	//// Transform 세팅
//	//auto slotTr = this->AddComponent<CTransform>();
//	//slotTr->SetParent(parentTr);
//	//slotTr->SetLocalPosition(pMentPos);
//	//slotTr->SetLocalScale(Vector3(600.0f, 200.0f, 1.0f));
//
//	//wchar_t buf[128];
//	//swprintf_s(buf, 128, L"Screen Size: %dx%d, UI Pos = (%.2f, %.2f)\n",
//	//	core.GetWidth(), core.GetHeight(), basePos.x, basePos.y);
//	//OutputDebugStringW(buf);
//
//	//CSpriteRenderer* pMentBoxSr = GetComponent<CSpriteRenderer>();
//	//if (!pMentBoxSr)
//	//	pMentBoxSr = AddComponent<CSpriteRenderer>();
//
//	//// 텍스처 로드
//	//CTexture* pMentBoxTex = CResourceManager::Find<CTexture>(L"NpcMent_BG");
//	//if (!pMentBoxTex)
//	//	pMentBoxTex = CResourceManager::Load<CTexture>(L"NpcMent_BG", L"../Resources/Texture/NPC/NpcMent_BG.png");
//
//	//if (pMentBoxTex)
//	//{
//	//	pMentBoxSr->SetTexture(pMentBoxTex);
//	//}
//
//	//pMentBoxSr->Init();
//
//	//if (!m_pMentBoxCol) {
//	//	m_pMentBoxCol = this->AddComponent<CCollider>();
//	//	if (m_pMentBoxCol) {
//	//		m_pMentBoxCol->SetBottomLeft(Vector3(-0.5f, -0.5f, -1.0f));
//	//		m_pMentBoxCol->SetTopRight(Vector3(0.5f, 0.5f, -1.0f));
//	//		OutputDebugStringA("INFO: Collider successfully added to CMentbox\n");
//	//	}
//	//	else {
//	//		OutputDebugStringA("ERROR: Collider creation failed in CMonster::Init()\n");
//	//	}
//	//}
//
//	//// 렌더링 상태 확인
//	//OutputDebugStringA("=== MentBox Init Debug ===\n");
//
//	//if (pMentBoxSr) {
//	//	OutputDebugStringA("SpriteRenderer exists\n");
//	//	if (pMentBoxTex) {
//	//		OutputDebugStringA("Texture loaded successfully\n");
//	//	}
//	//	else {
//	//		OutputDebugStringA("ERROR: Texture is NULL\n");
//	//	}
//	//}
//	//else {
//	//	OutputDebugStringA("ERROR: SpriteRenderer is NULL\n");
//	//}
//
//	//// 활성화 상태 확인
//	//if (IsActive()) {
//	//	OutputDebugStringA("MentBox is Active\n");
//	//}
//	//else {
//	//	OutputDebugStringA("WARNING: MentBox is not Active\n");
//	//}
//}

//void CMentBox::OnUpdate()
//{
//	//OutputDebugStringA("MentBox OnUpdate 실행 중\n");
//
//	//GetTransform()->SetLocalPosition(Vector3(m_vLocalPosition.x, m_vPosition.y, 0.f));
//
//	// 텍스트 출력 처리
//	if (m_charIndex < m_strFullText.length())
//	{
//		m_fAccTime += CTimeManager::GetfDeltaTime();
//
//		if (m_fAccTime >= m_fTypingSpeed)
//		{
//			m_strCurrentText += m_strFullText[m_charIndex];
//			m_charIndex++;
//			m_fAccTime = 0.0f;
//		}
//	}
//	else if (KEY_TAP(KEY_CODE::SPACE))
//	{
//		OutputDebugStringA("퀘스트 space바 누름\n");
//		OnAcceptQuest();
//	}
//
//	//// 드래그 처리
//	//CTransform* tr = GetComponent<CTransform>();
//	//if (!tr) return;
//
//	//Vector3 pos = tr->GetLocalPosition(); // UI 좌표 (클라이언트 기준)
//	//Vector3 scale = tr->GetLocalScale();  // UI 크기 (클라이언트 기준)
//
//	//Vector2 mousePos = CKeyManager::GetInst()->GetMousePos(); // 클라이언트 기준 좌표
//
//	//// 클릭 시 드래그 시작 판별
//	//if (KEY_TAP(KEY_CODE::LBTN))
//	//{
//	//	if (mousePos.x >= pos.x && mousePos.x <= pos.x + scale.x &&
//	//		mousePos.y >= pos.y && mousePos.y <= pos.y + scale.y)
//	//	{
//	//		m_bDragging = true;
//	//		m_vDragOffset = Vector2(mousePos.x - pos.x, mousePos.y - pos.y);
//	//		OutputDebugStringA("Drag 시작됨\n");
//	//	}
//	//}
//
//	//// 드래그 중이면 위치 이동
//	//if (KEY_HOLD(KEY_CODE::LBTN) && m_bDragging)
//	//{
//	//	Vector3 newPos(mousePos.x - m_vDragOffset.x, mousePos.y - m_vDragOffset.y, pos.z);
//	//	tr->SetLocalPosition(newPos);
//	//	//SetPos(Vector2(newPos.x, newPos.y)); // UI 동기화
//
//	//	OutputDebugStringA("드래그 중...\n");
//	//}
//
//	//// 드래그 종료
//	//if (KEY_AWAY(KEY_CODE::LBTN))
//	//{
//	//	if (m_bDragging)
//	//		OutputDebugStringA("드래그 종료됨\n");
//	//	m_bDragging = false;
//	//}
//
//	// ESC 눌렀을 때 UI 닫기
//	if (KEY_TAP(KEY_CODE::ESC))
//	{
//		if (CUIManager::IsActive(UI_TYPE::UT_MentBox))
//		{
//			CUIManager::Pop(UI_TYPE::UT_MentBox);
//		}
//	}
//}

//void CMentBox::OnRender()
//{
//	CUI::OnRender();
//
//	ID3D11DeviceContext* deviceContext = GetDeviceContext();
//	if (!deviceContext || !CFontManager::GetInst()) return;
//
//	// Step 1: 현재 Transform 위치를 가져오기
//	CTransform* tr = GetComponent<CTransform>();
//	if (!tr) return;
//
//	Vector3 worldPos = tr->GetWorldPosition();
//	Vector3 scale = tr->GetWorldScale(); // UI 크기
//
//	// Step 2: Transform 좌표 (NDC) → 스크린 픽셀 좌표 변환
//	Vector2 screenPos;
//	UINT screenWidth = core.GetWidth();
//	UINT screenHeight = core.GetHeight();
//
//	//char dbg[128];
//	//sprintf_s(dbg, "[MentBox::OnRender] MentBox Pos: %.1f, %.1f\n", m_vPosition.x, m_vPosition.y);
//	////sprintf_s(dbg, "[MentBox::OnRender] Core instance: %p\n", CCore::GetInst());
//	//OutputDebugStringA(dbg);
//
//	screenPos.x = (worldPos.x + 1.0f) * 0.5f * screenWidth;
//	screenPos.y = (1.0f - worldPos.y) * 0.5f * screenHeight;
//
//	// Step 3: 텍스트의 픽셀 좌표 계산 (패딩 20px)
//	float textX = screenPos.x + 0.0f;
//	float textY = screenPos.y + 0.0f;
//
//	// 디버깅 출력
//	/*char dbgs[256];
//	sprintf_s(dbgs, "MentBox Screen Pos: (%.2f, %.2f), Text Pos: (%.2f, %.2f)\n",
//		screenPos.x, screenPos.y, textX, textY);
//	OutputDebugStringA(dbgs);*/
//
//	// Step 4: 텍스트 렌더링
//	CFontManager::GetInst()->DrawText(
//		deviceContext,
//		m_strCurrentText,
//		30.0f,
//		textX,
//		textY,
//		0xFF0000FF,  // 파란색
//		L"Arial"
//	);
//
//	//ID3D11DeviceContext* deviceContext = GetDeviceContext();
//	//if (!deviceContext || !CFontManager::GetInst()) return;
//
//	///*CTransform* tr = GetComponent<CTransform>();
//	//if (!tr) return;
//
//	//Vector3 pos = tr->GetWorldPosition();
//	//Vector3 size = tr->GetLocalScale();*/
//
//	//Vector3 worldPos = GetTransform()->GetWorldPosition();
//	//Vector3 scale = GetTransform()->GetWorldScale();
//
//	////float offsetX = 80.0f;
//	////float offsetY = 40.0f;
//
//	////// 실제 전달되는 좌표값 확인
//	////float finalX = pos.x + offsetX;
//	////float finalY = pos.y + offsetY;
//	//float textX = worldPos.x + m_vTextOffset.x * scale.x;
//	//float textY = worldPos.y + m_vTextOffset.y * scale.y;
//
//	//// OutputDebugString용 문자열 포맷팅
//	///*wchar_t debugMsg[256];
//	//swprintf_s(debugMsg, 256, L"Box pos: (%.2f, %.2f, %.2f), Final text pos: (%.2f, %.2f)\n",
//	//	pos.x, pos.y, pos.z, finalX, finalY);
//	//OutputDebugString(debugMsg);*/
//
//	//CFontManager::GetInst()->DrawText(
//	//	deviceContext,
//	//	m_strCurrentText,
//	//	textX,
//	//	textY,  // offsetY도 적용
//	//	30.0f,
//	//	0xFFFF0000,  // 흰색
//	//	L"Arial"
//	//);
//}

// 2025-07-17
	//ID3D11DeviceContext* deviceContext = GetDeviceContext();
	//if (!deviceContext || !CFontManager::GetInst()) return;

	//// 1. Transform 가져오기
	//CTransform* tr = GetComponent<CTransform>();
	//if (!tr) return;

	//Vector3 screenPos = tr->GetLocalPosition(); // 이미 픽셀 좌표
	//Vector3 scale = tr->GetLocalScale();        // 크기도 픽셀 단위

	//// 2. 텍스트의 위치 = MentBox 좌측 상단 + 패딩
	//float paddingX = 20.0f;
	//float paddingY = 20.0f;

	//float textX = screenPos.x;
	//float textY = screenPos.y;

	//// 3. 텍스트 렌더링
	//CFontManager::GetInst()->DrawText(
	//	deviceContext,
	//	m_strCurrentText,
	//	25.0f,
	//	textX,
	//	textY,
	//	0xFF0000FF,  // 파란색
	//	L"Arial"
	//);

	//CUI::OnRender();

	//ID3D11DeviceContext* deviceContext = GetDeviceContext();
	//if (!deviceContext || !CFontManager::GetInst()) return;

	//// Step 1: 현재 Transform 위치를 가져오기
	//CTransform* tr = GetComponent<CTransform>();
	//if (!tr) return;

	//Vector3 worldPos = tr->GetWorldPosition();
	//Vector3 scale = tr->GetWorldScale(); // UI 크기

	//// Step 2: Transform 좌표 (NDC) → 스크린 픽셀 좌표 변환
	//Vector2 screenPos;
	//UINT screenWidth = core.GetWidth();
	//UINT screenHeight = core.GetHeight();

	//screenPos.x = (worldPos.x + 1.0f) * 0.5f * screenWidth;
	//screenPos.y = (1.0f - worldPos.y) * 0.5f * screenHeight;

	//// Step 3: 텍스트의 픽셀 좌표 계산 (패딩 20px)
	//float textX = screenPos.x + 0.0f;
	//float textY = screenPos.y + 0.0f;

	//// Step 4: 텍스트 렌더링
	//CFontManager::GetInst()->DrawText(
	//	deviceContext,
	//	m_strCurrentText,
	//	30.0f,
	//	textX,
	//	textY,
	//	0xFF0000FF,  // 파란색
	//	L"Arial"
	//);