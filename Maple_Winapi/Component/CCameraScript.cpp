#include "CCameraScript.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "../Manager/CKeyManager.h"
#include "../Manager/CTimeManager.h"
#include "../Manager/CResourceManager.h"
#include "../Object/CGameObject.h"
#include "../Object/CObject.h"
#include "../Component/CCamera.h"
#include "../Scene/CScene.h"
#include "../Resource/CTexture.h"
#include "../Core/CCore.h"

#include "../Scene/CTestScene.h"
#include "CRenderer.h"

extern CCore core;

CCameraScript::CCameraScript() :
    CScript(SCRIPT_TYPE::ST_CameraScript),
	m_vLookPosition(Vector2(0.0f, 0.0f)),
    m_bFreeCameraMode(false),
    m_pTarget(nullptr)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::Init()
{
}

void CCameraScript::Update()
{
    CTransform* tr = GetOwner()->GetComponent<CTransform>();
    Vector3 vPos = tr->GetWorldPosition();

    // 🔹 F2 키로 프리카메라 모드 토글
    if (KEY_TAP(KEY_CODE::F2))
    {
        m_bFreeCameraMode = !m_bFreeCameraMode;

        wstring camName = GetOwner()->GetName();
        wstring camType;

        if (camName.find(L"UICamera") != wstring::npos) camType = L"UI 카메라";
        else if (camName.find(L"MainCamera") != wstring::npos) camType = L"Main 카메라";
        else camType = L"카메라 없음";

        wstring debugStr = m_bFreeCameraMode ?
            L"프리카메라 모드 활성화 - " + camType + L"\n" :
            L"프리카메라 모드 비활성화 - " + camType + L"\n";

        OutputDebugStringW(debugStr.c_str());
    }

    if (m_bFreeCameraMode)
    {
        if (KEY_HOLD(KEY_CODE::A))
            vPos.x -= 200.0f * CTimeManager::GetfDeltaTime();
        if (KEY_HOLD(KEY_CODE::D))
            vPos.x += 200.0f * CTimeManager::GetfDeltaTime();
        if (KEY_HOLD(KEY_CODE::W))
            vPos.y += 200.0f * CTimeManager::GetfDeltaTime();
        if (KEY_HOLD(KEY_CODE::S))
            vPos.y -= 200.0f * CTimeManager::GetfDeltaTime();

        // 클램프 후 적용
        vPos = ClampCameraPosition(vPos);
        vPos.z = -10.0f;  // 직교 카메라는 보통 고정

        tr->SetLocalPosition(vPos);
        return;
    }

    // 🔹 타겟 추적 모드
    if (m_pTarget != nullptr)
    {
        //CTransform* targetTr = m_pTarget->GetComponent<CTransform>();
        //Vector3 targetPos = targetTr->GetWorldPosition();
        //Vector3 desiredPos = targetPos + m_vOffset;
        //desiredPos.z = -10.0f;

        //Vector3 smoothPos = Vector3::Lerp(vPos, desiredPos, 5.0f * CTimeManager::GetfDeltaTime());

        //// 🔹 Clamp 적용
        //smoothPos = ClampCameraPosition(smoothPos);
        //smoothPos.z = -10.0f;

        //tr->SetWorldPosition(smoothPos);
        //OutputDebugStringW(L"타겟 설정됨 -> 추적 로직 실행 시작\n");
        CTransform* targetTr = m_pTarget->GetComponent<CTransform>();
        if (targetTr != nullptr)
        {
            Vector3 targetPos = targetTr->GetWorldPosition();
            Vector3 desiredPos = targetPos + m_vOffset;
            desiredPos.z = -10.0f;

            //OutputDebugStringW((L"타겟 위치: " + to_wstring(targetPos.x) + L", " + to_wstring(targetPos.y) + L"\n").c_str());
            //OutputDebugStringW((L"목표 카메라 위치: " + to_wstring(desiredPos.x) + L", " + to_wstring(desiredPos.y) + L"\n").c_str());

            Vector3 smoothPos = Vector3::Lerp(vPos, desiredPos, 15.0f * CTimeManager::GetfDeltaTime());
            //OutputDebugStringW((L"보간 카메라 위치: " + to_wstring(smoothPos.x) + L", " + to_wstring(smoothPos.y) + L"\n").c_str());

            smoothPos = ClampCameraPosition(smoothPos);
            smoothPos.z = -10.0f;

            //OutputDebugStringW((L"최종 카메라 위치: " + to_wstring(smoothPos.x) + L", " + to_wstring(smoothPos.y) + L"\n").c_str());

            tr->SetWorldPosition(smoothPos);

            // 위치 설정 후 재확인
            Vector3 finalPos = tr->GetWorldPosition();
            if (finalPos != smoothPos)
            {
                //OutputDebugStringW(L"⚠️ 카메라 위치 설정 실패!\n");
                //OutputDebugStringW((L"실제 설정된 위치: " + to_wstring(finalPos.x) + L", " + to_wstring(finalPos.y) + L"\n").c_str());
            }
        }
        else
        {
            //OutputDebugStringW(L"❌ 타겟의 Transform 컴포넌트를 가져올 수 없습니다\n");
        }
    }
    else
    {
        //OutputDebugStringW(L"❌ 타겟이 NULL입니다\n");
    }
}

void CCameraScript::LateUpdate()
{
}

void CCameraScript::Render(const Matrix& view, const Matrix& projection)
{
}

float CCameraScript::Clamp(float value, float minVal, float maxVal)
{
	if (value < minVal) return minVal;
	if (value > maxVal) return maxVal;
	return value;
}

Vector3 CCameraScript::ClampCameraPosition(const Vector3& _vPos)
{
    Vector3 result = _vPos;

    CScene* pCurScene = CSceneManager::GetCurScene();
    if (pCurScene)
    {
        CBackGround* bg = pCurScene->GetBackGround();

        if (bg)
        {
            // 해당 배경의 width와 height가 존재하는데 Local, WorldSize는 1, 1, 1로 나오는지?
            CTransform* bgtr = bg->GetComponent<CTransform>();
            if (bgtr)
            {
                Vector3 bgSize = bgtr->GetWorldScale();
                float bgWidth = bgSize.x * 0.5f;
                float bgHeight = bgSize.y * 0.5f;

                float viewportHalfWidth = core.GetWidth() * 0.5f;
                float viewportHalfHeight = core.GetHeight() * 0.5f;

                float minX = -bgWidth + viewportHalfWidth;
                float maxX = bgWidth - viewportHalfWidth;
                float minY = -bgHeight + viewportHalfHeight;
                float maxY = bgHeight - viewportHalfHeight;

                if (bgWidth < viewportHalfWidth)
                    minX = maxX = 0.0f;
                if (bgHeight < viewportHalfHeight)
                    minY = maxY = 0.0f;

                result.x = Clamp(result.x, minX, maxX);
                result.y = Clamp(result.y, minY, maxY);
            }
        }
    }

    return result;
}
