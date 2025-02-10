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
		idle();  // Idle �ִϸ��̼��� ����
		break;
	case PLAYER_STATE::PS_Walk:
		move();  // �ȱ� �ִϸ��̼��� ����
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
		// �ʿ信 ���� �ٸ� ���� �߰�
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
	vector<Vector3> collisionPoints;       // �浹 ��ǥ�� ����
	int offsetY = _iPlayerHeightHalf;

	// ���� ��ġ�� ���� ��ǥ�� ���
	int x = static_cast<int>(_vPos.x);
	int y = static_cast<int>(_vPos.y);

	// �¿� �浹 �˻� ��ǥ�� ���
	int xLeft = x - _iPlayerWidthHalf;
	int xRight = x + _iPlayerWidthHalf;

	// ���� �浹 �˻� ��ǥ�� ���
	int yTop = y - 10.f;
	int yBottom = y + _iPlayerHeightHalf;

	// �浹��ǥ�� ���͸� �߰�
	collisionPoints.emplace_back(Vector3(x, y, 0.0f));
	collisionPoints.emplace_back(Vector3(xLeft, y, 0.0f));
	collisionPoints.emplace_back(Vector3(xRight, y, 0.0f));
	collisionPoints.emplace_back(Vector3(x, yTop, 0.0f));

	return collisionPoints;
}

