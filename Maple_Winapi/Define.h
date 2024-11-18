#pragma once

// Singletion Macro
#define SINGLETON(type)	public:\
							static type* GetInst()\
							{\
								static type Manager;\
								return &Manager;\
							}\
						public:\
							type();\
							~type();

#define SAFE_RELEASE(p)		if(p) { p->Release(); p = NULL; }

#define fDeltaTime CTimeManager::GetInst()->GetfDeltaTime()
#define DeltaTime CTimeManager::GetInst()->GetDeltaTime()

// �Ͻ��� ��������� ȣ���� ��� �θ� ��������� ȣ��ȴ�.
#define CLONE(type) type* Clone() { return new type(*this); }

#define KEY_CHECK(key, state) CKeyManager::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS CKeyManager::GetInst()->GetMousePos()

#define _HAS_STD_BYTE 0
#define _CRTDBG_MAP_ALLOC

#define CAST_UINT(value) static_cast<UINT>(value);
#define CAST_FLOAT(value) static_cast<float>(value);

// ���� �޼��� �ڽ� ����
#define AssertMsgBox(Text)\
std::string Value = Text;\
MessageBoxA(nullptr, Value.c_str(), "Error", MB_OK);\
assert(false);