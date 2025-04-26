#pragma once

enum class BRUSH_TYPE
{
	BT_Hollow,
	BT_Black,
	BT_END,
};

enum class PEN_TYPE
{
	PT_Red,
	PT_Green,
	PT_Blue,
	PT_Yellow,
	PT_End,
};

enum class OBJECT_TYPE
{
	OT_Default,
	OT_Item,
	OT_Meso,
	OT_Portal,
	OT_BackGround,
	OT_PixelBackGround,
	OT_Player,
	OT_Monster,
	OT_Rectangle,
	OT_Pixel,
	OT_Damage,

	OT_UI = 31,
	OT_End = 32,
};

enum class MON_STATE
{
	MS_Idle,		// 대기 상태
	MS_Patrol,		// 순찰 상태
	MS_Trace,		// 추적 상태
	MS_Attack,		// 공격
	MS_Run,
	MS_Dead,
};

enum class PLAYER_STATE
{
	PS_Idle,
	PS_Walk,
	PS_Attack,
	PS_Jump,
	PS_Rope,
	PS_Prone,
	PS_Dead,
};

enum COLLISION_STATE
{
	CS_Enter,
	CS_Stay,
	CS_Exit
};

enum class COLLIDER_TYPE
{
	// 3D에서 z값을 0으로 주면 2D값이 출력
	CT_Rect2D,
	CT_Circle2D,
	CT_Line2D,
	CT_AABB2D,
	CT_OBB2D,
	CT_Line3D,
	CT_Circle3D,
	CT_AABB3D,
	CT_OBB3D,
	CT_Pixel,
	CT_End
};

enum ITEM_TAG
{
	IT_None,
	IT_RedPotion,
	IT_Meso,
	IT_Size
};

enum class CAM_ORDER
{
	CO_Main,
	CO_Ui
};

enum class CAM_EFFECT
{
	CE_None,
	CE_Fade_In,
	CE_Fade_Out
};

enum class OBJECT_STATE
{
	OS_Active,
	OS_Paused,
	OS_Dead,
	OS_End
};

enum class LAYER_TYPE
{
	LT_None,
	LT_PixelBackGround,
	LT_BackGround,
	LT_Player,
	LT_Monster,
	LT_Floor,
	LT_Tile,
	LT_Particle,
	LT_UIBackGround,
	LT_UI,
	LT_End,
};

enum class COMPONENT_TYPE
{
	CT_Transform,
	CT_Collider,
	CT_RigidBody,
	CT_Gravity,
	CT_Script,
	CT_SpriteRenderer,
	CT_LineRenderer,
	CT_Animator,
	CT_Camera,
	CT_AudioListener,
	CT_AudioSource,
	CT_End
};

enum class RESOURCE_TYPE
{
	RT_Texture,
	RT_Mesh,
	RT_Material,
	RT_Animation,
	RT_AudioClip,
	RT_Shader,
	RT_Prefab,
	RT_End
};

enum class TEXTURE_STATE
{
	TS_None,
	TS_Bmp,
	TS_Png
};

enum class ANIMATION_STATE
{
	AS_None,
	AS_Pause,
	AS_Finish
};

enum class UI_TYPE
{
	UT_HpBar,
	UT_MiniMap,
	UT_Button,
	UT_End
};

enum class CAMERA_ORDER
{
	CO_None = 0,
	CO_Main = 1,
	CO_UI = 10
};

enum class CLEAR_FLAG
{
	CF_Color,      // 기본값 - 배경색으로 Clear
	CF_DepthOnly,  // Depth만 Clear
	CF_DontClear   // 아무것도 Clear 안 함
};