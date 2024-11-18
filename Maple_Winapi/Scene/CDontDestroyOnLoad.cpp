#include "CDontDestroyOnLoad.h"

CDontDestroyOnLoad::CDontDestroyOnLoad()
{
}

CDontDestroyOnLoad::~CDontDestroyOnLoad()
{
}

void CDontDestroyOnLoad::Init()
{
	CScene::Init();
}

void CDontDestroyOnLoad::Update()
{
	CScene::Update();
}

void CDontDestroyOnLoad::LateUpdate()
{
	CScene::LateUpdate();
}

void CDontDestroyOnLoad::Render()
{
	CScene::Render();
}

void CDontDestroyOnLoad::Enter()
{
	CScene::Enter();
}

void CDontDestroyOnLoad::Exit()
{
	CScene::Exit();
}