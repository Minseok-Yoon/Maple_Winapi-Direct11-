#include "../pch.h"
#include "CPortal.h"
#include "CObject.h"
#include "../Component/CCollider.h"
#include "../Component/CAnimator.h"
#include "../Component/CAnimation.h"

int CPortal::m_iPortalCount = 0;

CPortal::CPortal(const string& portalTag) :
	m_strPortalTag(portalTag)
{
}

CPortal::~CPortal()
{
}

void CPortal::StagePortals()
{
	m_stagePortals["Main"] = {
		{{150.f, 820.f}, "Stage1"}
	};

	m_stagePortals["Stage1"] = {
		{{1280.f, 485.f}, "Main"}
	};

	m_portalToStageMap["Portal 0"] = "Stage1";
	m_portalToStageMap["Portal 1"] = "Main";
}

void CPortal::PortalAnimationClip()
{
	/*CreateAnimator();

	AddAnimationClip(L"Stand", L"texture\\Portal\\%d.bmp", 8, 1.f, 88.f, 217.f, 0.f, 0.f);*/
}

void CPortal::AddAnimationClip(const wstring& strKey, const wchar_t* pFilePath, int iFrameMax, float fAnimationLimitTime, float fFrameSizeX, float fFrameSizeY, float _fOffsetX, float _fOffsetY)
{
	/*vector<wstring> vecFile;

	for (int i = 1; i <= iFrameMax; i++)
	{
		wchar_t strFileName[MAX_PATH] = {};
		wsprintf(strFileName, pFilePath, i);
		vecFile.push_back(strFileName);
	}

	GetAnimator()->CreateFrameAnimation(strKey, vecFile, Vector2(0.f, 0.f), Vector2(fFrameSizeX, fFrameSizeY), fAnimationLimitTime, Vector2(_fOffsetX, _fOffsetY));*/
}

void CPortal::Update()
{
}

void CPortal::Render()
{
	/*CObject::ComponentRender(_dc);

	Vector2 vPos = GetPos();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);*/
}