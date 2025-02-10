#include "CPlayerScript.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"
#include "../Object/CPlayer.h"
#include "../Component/CAnimator.h"
#include "../Component/CTransform.h"
#include "../Component/CRigidBody.h"
#include "../Component/CRenderer.h"
#include "../Component/CPixelCollider.h"

CPlayerScript::CPlayerScript() :
	m_ePlayerState(PLAYER_STATE::PS_Idle),
	m_iDir(1),
	m_pAnimator(nullptr),
	m_pPixelCollider(nullptr)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Init()
{
}

void CPlayerScript::Update()
{
	CheckPixelColor();

	CGameObject* pPlayer = renderer::selectedObject;
	if (pPlayer != nullptr)
	{
		CAnimator* animator = pPlayer->GetComponent<CAnimator>();
	}

	switch (m_ePlayerState)
	{
	case PLAYER_STATE::PS_Idle:
		idle();  // Idle 애니메이션을 설정
		break;
	case PLAYER_STATE::PS_Walk:
		move();  // 걷기 애니메이션을 설정
		break;
	case PLAYER_STATE::PS_Attack:
		attack();
		break;
	case PLAYER_STATE::PS_Prone:
		prone();
		break;
	case PLAYER_STATE::PS_Jump:
		jump();
		break;
		// 필요에 따라 다른 상태 추가
	}
}

void CPlayerScript::LateUpdate()
{
}

void CPlayerScript::Render()
{
}

void CPlayerScript::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayerScript::OnCollisionStay(CCollider* _pOther)
{
}

void CPlayerScript::OnCollisionExit(CCollider* _pOther)
{
}

vector<Vector3> CPlayerScript::GetCollisionPoints(const Vector3& _vPos, int _iPlayerWidthHalf, int _iPlayerHeightHalf)
{
	vector<Vector3> collisionPoints;       // 충돌 좌표를 저장
	int offsetY = _iPlayerHeightHalf;

	// 현재 위치의 정수 좌표로 계산
	int x = static_cast<int>(_vPos.x);
	int y = static_cast<int>(_vPos.y);

	// 좌우 충돌 검사 좌표를 계산
	int xLeft = x - _iPlayerWidthHalf;
	int xRight = x + _iPlayerWidthHalf;

	// 상하 충돌 검사 좌표를 계산
	int yTop = y - 10.f;
	int yBottom = y + _iPlayerHeightHalf;

	// 충돌좌표에 벡터를 추가
	collisionPoints.emplace_back(Vector3(x, y, 0.0f));
	collisionPoints.emplace_back(Vector3(xLeft, y, 0.0f));
	collisionPoints.emplace_back(Vector3(xRight, y, 0.0f));
	collisionPoints.emplace_back(Vector3(x, yTop, 0.0f));

	return collisionPoints;
}

bool CPlayerScript::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
{
	//// 좌표가 픽셀 충돌 객체의 범위 내에 있는지 확인
	//if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() && _iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {
	//	_pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

	//	// 충돌 태그에 따라 충돌 여부를 판정
	//	if (_colTag == "StageColl") {
	//		return (_pPixel.r == 255 && _pPixel.g == 0 && _pPixel.b == 255);
	//	}
	//	else if (_colTag == "StageColl") {
	//		return (_pPixel.r == 255 && _pPixel.g == 255 && _pPixel.b == 255);
	//	}
	//	/*
	//	else if (_colTag == "Wall") {
	//		return (_pPixel.r == 0 && _pPixel.g == 255 && _pPixel.b == 255);
	//	}
	//	else if (_colTag == "Rope") {
	//		return (_pPixel.r == 0 && _pPixel.g == 128 && _pPixel.b == 0);
	//	}*/
	//}
	//else {
	//	OutputDebugStringA("Player position out of bounds.\n");
	//}
	//return false;
	// 중심 기준으로 좌표 변환

	if (!m_pPixelCollider) {
		OutputDebugStringA("Pixel collider not set.\n");
		return false;
	}

	if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() &&
		_iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {

		// 픽셀 데이터 가져오기
		_pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

		// 디버그 출력
		char debugMsg[200];
		sprintf_s(debugMsg, "Pixel Collision Debug - Pos(%d, %d), Color(R=%d, G=%d, B=%d)\n",
			_iPosX, _iPosY, _pPixel.r, _pPixel.g, _pPixel.b);
		OutputDebugStringA(debugMsg);

		if (_colTag == "StageColl") {
			return (_pPixel.r == 255 && _pPixel.g == 0 && _pPixel.b == 255);
		}
	}
	OutputDebugStringA("Pixel position out of bounds or no collision.\n");
	return false;

	//// 중심 기준으로 좌표 변환
	//int centerX = m_pPixelCollider->GetWidth() / 2;
	//int centerY = m_pPixelCollider->GetHeight() / 2;

	//int localX = _iPosX + centerX;  // 정중앙을 기준으로 변환
	//int localY = centerY - _iPosY;  // Y축 방향 반전 및 정중앙 기준 변환

	//if (localX >= 0 && localX < m_pPixelCollider->GetWidth() &&
	//	localY >= 0 && localY < m_pPixelCollider->GetHeight()) {

	//	// 픽셀 데이터 가져오기
	//	_pPixel = m_pPixelCollider->GetPixelColor(localX, localY);

	//	// 디버그 출력
	//	char debugMsg[200];
	//	sprintf_s(debugMsg, "Pixel Collision Debug - LocalPos(%d, %d), Color(R=%d, G=%d, B=%d)\n",
	//		localX, localY, _pPixel.r, _pPixel.g, _pPixel.b);
	//	OutputDebugStringA(debugMsg);

	//	if (_colTag == "StageColl") {
	//		return (_pPixel.r == 255 && _pPixel.g == 0 && _pPixel.b == 255);
	//	}
	//}

	//OutputDebugStringA("Pixel position out of bounds or no collision.\n");
	//return false;
}