bool CPlayerScript::CheckPixelCollision(int _iPosX, int _iPosY, PIXEL& _pPixel, const string& _colTag)
{
	//// ��ǥ�� �ȼ� �浹 ��ü�� ���� ���� �ִ��� Ȯ��
	//if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() && _iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {
	//	_pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

	//	// �浹 �±׿� ���� �浹 ���θ� ����
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
	// �߽� �������� ��ǥ ��ȯ

	if (!m_pPixelCollider) {
		OutputDebugStringA("Pixel collider not set.\n");
		return false;
	}

	if (_iPosX >= 0 && _iPosX < m_pPixelCollider->GetWidth() &&
		_iPosY >= 0 && _iPosY < m_pPixelCollider->GetHeight()) {

		// �ȼ� ������ ��������
		_pPixel = m_pPixelCollider->GetPixelColor(_iPosX, _iPosY);

		// ����� ���
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

	//// �߽� �������� ��ǥ ��ȯ
	//int centerX = m_pPixelCollider->GetWidth() / 2;
	//int centerY = m_pPixelCollider->GetHeight() / 2;

	//int localX = _iPosX + centerX;  // ���߾��� �������� ��ȯ
	//int localY = centerY - _iPosY;  // Y�� ���� ���� �� ���߾� ���� ��ȯ

	//if (localX >= 0 && localX < m_pPixelCollider->GetWidth() &&
	//	localY >= 0 && localY < m_pPixelCollider->GetHeight()) {

	//	// �ȼ� ������ ��������
	//	_pPixel = m_pPixelCollider->GetPixelColor(localX, localY);

	//	// ����� ���
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
	// �������� �浹 �� ��, ���� �浹 ���¸� ��Ÿ���� ���� ����
	static bool isCollidingWithStage = false;
	static bool isCollidingWithWall = false;
	static bool isCollidingWithRope = false;

	// �ȼ� �浹 ��ü�� �����Ѵٸ�
	if (m_pPixelCollider) {
		CTransform* tr = GetOwner()->GetComponent<CTransform>();
		Vector3 vPos = tr->GetPosition();

		int playerWidthHalf = 17;
		int playerHeightHalf = 32;

		auto collisionPoints = GetCollisionPoints(vPos, playerWidthHalf, playerHeightHalf);
		m_vecCollisionPoint.clear(); // �浹 ��ġ ���� �ʱ�ȭ

		// �浹 ���� �÷��� �ʱ�ȭ
		bool stageCollisionDetected = false;
		bool wallCollisionDetected = false;
		bool ropeCollisionDetected = false;

		PIXEL pixel;

		// �������� �浹 �˻�
		const auto& stagePoint = collisionPoints[0];
		if (CheckPixelCollision(stagePoint.x, stagePoint.y, pixel, "StageColl")) {
			m_vecCollisionPoint.emplace_back(stagePoint);
			stageCollisionDetected = true;

			// �ȼ� ���� �����
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

		//// �� �浹 �˻�
		//for (size_t i = 1; i <= 2; ++i) {
		//	const auto& point = collisionPoints[i];
		//	if (CheckPixelCollision(point.x, point.y, pixel, "Wall")) {
		//		m_CollisionPoint.emplace_back(point);
		//		wallCollisionDetected = true;
		//	}
		//}

		//// ���� �浹 �˻�
		//const auto& ropePoint = collisionPoints.back();
		//int ropeCollisionRadius = 10; // ���� �浹 ���� �ݰ�

		//for (int dx = -ropeCollisionRadius; dx <= ropeCollisionRadius; dx += 1) { // X�� �������� ��ǥ �˻�
		//	if (CheckPixelCollision(ropePoint.x + dx, ropePoint.y, pixel, "Rope")) {
		//		m_CollisionPoint.emplace_back(Vec2(ropePoint.x + dx, ropePoint.y));
		//		ropeCollisionDetected = true;
		//		m_vRopePos = Vec2(ropePoint.x + dx, ropePoint.y); // ���� ��ġ ������Ʈ
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

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Idle";
	wstring filePathPattern = L"../Resources/Texture/Player/Idle/" + direction + L"/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 3;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		54.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���

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

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring animationName = L"Walk";
	wstring filePathPattern = L"../Resources/Texture/Player/Walk/Right/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 4;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		54.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���
}

void CPlayerScript::prone()
{
	CAnimator* animator = GetOwner()->GetComponent<CAnimator>();
	if (animator == nullptr)
	{
		animator = GetOwner()->AddComponent<CAnimator>();
	}

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Prone";
	wstring filePathPattern = L"../Resources/Texture/Player/Prone/" + direction + L"/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 1;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		80.0f,                 // �� �������� �ʺ�
		37.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���

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

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Idle �ִϸ��̼� ����
	wstring direction = (m_iDir == 1) ? L"Right" : L"Left";
	wstring animationName = L"Jump";
	wstring filePathPattern = L"../Resources/Texture/Player/Jump/" + direction + L"/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 1;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		54.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, true);  // �ݺ� ���

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

	// SpriteRenderer ��������
	CSpriteRenderer* sr = GetOwner()->GetComponent<CSpriteRenderer>();
	if (sr == nullptr)
	{
		sr = GetOwner()->AddComponent<CSpriteRenderer>();
	}

	// Attack �ִϸ��̼� ����
	wstring animationName = L"Attack";
	wstring filePathPattern = L"../Resources/Texture/Player/Attack/Right/%d.bmp";  // %d�� ������ ��ȣ ����
	int frameCount = 3;  // �ִϸ��̼� ������ �� (��: 1.bmp, 2.bmp, 3.bmp)
	float frameDuration = 0.5f;  // ������ �� ���� �ð� (0.5�ʷ� ����)

	// �ִϸ��̼� �߰�
	animator->AddFrameAnimation(
		animationName,
		filePathPattern.c_str(),
		frameCount,
		Vector2(0.0f, 0.0f),  // ���� ��� ��ǥ
		55.0f,                 // �� �������� �ʺ�
		65.0f,                 // �� �������� ����
		0.0f,                  // ������ �� x ������
		0.0f,                  // ������ �� y ������
		frameDuration         // ������ ���� �ð�
	);

	// �ִϸ��̼� ���� (ù ��° ������ ���� �������� ����)
	animator->Play(animationName, false);  // �ݺ����� �ʵ��� false ����

	// �ִϸ��̼��� �������� üũ�ϰ�, �������� Idle ���·� ����
	if (animator->End())  // �ִϸ��̼��� �����ٸ�
	{
		m_ePlayerState = PLAYER_STATE::PS_Idle;  // Idle ���·� ����
	}
}