#pragma once
#include "CGameObject.h"
#include "../Component/CUIRenderer.h"

class CPlayerUI : public CGameObject
{
	CPlayerUI();
	virtual ~CPlayerUI();

	CPlayerUI(const CPlayerUI& _Ohter) = delete;
	CPlayerUI(CPlayerUI&& _Ohter) noexcept = delete;
	CPlayerUI& operator=(const CPlayerUI& _Other) = delete;
	CPlayerUI& operator=(CPlayerUI&& _Other) noexcept = delete;

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;

private:
	void playerStatusUpdate(float _Delta);
	void hpUpdate(float _Delta);
	void mpUpdate(float _Delta);
	//void numUpdate(BarFrame _Bar, int PlayerStateValue);

private:
	float ReflectUpSpeed = 100.0f;
	float ReflectDownSpeed = 30.0f;
	float CurHPScale = 0;
	float CurMPScale = 0;

	class CUIRenderer* m_pExpBar = nullptr;

	//std::shared_ptr<GameEngineUIRenderer> EXP_Bar = nullptr;
	//std::shared_ptr<GameEngineUIRenderer> HP_Bar = nullptr;
	//std::shared_ptr<GameEngineUIRenderer> HP_Bar_BG = nullptr;
	//BarFrame HP;
	//BarFrame MP;
	//std::shared_ptr<GameEngineUIRenderer> Bar_Name = nullptr;
	//std::vector<std::shared_ptr<class ContentButton>> PlayerUIButtons;
};