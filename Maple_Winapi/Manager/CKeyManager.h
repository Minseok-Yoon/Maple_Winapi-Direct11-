#pragma once

enum class KEY_STATE
{
	NONE,	// ������ �ʾҰ�, �������� ������ ���� ����
	TAP,	// �� ���� ����
	HOLD,	// ������ ����
	AWAY,	// �� �� ����
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
		KEY_STATE	eKeyState;			// Ű�� ���°�
		bool		bPrevPush;		// ���� �����ӿ��� ���Ǵ��� ����
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

	// Ű�� ���´� ��� Ŭ���� �ȿ����� ������ �����ؾ� �ϴ� ���� ������ ����.
	static vector<tKeyInfo>		m_vecKey;
	static math::Vector2		m_vCurMousePos;
};