void CPlayerScript::UpdateCollisionState(bool& _bIsColiding, bool _bCollisionDetected, const string& _strColTag, void(CPlayerScript::* onEnter)(), void(CPlayerScript::* onExit)())
{
	if (_bCollisionDetected) {
		if (!_bIsColiding) {
			_bIsColiding = true;
			(this->*onEnter)();
		}
	}
	else {
		if (_bIsColiding) {
			_bIsColiding = false;
			(this->*onExit)();
		}
	}
}

void CPlayerScript::CheckPixelColor()
{
	// 스테이지 충돌 및 벽, 로프 충돌 상태를 나타내는 정적 변수
	static bool isCollidingWithStage = false;
	static bool isCollidingWithWall = false;
	static bool isCollidingWithRope = false;

	// 픽셀 충돌 객체가 존재한다면
	if (m_pPixelCollider) {
		CTransform* tr = GetOwner()->GetComponent<CTransform>();
		Vector3 vPos = tr->GetPosition();

		int playerWidthHalf = 17;
		int playerHeightHalf = 32;

		auto collisionPoints = GetCollisionPoints(vPos, playerWidthHalf, playerHeightHalf);
		m_vecCollisionPoint.clear(); // 충돌 위치 벡터 초기화

		// 충돌 감지 플래그 초기화
		bool stageCollisionDetected = false;
		bool wallCollisionDetected = false;
		bool ropeCollisionDetected = false;

		PIXEL pixel;

		// 스테이지 충돌 검사
		const auto& stagePoint = collisionPoints[0];
		if (CheckPixelCollision(stagePoint.x, stagePoint.y, pixel, "StageColl")) {
			m_vecCollisionPoint.emplace_back(stagePoint);
			stageCollisionDetected = true;

			// 픽셀 색상 디버깅
			char debugMsg[100];
			sprintf_s(debugMsg, "Pixel Collider Color - R: %d, G: %d, B: %d\n", pixel.r, pixel.g, pixel.b);
			OutputDebugStringA(debugMsg);

			CCollider* collider = GetOwner()->GetComponent<CCollider>();
			if (collider) {
				collider->SetCollisionDetected(true);
			}
		}
		else {
			CCollider* collider = GetOwner()->GetComponent<CCollider>();
			if (collider) {
				collider->SetCollisionDetected(false);
			}
		}

		//// 벽 충돌 검사
		//for (size_t i = 1; i <= 2; ++i) {
		//	const auto& point = collisionPoints[i];
		//	if (CheckPixelCollision(point.x, point.y, pixel, "Wall")) {
		//		m_CollisionPoint.emplace_back(point);
		//		wallCollisionDetected = true;
		//	}
		//}

		//// 로프 충돌 검사
		//const auto& ropePoint = collisionPoints.back();
		//int ropeCollisionRadius = 10; // 로프 충돌 범위 반경

		//for (int dx = -ropeCollisionRadius; dx <= ropeCollisionRadius; dx += 1) { // X축 방향으로 좌표 검사
		//	if (CheckPixelCollision(ropePoint.x + dx, ropePoint.y, pixel, "Rope")) {
		//		m_CollisionPoint.emplace_back(Vec2(ropePoint.x + dx, ropePoint.y));
		//		ropeCollisionDetected = true;
		//		m_vRopePos = Vec2(ropePoint.x + dx, ropePoint.y); // 로프 위치 업데이트
		//	}
		//}

		UpdateCollisionState(isCollidingWithStage, stageCollisionDetected, "StageColl", &CPlayerScript::OnStageCollisionEnter, &CPlayerScript::OnStageCollisionExit);
		//UpdateCollisionState(isCollidingWithWall, wallCollisionDetected, "Wall", &CPlayer::OnWallCollisionEnter, &CPlayer::OnWallCollisionExit);
		//UpdateCollisionState(isCollidingWithRope, ropeCollisionDetected, "Rope", &CPlayer::OnRopeCollisionEnter, &CPlayer::OnRopeCollisionExit);
	}
	else {
		OutputDebugStringA("Pixel collider not set.\n");
	}
}

