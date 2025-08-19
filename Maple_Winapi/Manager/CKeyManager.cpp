#include "../pch.h"
#include "CKeyManager.h"
#include "../Core/CCore.h"

extern CCore core;

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
	'N',
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
	VK_F4,
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

bool CKeyManager::IsMouseClicked(KEY_CODE _eMouseBtn)
{
	if (_eMouseBtn != KEY_CODE::LBTN && _eMouseBtn != KEY_CODE::RBTN)
		return false;

	return GetKeyState(_eMouseBtn) == KEY_STATE::TAP;
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

	UINT width = core.GetWidth();
	UINT height = core.GetHeight();

	m_vCurMousePos.x = -1.0f;
	m_vCurMousePos.y = -1.0f;

	if (mousePos.x >= 0 && mousePos.x < static_cast<int>(width))
		m_vCurMousePos.x = static_cast<float>(mousePos.x);
	if (mousePos.y >= 0 && mousePos.y < static_cast<int>(height))
		m_vCurMousePos.y = static_cast<float>(mousePos.y);
	/*if (static_cast<UINT>(mousePos.x) > 0 && static_cast<UINT>(mousePos.x) < width)
		m_vCurMousePos.x = static_cast<float>(mousePos.x);
	if (static_cast<UINT>(mousePos.y) > 0 && static_cast<UINT>(mousePos.y) < height)
		m_vCurMousePos.y = static_cast<float>(mousePos.y);*/
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