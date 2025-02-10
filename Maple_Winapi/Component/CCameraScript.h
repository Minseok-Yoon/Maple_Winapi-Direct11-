#pragma once
#include "../Component/CScript.h"
#include "../Component/CTransform.h"

class CCameraScript : public CScript
{
public:
	CCameraScript();
	virtual ~CCameraScript();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;

	float Clamp(float value, float minVal, float maxVal);

private:
	Vector2			m_vLookPosition;
};