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

            // ī�޶� Ŭ�� ũ�� ��������
            CCamera* camera = GetOwner()->GetComponent<CCamera>();

            // ī�޶� ���翵 ���������� ��쿡�� Ŭ���� ���� ����
            if (camera->GetProjectionType() == CCamera::PROJECTION_TYPE::PT_Orthographic)
            {
                left = camera->GetLeft();
                right = camera->GetRight();
                top = camera->GetTop();
                bottom = camera->GetBottom();

                // �ؽ�ó ũ��� ī�޶��� ������ ���缭 ����
                float aspectRatio = right - left; // ī�޶��� ���� (���ʰ� �������� ����)
                float textureAspectRatio = textureSize.width / textureSize.height; // �ؽ�ó�� ����

                // �ؽ�ó�� ������ �°� ī�޶��� Ŭ���� ������ ����
                if (textureAspectRatio > aspectRatio)
                {
                    // �ؽ�ó�� �� ���� ���, ī�޶��� ���ʰ� �������� ����
                    float newWidth = textureSize.height * aspectRatio;
                    left = (textureSize.width - newWidth) / 2.0f;
                    right = left + newWidth;
                }
                else
                {
                    // �ؽ�ó�� �� ���� ���, ī�޶��� ��ܰ� �ϴ��� ����
                    float newHeight = textureSize.width / aspectRatio;
                    top = (textureSize.height - newHeight) / 2.0f;
                    bottom = top + newHeight;
                }
            }

            // ī�޶� �̵� ���� ����
            vPos.x = std::clamp(vPos.x, left + textureSize.width / 2.0f, right - textureSize.width / 2.0f);
            vPos.y = std::clamp(vPos.y, top + textureSize.height / 2.0f, bottom - textureSize.height / 2.0f);

            tr->SetPosition(vPos);
        }
    }
}

void CCameraScript::Render()
{
}
