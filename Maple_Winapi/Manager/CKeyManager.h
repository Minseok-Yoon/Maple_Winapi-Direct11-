#pragma once

enum class KEY_STATE
{
	NONE,	// 눌리지 않았고, 이전에도 눌리지 않은 상태
	TAP,	// 막 누른 시점
	HOLD,	// 누르고 있음
	AWAY,	// 막 땐 시점
};

enum class KEY_CODE
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G,
	Z, X, C, V, B, N,
	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBTN,
	RBTN,
	F1,
	F2,
	F3,
	F4,

	LAST,
};

class CKeyManager
{
	struct tKeyInfo
	{
		KEY_CODE	eKeyCode;
		KEY_STATE	eKeyState;			// 키의 상태값
		bool		bPrevPush;		// 이전 프레임에서 눌렷는지 여부
	};

	SINGLETON(CKeyManager);

public:
	static void Init();
	static void Update();

public:
	static KEY_STATE GetKeyState(KEY_CODE _eCodeKey) { return m_vecKey[(UINT)_eCodeKey].eKeyState; }
	static math::Vector2 GetMousePos() { return m_vCurMousePos; }

	static bool IsMouseClicked(KEY_CODE _eMouseBtn);

private:
	static void updateKey(tKeyInfo& _key);

	static bool isKeyDown(KEY_CODE _code);
	static void updateKeyDown(tKeyInfo& _key);
	static void updateKeyUp(tKeyInfo& _key);
	static void getMousePositionByWindow();
	static void clearKeys();

	// 키의 상태는 어느 클래스 안에서도 접근이 가능해야 하니 정적 변수로 선언.
	static vector<tKeyInfo>		m_vecKey;
	static math::Vector2		m_vCurMousePos;
};