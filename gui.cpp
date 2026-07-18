#pragma once
#include <Windows.h>
#include <cstdint>
#include <cmath>

#include "d3d11.h"

#include "drawing.h"

#define MAX_CLIENTS 18					// Players
#define MAX_ENTITIES 1024				// (in which 1023/0x3FF is your viewmodel)

#define ENTITY_TRACE_MASK 0x280B001		// Use this in trace functions (CG_TraceCapsule, CG_LocationalTrace...)

#define PITCH 0
#define YAW 1
#define ROLL 2
#define M_PI 3.14159265358979323846f

class engine_c
{
public:
	bool init() noexcept;
};
inline engine_c engine;

// typedefs
using vec = float;
using vec2_t = vec[2];
using vec3_t = vec[3];	// [0] is X, [1] is Y, [2] is Z
using vec4_t = vec[4];

// q3 sdk
static void vectoangles(const vec3_t value1, vec3_t angles)
{
	float	forward;
	float	yaw, pitch;

	if (value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if (value1[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		if (value1[0])
			yaw = (atan2(value1[1], value1[0]) * (180.0f / M_PI));
		else if (value1[1] > 0)
			yaw = 90.0f;
		else
			yaw = 270.0f;

		if (yaw < 0.0f)
			yaw += 360.0f;

		forward = sqrt((value1[0] * value1[0]) + (value1[1] * value1[1]));
		pitch = (atan2(value1[2], forward) * (180.0f / M_PI));

		if (pitch < 0.0f)
			pitch += 360;
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}

// game structs & enums
enum entity_type_e
{
	ET_GENERAL = 0x0,
	ET_PLAYER = 0x1,
	ET_PLAYER_CORPSE = 0x2,
	ET_ITEM = 0x3,
	ET_MISSILE = 0x4,
	ET_INVISIBLE = 0x5,
	ET_SCRIPTMOVER = 0x6,
	ET_SOUND_BLEND = 0x7,
	ET_FX = 0x8,
	ET_LOOP_FX = 0x9,
	ET_PRIMARY_LIGHT = 0xA,
	ET_TURRET = 0xB,
	ET_HELICOPTER = 0xC,
	ET_PLANE = 0xD,
	ET_VEHICLE = 0xE,
	ET_VEHICLE_CORPSE = 0xF,
	ET_ACTOR = 0x10,
	ET_ACTOR_SPAWNER = 0x11,
	ET_ACTOR_CORPSE = 0x12,
	ET_STREAMER_HINT = 0x13,
	ET_ZBARRIER = 0x14,
	ET_EVENTS = 0x15
};

struct snapshot_s
{
private:
	char _0x0[0x8];
public:
	int iServerTime;
private:
	char _0xC[0xC];
public:
	int iPing;
};

struct refdef_t
{
public:
	__int32 x; //0x0000 
	__int32 y; //0x0004 
	__int32 iWidth; //0x0008 
	__int32 iHeight; //0x000C 
	char _0x0010[16];
	float fovx; //0x0020 
	float fovy; //0x0024 
	float fovz; //0x0028 
	char _0x002C[4];
	float TotalFov; //0x0030 
	vec3_t vOrigin; //0x0034 
	char _0x0040[4];
	vec3_t viewAxis[3]; //0x0044 
	char _0x0068[4];
	__int32 iTime; //0x006C 
	char _0x0070[144];
};
extern refdef_t* refdef;

struct playerState_s
{
public:
	int iServerTime;
private:
	char _0x4[0x14];
public:
	int iOtherFlags;
private:
	char _0x1C[0xC];
public:
	vec3_t vOrigin;
	vec3_t vVelocity;
private:
	char _0x40[0xC];
public:
	float flWeaponTime;
	int iWeaponDelay;
private:
	char _0x54[0x30];
public:
	int iThirdPerson;
private:
	char _0x88[0x4];
public:
	int iGravity;
private:
	char _0x90[0x4];
public:
	int iSpeed;
	vec3_t vDeltaAngles;
private:
	char _0xA4[0x12C];
public:
	int iWeaponState[2];
private:
	char _0x1D8[0x8];
public:
	float flZoomProgress;
private:
	char _0x1E4[0x4];
public:
	float flSpreadOverride;         // 0x1E8
	int iSpreadOverrideState;
private:
	char _0x1F0[0x8];
public:
	vec3_t vViewAngles;
	int iPlayerHeight;
	vec3_t vPlayerHeight;
private:
	char _0x214[0x18];
public:
	int iDamageYaw;
	int iDamagePitch;
	int iDamageCount;
private:
	char _0x238[0x8];
public:
	int iHealth;
private:
	char _0x244[0x4];
public:
	int iMaxHealth;
private:
	char _0x24C[0x74];
public:
	int iSecondaryWeaponID;
private:
	char _0x2C4[0x18];
public:
	unsigned int iPrimaryWeaponID;
private:
	char _0x2E0[0x124];
public:
	int iSecondaryAmmoStock;
	int iPrimaryAmmoStock;
private:
	char _0x40C[0x34];
public:
	int iSecondaryAmmoClip;
	int iPrimaryAmmoClip;
	int iLethalAmmo;
	int iTacticalAmmo;
private:
	char _0x450[0x5C];
public:
	int iSatelliteTypeEnabled;      // VSAT
private:
	char _0x4B0[0xA0];
public:
	int iPerkFlags;
};

struct clientInfo_t
{
public:
	char _0x0000[4];
	__int32 IsValid;      //0x0004 
	__int32 clientNum;    //0x0008 
	char szName[32];      //0x000C 
	__int32 Team;         //0x002C 
	__int32 Team2;        //0x0030 
	char _0x0034[88];
	__int32 IsDead;       //0x008C 
	char _0x0090[4];
	__int32 Score;        //0x0094 
	__int32 Kills;        //0x0098 
	char _0x009C[4];
	__int32 Deaths;       //0x00A0 
	char _0x00A4[20];
	__int32 Assists;      //0x00B8 
	char _0x00BC[32];     // 0x00BC
	__int32 Health;       //0x00DC
	char _0x00E0[1888];
	float ViewAngleY;     //0x04C4 
	float ViewAngleX;     //0x04C8 
	char _0x04CC[192];
	__int32 IsFiring;     //0x058C 
	__int32 IsZoomed;     //0x0590 
	char _0x0594[48];
	__int32 weaponID;     //0x05C4 
	char _0x05C8[576];
};

struct centity_t
{
	char _0x0000[44];
	vec3_t vOrigin;		//0x002C // pose->Origin
	char _0x0038[296];
	BYTE State;			//0x0160  4 = crouch | 8 = prone | 40 = fire
	char _0x0161[1];
	BYTE IsZoomed;		//0x0162 
	char _0x0163[17];
	vec3_t vOldOrigin;	//0x0174 
	char _0x0180[92];
	__int32 clientNum;	//0x01DC 
	char _0x01E0[20];
	vec3_t vNewOrigin;	//0x01F4 
	char _0x0200[180];
	BYTE eType;			//0x02B4 
	char _0x02B5[11];
	__int32 weaponID;	//0x02C0 
	char _0x02C4[180];
	BYTE IsAlive;		//0x0378 
	char _0x0379[7];
};
extern centity_t* cg_entities;

struct cg_t
{
public:
	int iClientNum;
private:
	char _0x4[0x20];
public:
	snapshot_s* OldSnapShot;
	snapshot_s* NewSnapShot;
private:
	char _0x2C[0x58];
public:
	int iPing;
	int iServerTime;
	int iPhysicsTime;
	int iCommandTime;
private:
	char _0x94[0x24];
public:
	vec3_t vOrigin;
	vec3_t vVelocity;
private:
	char _0xD0[0x10];
public:
	int iWeaponDelay;
private:
	char _0xE4[0x78];
public:
	int iMoveType;
private:
	char _0x160[0x2C];
public:
	int iMoveFlag;
private:
	char _0x190[0xB8];
public:
	int iWeaponID;
private:
	char _0x24C[0x14];
public:
	int iWeaponState[2];
private:
	char _0x268[0x8];
public:
	float flZoomProgress;
private:
	char _0x274[0x54];
public:
	int iHealth;
private:
	char _0x2CC[0x47DB8];
public:
	float flFrameInterpolation;
	int iFrameTime;
	int iTime;
	int iOldTime;
private:
	char _0x48094[0x8];
public:
	int iThirdPerson;
private:
	char _0x480A0[0x8];
public:
	playerState_s playerState;
private:
	char _0x485FC[0x5294];
public:
	refdef_t RefDef;
	vec3_t vRefDefViewAngles;
private:
	char _0x648FC[0x130];
public:
	vec3_t vWeaponAngles;
private:
	char _0x64A38[0x3890];
public:
	float flZoomSensitivity;
private:
	char _0x682CC[0x68C];
public:
	int iMatchUIVisibilityFlags;
private:
	char _0x6895C[0xD18];
public:
	int iWeaponSelect;
private:
	char _0x69678[0x54];
public:
	int iEquippedOffhand;
private:
	char _0x696D0[0xF8];
public:
	vec3_t vKickAngles;
	vec3_t vOffsetAngles;
	float flGunPitch;
	float flGunYaw;
private:
	char _0x697E8[0xB4];
public:
	float flShockSensitivity;
private:
	char _0x698A0[0x1C0];
public:
	clientInfo_t clientInfo[MAX_ENTITIES];
private:
	char _0x72AF0[0xE04C];
public:
	int iExtraButtonBits[2];
private:
	char _0x80B44[0x8];
public:
	int iPlayerTeleported;
private:
	char _0x80B50[0xC];
public:
	float flSpread;
private:
	char _0x80B60[0x6BD0];
public:
	char szGameType[4];
private:
	char _0x87734[0x1D90C];
public:
	centity_t cent[MAX_ENTITIES];
};
extern cg_t* cg;

struct usercmd_s
{
public:
	std::int32_t serverTime;
	std::int32_t buttons[2];
	std::int32_t viewAngles[3];
	std::int32_t weapon;
	std::int32_t offHandWeapon;
	std::int32_t lastWeaponAltModeSwitch;
	char forwardMove;
	char rightMove;
private:
	byte _0x26[0x16];
};

struct clientActive_t
{
private:
	byte _0x0[0x68];
public:
	std::int32_t ping;
private:
	byte _0x6C[0x2C20];
public:
	vec3_t viewangles;                  // 0x2C8C
private:
	std::uint8_t _0x2C98[0x42CA8 - 0x2C98];
public:
	usercmd_s userCmd[128];            // 0x42CA8  
	std::int32_t currentCmd;           // 0x44AA8
	usercmd_s* GetUserCmd(int cmdNumber)
	{
		return &userCmd[cmdNumber & 0x7F];
	}

}; extern clientActive_t* clientActive;

struct ShaderConstantSet
{
	vec4_t value;
	int constantSource;
	bool used;
};

enum trace_hit_type_e
{
	TRACE_HITTYPE_NONE = 0x0,
	TRACE_HITTYPE_ENTITY = 0x1,
	TRACE_HITTYPE_DYNENT_MODEL = 0x2,
	TRACE_HITTYPE_DYNENT_BRUSH = 0x3,
	TRACE_HITTYPE_GLASS = 0x4,
};

namespace structures
{
	struct trace_t
	{
	public:
		vec3_t normal;       // 0x00

		char __pad[4];

		float fraction;             // 0x10
		int sflags;                 // 0x14
		int cflags;                 // 0x18
		trace_hit_type_e hitType;   // 0x1C

		std::uint16_t hitId;        // 0x20
		std::uint16_t modelIndex;   // 0x22
		std::uint16_t partName;     // 0x24
		std::uint16_t boneIndex;    // 0x26
		std::uint16_t partGroup;    // 0x28

		bool allsolid;              // 0x2A
		bool startsolid;            // 0x2B
		bool walkable;              // 0x2C
		char pad[0x13];
	};
}

namespace offsets
{
	constexpr auto CG_GetPlayerViewOrigin = 0x640E80;
	constexpr auto AimTarget_IsTargetVisible = 0x507EC0;
	constexpr auto CG_LocationalTrace = 0x50B870;
	constexpr auto EntityIsTeammate = 0x4309A0;
	constexpr auto CL_LocalClientIsInGame = 0x402F80;
	constexpr auto WorldPosToScreenPos = 0x429A80;
	constexpr auto Com_GetClientDObj = 0x4DA190;
	constexpr auto CG_DObjGetWorldTagPos = 0x4A1210;
	constexpr auto SL_FindLowercaseString = 0x5EE820;
	constexpr auto R_AddDObjSurfacesCamera = 0x728260;
}

// Function pointers
using CG_GetPlayerViewOrigin_t = bool(__cdecl*)(std::int32_t clientNum, playerState_s* ps, vec3_t* vOut);
using AimTarget_IsTargetVisible_t = bool(__cdecl*)(std::int32_t localClientNum, centity_t* targetEnt, std::uint32_t visBone);
using EntityIsTeammate_t = bool(__cdecl*)(std::int32_t clientNum, centity_t* entity);
using CL_LocalClientIsInGame_t = bool(__cdecl*)(std::int32_t localClientNum);
using CG_WorldPosToScreenPos_t = bool(__cdecl*)(std::int32_t clientNum, vec3_t worldPos, vec2_t screenPos);
using Com_GetClientDObj_t = void* (__cdecl*)(std::int32_t handle, std::int32_t localClientNum);
using CG_DObjGetWorldTagPos_t = bool(__cdecl*)(centity_t* a1, void* a2, WORD tag_name, std::float_t* pos);
using CG_LocationalTrace_t = void(__cdecl*)(structures::trace_t* trace, vec3_t* from, vec3_t* to, std::int32_t skip, std::int32_t mask, std::int32_t zero, std::int32_t _zero);
using RegisterTag_t = int(__cdecl*)(const char* a1, int a2);

using R_AddDObjToScene_t = int(__cdecl*)(int a1, centity_t* entity, int ent_num, UINT32 renderflags, float* a2, int* a3, int* a4, bool a5, int a6,
	ShaderConstantSet* scs, int a8, float a9, bool a10);

static R_AddDObjToScene_t R_AddDObjToScene = nullptr;

namespace functions
{
	static bool CG_GetPlayerViewOrigin(vec3_t* vOut)
	{
		return reinterpret_cast<CG_GetPlayerViewOrigin_t>(offsets::CG_GetPlayerViewOrigin)(0, &cg->playerState, vOut);
	}
	
	static bool AimTarget_IsTargetVisible(centity_t* targetEnt, std::uint32_t visibleBone)
	{
		return reinterpret_cast<AimTarget_IsTargetVisible_t>(offsets::AimTarget_IsTargetVisible)(0, targetEnt, visibleBone);
	}

	static bool EntityIsTeammate(centity_t* entity)
	{
		return reinterpret_cast<EntityIsTeammate_t>(offsets::EntityIsTeammate)(0, entity);
	}

	static bool CL_LocalClientIsInGame()
	{
		return reinterpret_cast<CL_LocalClientIsInGame_t>(offsets::CL_LocalClientIsInGame)(0);
	}

	static bool CG_WorldPosToScreenPos(vec3_t worldPos, vec2_t screenPos)
	{
		return reinterpret_cast<CG_WorldPosToScreenPos_t>(offsets::WorldPosToScreenPos)(0, worldPos, screenPos);
	}

	static void CG_LocationalTrace(structures::trace_t* trace, vec3_t* from, vec3_t* to, std::int32_t skip, std::int32_t mask, std::int32_t zero, std::int32_t _zero)
	{
		return reinterpret_cast<CG_LocationalTrace_t>(offsets::CG_LocationalTrace)(trace, from, to, skip, mask, zero, _zero);
	}

	static bool IsVisible(vec3_t& point)	// This is similar to AimTarget_IsTargetVisible but more precise. Pass in a player's tag as the parameter.
	{
		structures::trace_t t;
		int mask = ENTITY_TRACE_MASK; // which surface type to look for when the trace is ran.

		CG_LocationalTrace(&t, &cg->vOrigin, &point, 0u, mask, 0u, 0u);

		if (t.fraction >= 0.97f)
			return true;

		return false;
	}

	static void* Com_GetClientDObj(centity_t* entity)
	{
		return reinterpret_cast<Com_GetClientDObj_t>(offsets::Com_GetClientDObj)(entity->clientNum, 0);	// entity->NextEntityState.clientNum (0x1DC)
	}

	static bool CG_DObjGetWorldTagPos(centity_t* entity, void* DObj, std::uint32_t tag, std::float_t* vOut)
	{
		return reinterpret_cast<CG_DObjGetWorldTagPos_t>(offsets::CG_DObjGetWorldTagPos)(entity, DObj, tag, vOut);
	}

	static bool GetPlayerTag(centity_t* entity, WORD szBone, std::float_t* vOut)
	{
		if (!szBone)
			return false;

		void* DObj = Com_GetClientDObj(entity);

		if (!DObj)
			return false;

		return CG_DObjGetWorldTagPos(entity, DObj, szBone, vOut);
	}

	// IDA shows: (iClientNum * ci_stride) + (cg + ci_base)			in IsEntityFriendlyNotEnemy
	static clientInfo_t* GetClientInfo(cg_t* ptrCg, int iClientNum) 
	{
		int ci_base = 0x69A8C - 0x2C;		// cg_Array + 0x69A8C. 0x69A8C is ci->team, team's offset is 0x2C so we substract it
		int ci_stride = 0x808;				// sizeof clientInfo_t == 0x808
		uintptr_t base = (uintptr_t)ptrCg + ci_base;
		return (clientInfo_t*)(base + iClientNum * ci_stride);
	}

	static int RegisterTag(const char* a1, int a2) 
	{
		return reinterpret_cast<RegisterTag_t>(offsets::SL_FindLowercaseString)(a1, a2);
	}
}

// other structs (for chams)

/*
//=====================================================================================
*/
struct Picmip
{
	char platform[2];
};
/*
//=====================================================================================
*/
struct CardMemory
{
	int platform[2];
};
/*
//=====================================================================================
*/
struct GfxStreamedPartInfo
{
	unsigned int levelCountAndSize;
	unsigned int hash;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int32 offset : 32;
	unsigned __int32 size : 28;
	unsigned __int32 ipakIndex : 4;
	unsigned __int32 adjacentLeft : 15;
	unsigned __int32 adjacentRight : 15;
	unsigned __int32 compressed : 1;
	unsigned __int32 valid : 1;
};
/*
//=====================================================================================
*/
struct GfxSkinnedXModelSurfs
{
	void* firstSurf;
};
/*
//=====================================================================================
*/
struct GfxSceneEntityCull
{
	volatile unsigned int state;
	vec3_t mins;
	vec3_t maxs;
	char lods[32];
	GfxSkinnedXModelSurfs skinnedSurfs;
};
/*
//=====================================================================================
*/
struct GfxPlacement
{
	vec4_t quat;
	vec3_t origin;
};
/*
//=====================================================================================
*/
struct GfxScaledPlacement
{
	GfxPlacement base;
	float scale;
};
/*
//=====================================================================================
*/
struct GfxEntity
{
	unsigned int renderFxFlags;
	float materialTime;
	float destructibleBurnAmount;
	int textureOverrideIndex;
};
/*
//=====================================================================================
*/
struct GfxBackEndData
{
	char _pad0[0x56E8C];
	GfxEntity gfxEnts[256];   // 0x56E8C
};
/*
//=====================================================================================
*/
struct GfxSceneEntity
{
	char pad_0000[0x78];
	float lightingOriginToleranceSq;
	float invScaleSq;
	unsigned __int16 gfxEntIndex;
	unsigned __int16 gfxEntIndex2;
	unsigned __int16 entnum;                // 0x84
	char primaryLightIndex;
	char reflectionProbeIndex;
	char altXModelIndex;
	char entShaderConstantSetIndex;
	char useHeroLighting;
	char pad_008B;
};  // sizeof 0x8C
/*
//=====================================================================================
*/
struct GfxMatrix
{
	vec4_t m[4];
};
/*
//=====================================================================================
*/
struct __declspec(align(16)) GfxViewParms
{
	GfxMatrix viewMatrix;
	GfxMatrix projectionMatrix;
	GfxMatrix viewProjectionMatrix;
	GfxMatrix inverseViewProjectionMatrix;
	vec4_t origin;
	vec3_t axis[3];
	float depthHackNearClip;
	float zNear;
	float zFar;
	int bspCellIndex;
};
/*
//=====================================================================================
*/
struct XModelRigidCullInfoContext
{
	int cullInfoPos;
	unsigned int cullInfoBufSize;
	char cullFlags;
	unsigned int size;
};
/*
//=====================================================================================
*/
struct GfxDrawSurfFields
{
	unsigned __int64 objectId : 16;
	unsigned __int64 customIndex : 9;
	unsigned __int64 reflectionProbeIndex : 5;
	unsigned __int64 dlightMask : 2;
	unsigned __int64 materialSortedIndex : 12;
	unsigned __int64 primaryLightIndex : 8;
	unsigned __int64 surfType : 4;
	unsigned __int64 prepass : 2;
	unsigned __int64 primarySortKey : 6;
};
/*
//=====================================================================================
*/
union GfxDrawSurf
{
	GfxDrawSurfFields fields;
	unsigned __int64 packed;
};
/*
//=====================================================================================
*/
struct __declspec(align(8)) MaterialInfo
{
	const char* name;
	unsigned int gameFlags;
	char pad;
	char sortKey;
	char textureAtlasRowCount;
	char textureAtlasColumnCount;
	GfxDrawSurf drawSurf;
	unsigned int surfaceTypeBits;
	unsigned int layeredSurfaceTypes;
	unsigned __int16 hashIndex;
	int surfaceFlags;
	int contents;
};
/*
//=====================================================================================
*/
struct __declspec(align(4)) GfxImageLoadDef
{
	char levelCount;
	char flags;
	int format;
	int resourceSize;
	char data[1];
};
/*
//=====================================================================================
*/
union GfxTexture
{
	ID3D11ShaderResourceView* basemap;
	GfxImageLoadDef* loadDef;
};
/*
//=====================================================================================
*/
struct GfxImage
{
	GfxTexture texture;
	char mapType;
	char semantic;
	char category;
	bool delayLoadPixels;
	Picmip picmip;
	bool noPicmip;
	char track;
	CardMemory cardMemory;
	unsigned __int16 width;
	unsigned __int16 height;
	unsigned __int16 depth;
	char levelCount;
	char streaming;
	unsigned int baseSize;
	char* pixels;
	GfxStreamedPartInfo streamedParts[1];
	char streamedPartCount;
	unsigned int loadedSize;
	char skippedMipLevels;
	const char* name;
	unsigned int hash;
};
/*
//=====================================================================================
*/
struct MaterialTextureDef
{
	unsigned int nameHash;
	char nameStart;
	char nameEnd;
	char samplerState;
	char semantic;
	char isMatureContent;
	char pad[3];
	GfxImage* image;
};
/*
//=====================================================================================
*/
struct GfxStateBits
{
	unsigned int loadBits[2];
	ID3D11BlendState* blendState;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11RasterizerState* rasterizerState;
};
/*
//=====================================================================================
*/
struct $31FD236FB3888DAAD580490E35E6A96F
{
	char textureIndex;
	char samplerIndex;
};
/*
//=====================================================================================
*/
struct MaterialArgumentLocation
{
	unsigned __int16 offset;
	$31FD236FB3888DAAD580490E35E6A96F __s1;
};
/*
//=====================================================================================
*/
struct MaterialArgumentCodeConst
{
	unsigned __int16 index;
	char firstRow;
	char rowCount;
};
/*
//=====================================================================================
*/
union MaterialArgumentDef
{
	const float* literalConst;
	MaterialArgumentCodeConst codeConst;
	unsigned int codeSampler;
	unsigned int nameHash;
};
/*
//=====================================================================================
*/
struct GfxPixelShaderLoadDef
{
	char* program;
	unsigned int programSize;
};
/*
//=====================================================================================
*/
struct MaterialPixelShaderProgram
{
	ID3D11PixelShader* ps;
	GfxPixelShaderLoadDef loadDef;
};
/*
//=====================================================================================
*/
struct MaterialStreamRouting
{
	char source;
	char dest;
};
/*
//=====================================================================================
*/
struct MaterialVertexStreamRouting
{
	MaterialStreamRouting data[16];
	ID3D11InputLayout* decl[20];
};
/*
//=====================================================================================
*/
struct GfxVertexShaderLoadDef
{
	char* program;
	unsigned int programSize;
};
/*
//=====================================================================================
*/
struct MaterialVertexShaderProgram
{
	ID3D11VertexShader* vs;
	GfxVertexShaderLoadDef loadDef;
};
/*
//=====================================================================================
*/
struct MaterialShaderArgument
{
	unsigned __int16 type;
	MaterialArgumentLocation location;
	unsigned __int16 size;
	unsigned __int16 buffer;
	MaterialArgumentDef u;
};
/*
//=====================================================================================
*/
struct MaterialPixelShader
{
	const char* name;
	MaterialPixelShaderProgram prog;
};
/*
//=====================================================================================
*/
struct MaterialVertexDeclaration
{
	char streamCount;
	bool hasOptionalSource;
	bool isLoaded;
	MaterialVertexStreamRouting routing;
};
/*
//=====================================================================================
*/
struct MaterialVertexShader
{
	const char* name;
	MaterialVertexShaderProgram prog;
};
/*
//=====================================================================================
*/
union $55088C612251863DED87690D669FF6D2
{
	MaterialPixelShader* pixelShader;
	MaterialPixelShader* localPixelShader;
};
/*
//=====================================================================================
*/
union $F01209007E6D5437D99F198F0FAAA1A0
{
	MaterialShaderArgument* localArgs;
	MaterialShaderArgument* args;
};
/*
//=====================================================================================
*/
struct MaterialPass
{
	MaterialVertexDeclaration* vertexDecl;
	MaterialVertexShader* vertexShader;
	$55088C612251863DED87690D669FF6D2 ___u2;
	char perPrimArgCount;
	char perObjArgCount;
	char stableArgCount;
	char customSamplerFlags;
	char precompiledIndex;
	char materialType;
	$F01209007E6D5437D99F198F0FAAA1A0 ___u9;
};
/*
//=====================================================================================
*/
struct MaterialTechnique
{
	const char* name;
	unsigned __int16 flags;
	unsigned __int16 passCount;
	MaterialPass passArray[1];
};
/*
//=====================================================================================
*/
struct MaterialTechniqueSet
{
	const char* name;
	char worldVertFormat;
	MaterialTechnique* techniques[36];
};
/*
//=====================================================================================
*/
struct MaterialConstantDef
{
	unsigned int nameHash;
	char name[12];
	vec4_t literal;
};
/*
//=====================================================================================
*/
struct Material
{
	MaterialInfo info;
	char stateBitsEntry[36];
	char textureCount;
	char constantCount;
	char stateBitsCount;
	char stateFlags;
	char cameraRegion;
	char probeMipBits;
	union $93500E63A3928317F74DE3F1B0115B10
	{
		MaterialTechniqueSet* localTechniqueSet;
		MaterialTechniqueSet* techniqueSet;
	};
	MaterialTextureDef* textureTable;
	union $9B20CC24F769322148675104C79675BA
	{
		MaterialConstantDef* localConstantTable;
		MaterialConstantDef* constantTable;
	};
	GfxStateBits* stateBitsTable;
	Material* thermalMaterial;
};
extern Material* g_mtlCopy;