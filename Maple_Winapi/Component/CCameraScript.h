#pragma once
#include "../Component/CScript.h"
#include "../Component/CTransform.h"

class CCameraScript : public CScript
{
public:
	CCameraScript();
	~CCameraScript();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
};