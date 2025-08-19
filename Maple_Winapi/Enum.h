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
	MS_Idle,		// ��� ����
	MS_Move,
	MS_Patrol,		// ���� ����
	MS_Trace,		// ���� ����
	MS_Attack,		// ����
	MS_Run,
	MS_Dead,
};

enum class BOSS_MON_STATE
{
	BMS_Idle,		// ��� ����
	BMS_summon1,
	BMS_Move,
	BMS_Trace,		// ���� ����
	BMS_Attack,		// ����
	BMS_Run,
	BMS_Dead,
};

enum class NPC_STATE
{
	NS_Idle,		// ��� ����
	NS_Move,
	NS_Patrol,		// ���� ����
	NS_Trace,		// ���� ����
	NS_Attack,		// ����
	NS_Run,
	NS_Dead,
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
	// 3D���� z���� 0���� �ָ� 2D���� ���
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

enum class CAM_EFFECT
{
	CE_None,
	CE_Fade_In,
	CE_Fade_Out
};

enum class OBJECT_STATE
{
	OS_None,
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
	LT_Item,
	LT_Npc,
	LT_Player,
	LT_Skill,
	LT_Monster,
	LT_BossMonster,
	LT_Floor,
	LT_Portal,
	LT_Tile,
	LT_Particle,
	LT_UIBackGround,
	LT_UI,
	LT_End
};

enum class COMPONENT_TYPE
{
	CT_Transform,
	CT_UITransform,
	CT_Collider,
	CT_PixelCollider,
	CT_RigidBody,
	CT_Gravity,
	CT_Script,
	CT_UIRenderer,
	CT_SpriteRenderer,
	CT_LineRenderer,
	CT_Animator,
	CT_Camera,
	CT_AudioListener,
	CT_AudioSource,
	CT_End
};

enum class SCRIPT_TYPE
{
	ST_CameraScript,
	ST_NpcScript,
	ST_PlayerScript,
	ST_SkillScript,
	ST_PlayerBehavious,
	ST_MonsterScript,
	ST_BossMonsterScript,
	ST_ItemScript,
	ST_PortalScript,
	ST_End
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
	UT_NotificationToast,
	UT_HpBar,
	UT_MentBox,
	UT_Text,
	UI_Inventory,
	UT_MiniMap,
	UT_Button,
	UT_End
};

enum class CAMERA_ORDER
{
	CO_Main,
	CO_UI
};

enum class CLEAR_FLAG
{
	CF_Color,      // �⺻�� - �������� Clear
	CF_DepthOnly,  // Depth�� Clear
	CF_DontClear   // �ƹ��͵� Clear �� ��
};

enum class PIVOT_TYPE
{
	PT_Center,
	PT_Top,
	PT_RightUp,
	PT_Right,
	PT_RightBottom,
	PT_Bottom,
	PT_LeftBottom,
	PT_Left,
	PT_LeftTop,
};

enum class Skill_State
{
	SS_Idle,
	SS_Casting,
	SS_Cooldown,
	SS_End
};

enum class SKILL_TYPE
{
	ST_Trinity,
};