#pragma once
#include "../Object/CBoss.h"

#define ATTACK1_COOLDOWN 10.0f

//enum class Jinhilla_State
//{
//	JS_Idle,
//	JS_Death,
//	JS_Move,
//	JS_Attack1,
//};
//
//struct Jinhilla_Phase1_Skill
//{
//	Jinhilla_Phase1_Skill() {}
//	Jinhilla_Phase1_Skill(float _fSkillCoolDown, Jinhilla_State _eJinhillaState) :
//		SkillCoolDownValue(_fSkillCoolDown), SkillCoolDown(_fSkillCoolDown), SkillState(_eJinhillaState)
//	{
//	}
//
//	float SkillCoolDownValue = 0.0f;
//	float SkillCoolDown = 0.0f;
//	Jinhilla_State SkillState = Jinhilla_State::JS_Idle;
//};

class Jinhilla_Phase1 : public CBoss
{
public:
	Jinhilla_Phase1();
	virtual ~Jinhilla_Phase1();

	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(const Matrix& view, const Matrix& projection) override;
	//virtual void Release() override;

private:
	bool OnceWarning = false;
	//vector<Jinhilla_Phase1_Skill> m_vecSkillInfo;
	//Jinhilla_State m_eState = Jinhilla_State::JS_Idle;
	//std::shared_ptr<GameEngineSpriteRenderer> FlowerRenderer = nullptr;
	//std::shared_ptr<GameEngineSpriteRenderer> TeleportRenderer = nullptr;
	//GameEngineSoundPlayer BossPlayer;

	class CAudioSource* m_pAudioSource = nullptr; // 나중에 Scene으로 옮겨서 사용할 것
	//Jinhilla_Phase1_GUI* m_pJinhillaGui = nullptr;

};

//class Jinhilla_Phase1_GUI : public GameEngineGUIWindow
//{
//	friend class Jinhilla_Phase1;
//
//private:
//	bool				m_bIsGUIUpdate = true;
//	Jinhilla_Phase1*	m_pCurBoss = nullptr;
//	void Init() override;
//	void OnGUI(GameEngineLevel* _Level, float _DeltaTime) override;
//};
