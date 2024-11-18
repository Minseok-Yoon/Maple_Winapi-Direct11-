#include "CLeakCheck.h"

namespace GameEngine
{
	void GameEngine::LeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
};