void CPlayerScript::OnStageCollisionEnter() {
	CCollider tempCollider;
	tempCollider.SetColTag("StageColl");
	OnStageCollisionEnter(&tempCollider);
}

void CPlayerScript::OnStageCollisionExit() {
	CCollider tempCollider;
	tempCollider.SetColTag("StageColl");
	OnStageCollisionExit(&tempCollider);
}

void CPlayerScript::OnStageCollisionEnter(CCollider* _pOther)
{
	if (_pOther->GetColTag() == "StageColl")
	{
	}
}

void CPlayerScript::OnStageCollision(CCollider* _pOther)
{
	if (_pOther->GetColTag() == "StageColl")
	{
	}
}

void CPlayerScript::OnStageCollisionExit(CCollider* _pOther)
{
	if (_pOther->GetColTag() == "StageColl")
	{
	}
}

void CPlayerScript::idle()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Idle";
	wstring filePathPattern = L"../Resources/Texture/Player/Idle/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 3;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생

	if (KEY_HOLD(KEY_CODE::LEFT) || KEY_TAP(KEY_CODE::LEFT) ||
		KEY_HOLD(KEY_CODE::RIGHT) || KEY_TAP(KEY_CODE::RIGHT))
	{
		m_ePlayerState = PLAYER_STATE::PS_Walk;
	}

	if (KEY_HOLD(KEY_CODE::DOWN) || KEY_TAP(KEY_CODE::DOWN))
	{
		m_ePlayerState = PLAYER_STATE::PS_Prone;
	}

	if (KEY_HOLD(KEY_CODE::UP) || KEY_TAP(KEY_CODE::UP))
	{
		m_ePlayerState = PLAYER_STATE::PS_Jump;
	}

	if (KEY_HOLD(KEY_CODE::X) || KEY_TAP(KEY_CODE::X))
	{
		m_ePlayerState = PLAYER_STATE::PS_Attack;
	}
}

void CPlayerScript::move()
{
	CRigidBody* rb = GetOwner()->GetComponent<CRigidBody>();
	rb->ClearForce();

	if (KEY_HOLD(KEY_CODE::RIGHT))
	{
		rb->AddForce(Vector2(100.0f, 0.0f));
	}
	if (KEY_HOLD(KEY_CODE::LEFT))
	{
		rb->AddForce(Vector2(-100.0f, 0.0f));
	}
	if (KEY_HOLD(KEY_CODE::UP))
	{
		rb->AddForce(Vector2(0.0f, 100.0f));
	}
	if (KEY_HOLD(KEY_CODE::DOWN))
	{
		rb->AddForce(Vector2(0.0f, -100.0f));
	}

	if (KEY_AWAY(KEY_CODE::RIGHT) || KEY_AWAY(KEY_CODE::LEFT) ||
		KEY_AWAY(KEY_CODE::UP) || KEY_AWAY(KEY_CODE::DOWN))
	{
		rb->ClearForce();
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}


	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring animationName = L"Walk";
	wstring filePathPattern = L"../Resources/Texture/Player/Walk/Right/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 4;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생
}

void CPlayerScript::prone()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Prone";
	wstring filePathPattern = L"../Resources/Texture/Player/Prone/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 1;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		80.0f,                 // 각 프레임의 너비
		37.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생

	if(KEY_AWAY(KEY_CODE::DOWN))
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}
}

void CPlayerScript::jump()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle 애니메이션 생성
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Jump";
	wstring filePathPattern = L"../Resources/Texture/Player/Jump/" + direction + L"/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 1;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		54.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, true);  // 반복 재생

	if (KEY_AWAY(KEY_CODE::UP))
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;
	}
}

void CPlayerScript::attack()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer 가져오기
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Attack 애니메이션 생성
	wstring animationName = L"Attack";
	wstring filePathPattern = L"../Resources/Texture/Player/Attack/Right/%d.bmp";  // %d로 프레임 번호 변경
	int frameCount = 3;  // 애니메이션 프레임 수 (예: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // 프레임 간 지속 시간 (0.5초로 설정)

	// 애니메이션 추가
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // 왼쪽 상단 좌표
		55.0f,                 // 각 프레임의 너비
		65.0f,                 // 각 프레임의 높이
		0.0f,                  // 프레임 간 x 오프셋
		0.0f,                  // 프레임 간 y 오프셋
		frameDuration         // 프레임 지속 시간
	);

	// 애니메이션 실행 (첫 번째 프레임 강제 설정하지 않음)
	animator->Play(animationName, false);  // 반복되지 않도록 false 설정

	// 애니메이션이 끝났는지 체크하고, 끝났으면 Idle 상태로 변경
	if (animator->End())  // 애니메이션이 끝났다면
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;  // Idle 상태로 변경
	}
}