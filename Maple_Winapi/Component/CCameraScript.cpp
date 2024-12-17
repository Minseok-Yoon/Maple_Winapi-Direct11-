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

CCameraScript::CCameraScript()
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
	math::Vector3 vPos = tr->GetPosition();

	if (KEY_HOLD(KEY_CODE::A))
		vPos += 200.0f * -tr->Right() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::W))
		vPos += 200.0f * tr->Forward() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::D))
		vPos += 200.0f * tr->Right() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::S))
		vPos += 200.0f * -tr->Forward() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::E))
		vPos += 200.0f * tr->Up() * CTimeManager::GetfDeltaTime();
	if (KEY_HOLD(KEY_CODE::Q))
		vPos += 200.0f * -tr->Up() * CTimeManager::GetfDeltaTime();

	tr->SetPosition(vPos);
}

void CCameraScript::LateUpdate()
{
    CTransform* tr = GetOwner()->GetComponent<CTransform>();
    Vector3 vPos = tr->GetPosition();

    CScene* activeScene = CSceneManager::GetCurScene();
    if (activeScene && activeScene->m_pBackGround)
    {
        CTexture* texture = activeScene->m_pBackGround->GetBackGroundTexture();
        if (!texture)
        {
            CTexture::TextureSize textureSize = texture->GetTextureSize();
            float left = 0.0f;
            float top = 0.0f;
            float right = textureSize.width;
            float bottom = textureSize.height;

            // 카메라 클립 크기 가져오기
            CCamera* camera = GetOwner()->GetComponent<CCamera>();

            // 카메라가 정사영 프로젝션인 경우에만 클리핑 범위 제한
            if (camera->GetProjectionType() == CCamera::PROJECTION_TYPE::PT_Orthographic)
            {
                left = camera->GetLeft();
                right = camera->GetRight();
                top = camera->GetTop();
                bottom = camera->GetBottom();

                // 텍스처 크기와 카메라의 비율을 맞춰서 제한
                float aspectRatio = right - left; // 카메라의 비율 (왼쪽과 오른쪽의 차이)
                float textureAspectRatio = textureSize.width / textureSize.height; // 텍스처의 비율

                // 텍스처의 비율에 맞게 카메라의 클리핑 영역을 조정
                if (textureAspectRatio > aspectRatio)
                {
                    // 텍스처가 더 넓을 경우, 카메라의 왼쪽과 오른쪽을 조정
                    float newWidth = textureSize.height * aspectRatio;
                    left = (textureSize.width - newWidth) / 2.0f;
                    right = left + newWidth;
                }
                else
                {
                    // 텍스처가 더 높을 경우, 카메라의 상단과 하단을 조정
                    float newHeight = textureSize.width / aspectRatio;
                    top = (textureSize.height - newHeight) / 2.0f;
                    bottom = top + newHeight;
                }
            }

            // 카메라 이동 범위 제한
            vPos.x = std::clamp(vPos.x, left + textureSize.width / 2.0f, right - textureSize.width / 2.0f);
            vPos.y = std::clamp(vPos.y, top + textureSize.height / 2.0f, bottom - textureSize.height / 2.0f);

            tr->SetPosition(vPos);
        }
    }
}

void CCameraScript::Render()
{
}
