#pragma once
#include "../Component/CComponent.h"

class CAudioListener : public CComponent
{
public:
	CAudioListener();
	virtual ~CAudioListener();

	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void Render(const Matrix& view, const Matrix& projection) override;
};