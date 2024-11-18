#include "../pch.h"
#include "CKeyManager.h"
#include "../Core/CCore.h"

vector<CKeyManager::tKeyInfo> CKeyManager::m_vecKey = {};
Vector2 CKeyManager::m_vCurMousePos = Vector2(1.0f, 1.0f);

int g_arrVK[static_cast<UINT>(KEY_CODE::LAST)] =
{
	VK_LEFT, //LEFT,
	VK_RIGHT,//RIGHT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,
	VK_F1,
	VK_F2,
	VK_F3,
};

CKeyManager::CKeyManager()
{
}

CKeyManager::~CKeyManager()
{
}

void CKeyManager::Init()
{
	for (size_t i = 0; i < static_cast<UINT>(KEY_CODE::LAST); i++)
	{
		tKeyInfo key = {};
		key.bPrevPush = false;
		key.eKeyState = KEY_STATE::NONE;
		key.eKeyCode = static_cast<KEY_CODE>(i);

		m_vecKey.push_back(key);
	}
}

void CKeyManager::Update()
{
	for_each(m_vecKey.begin(), m_vecKey.end(),
		[](tKeyInfo& key) -> void
	{
		updateKey(key);
	});
}

void CKeyManager::updateKey(tKeyInfo& _key)
{
	// ������ ��Ŀ��(������ â�� Ȱ��ȭ, �� �������� �ִ� ����, �ּ�ȭ �Ǹ� ��Ȱ��ȭ)
	// ������ ��Ŀ�� �˾Ƴ���
	HWND hWnd = GetFocus();

	// ������ ��Ŀ�� ���� ��, Ű �̺�Ʈ ����
	if (nullptr != hWnd)
	{
		// Ű�� �����ִ�.(����Ű�� ����)
		if (isKeyDown(_key.eKeyCode))
		{
			updateKeyDown(_key);
		}
		// Ű�� �ȴ����ִ�.
		else
		{
			updateKeyUp(_key);
		}

		getMousePositionByWindow();
	}

	// ������ ��Ŀ�� ���� ����
	else
	{
		clearKeys();
	}
}

bool CKeyManager::isKeyDown(KEY_CODE _code)
{
	return GetAsyncKeyState(g_arrVK[(UINT)_code]) & 0x8000;
}

void CKeyManager::updateKeyDown(tKeyInfo& _key)
{
	if (_key.bPrevPush == true)
	{
		// �������� �����־���.
		_key.eKeyState = KEY_STATE::HOLD;
	}
	else
	{
		// ������ �������� �ʾҴ�.
		_key.eKeyState = KEY_STATE::TAP;
	}
	_key.bPrevPush = true;
}

void CKeyManager::updateKeyUp(tKeyInfo& _key)
{
	if (_key.bPrevPush == true)
	{
		// ������ �����־���.
		_key.eKeyState = KEY_STATE::AWAY;
	}
	else
	{
		// �������� �ȴ����־���.
		_key.eKeyState = KEY_STATE::NONE;
	}

	_key.bPrevPush = false;
}

void CKeyManager::getMousePositionByWindow()
{
	// Mouse ��ġ ���
	POINT mousePos = {};
	GetCursorPos(&mousePos);

	// ������ ��ǥ->Ŭ���̾�Ʈ ��ǥ(���ڷ� ���� ������ �ڵ鰪 �ִ´�.)
	ScreenToClient(CCore::GetInst()->GetMainHWnd(), &mousePos);

	m_vCurMousePos = Vector2((float)mousePos.x, (float)mousePos.y);
}

void CKeyManager::clearKeys()
{
	for (tKeyInfo& key : m_vecKey)
	{
		key.bPrevPush = false;

		if (key.eKeyState == KEY_STATE::TAP || key.eKeyState == KEY_STATE::HOLD)
		{
			key.eKeyState = KEY_STATE::AWAY;
		}
		else if (key.eKeyState == KEY_STATE::AWAY)
		{
			key.eKeyState = KEY_STATE::NONE;
		}
	}
}