#pragma once
#include "../Component/CComponent.h"

class CAudioListener : public CComponent
{
public:
	CAudioListener();
	~CAudioListener();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render() override;
};