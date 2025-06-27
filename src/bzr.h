/* Copyright (C) 2023-2025 VTrider
 *
 * This file is part of Extra Utilities.
 *
 * Extra Utilities is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
* Structs and memory offsets for BZR 2.2.301
*/

#pragma once

#include <cstdint>

#include "Vec3.h"

namespace BZR
{
	// Typedefs
	using handle = unsigned int; // internally handles are unsigned ints

	// Forward declarations
	class GameObject;
	using GameObjectClass = void;
	using tagENTITY = void;
	class OBJ76;
	using AiProcess = void;
	using VECTOR_3D = ExtraUtilities::Vec3;

	using pure_virtual_t = void(__thiscall*)(void);

	struct Jammer
	{
		float maxSpeed;
		GameObject* owner;
	};

	struct Scanner
	{
		float range;
		float period;
		bool active;
		float sweep;
		GameObject* owner;
	};

	struct MAT_3D
	{
		float right_x;
		float right_y;
		float right_z;
		float up_x;
		float up_y;
		float up_z;
		float front_x;
		float front_y;
		float front_z;
		uint8_t padding[4];
		double posit_x;
		double posit_y;
		double posit_z;
	};

	using Mat3 = MAT_3D;

	//struct VECTOR_3D
	//{
	//	VECTOR_3D() : x(0), y(0), z(0) {}
	//	VECTOR_3D(float x, float y, float z) : x(x), y(y), z(z) {}
	//	VECTOR_3D(double x, double y, double z) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)) {}
	//	float x, y, z;
	//};

	struct VECTOR_3D_LONG
	{
		double x, y, z;
		VECTOR_3D_LONG() : x(0), y(0), z(0) {}
		VECTOR_3D_LONG(double x, double y, double z) : x(x), y(y), z(z) {}
		ExtraUtilities::Vec3 ToVec()
		{
			return ExtraUtilities::Vec3(x, y, z);
		}
	};

	struct Frustum
	{
		VECTOR_3D near_top_left;
		VECTOR_3D near_top_right;
		VECTOR_3D near_bottom_right;
		VECTOR_3D near_bottom_left;
		VECTOR_3D far_top_left;
		VECTOR_3D far_top_right;
		VECTOR_3D far_bottom_right;
		VECTOR_3D far_bottom_left;
	};

	class BZR_Camera // todo merge camera namespaces
	{
	public:
		float Orig_x;
		float Orig_y;
		float Const_x;
		float Const_y;
		float Max_Depth;
		float Left;
		float Bottom;
		float Right;
		float Top;
		float View_Angle;
		float Tang;
		float Aspect;
		float Zoom_Factor;
		void (*Draw_Poly)(void);
		void* Buffer; // _GRAPHIC_BUFFER*
		MAT_3D Matrix;
		VECTOR_3D_LONG bSphere_Center;
		double bSphere_Radius;
		uint8_t View_Volume[0x60]; // plane class
		Frustum View_Frustum;
		VECTOR_3D_LONG View_Pyramid[5];
	};

	namespace Camera
	{
		inline auto View_Record_MainCam = (BZR_Camera*)0x008EAAE0;

		inline auto zoomFactorFPP = (float*)0x008EAD10;
		inline auto zoomFactorTPP = (float*)0x008EAB10;
		inline auto maxZoomFactor = (float*)0x008A2688;
		inline auto minZoomFactor = (float*)0x008A25FC;
		inline auto viewFrustum = (void*)0x008EABE0;

		enum View
		{
			// Camera types
			FIRST_PERSON = 0x100,
			THIRD_PERSON = 0x101,

			// SetView Values

			// User assignable views
			COCKPIT = 1, // F1
			NO_COCKPIT = 2, // F2
			CHASE = 4, // F3
			ORBIT = 5, // F4
			NO_HUD = 3, // F5
			EDITOR = 6, // F9
			CHEAT_SATELLITE = 7, // F10
			FREECAM = 9, // F11

			TOGGLE_SATELLITE = 8, // 9 key with satellite built and powered
			TERRAIN_EDIT = 0x2A // CTRL+E
		};

		inline auto currentView = (int*)0x02CECEA0;

		using _Set_View = void (__cdecl*) (tagENTITY*, int); // 2nd param is an enum
		inline _Set_View Set_View = (_Set_View)0x0061D120;
	}
	using _Matrix_Inverse = void (__cdecl*) (MAT_3D* returnStoragePointer, MAT_3D* matToInverse);
	inline _Matrix_Inverse Matrix_Inverse = (_Matrix_Inverse)0x008203F0;

	using _Vector_Unrotate = void (__cdecl*)(VECTOR_3D* returnStoragePointer, VECTOR_3D* inputVector, MAT_3D* perspectiveMatrix);
	inline _Vector_Unrotate Vector_Unrotate = (_Vector_Unrotate)0x00440300;

	namespace Cheats
	{
		inline auto editMode = (bool*)0x009454B8;
	}

	class ControlPanel
	{
	public:
		static inline auto p_controlPanel = (ControlPanel*)0x00978E20;

		using _SelectOne = void(__thiscall*)(ControlPanel*, GameObject*);
		static inline _SelectOne SelectOne = (_SelectOne)0x004A6CD0;

		using _SelectNone = void(__thiscall*)(ControlPanel*);
		static inline _SelectNone SelectNone = (_SelectNone)0x004A6D50;

		using _SelectAdd = void(__thiscall*)(ControlPanel*, GameObject*);
		static inline _SelectAdd SelectAdd = (_SelectAdd)0x004a6c70;
	};

	namespace Environment
	{
		inline auto gravityVector = (VECTOR_3D*)0x00871A80;
	}

	struct EULER {
		float mass;
		float mass_inv;
		float v_mag;
		float v_mag_inv;
		float I;
		float k_i;
		VECTOR_3D v;
		VECTOR_3D omega;
		VECTOR_3D Accel;
		VECTOR_3D Alpha;
	};

	using Euler = EULER;

	class GameObject
	{
	public:
		using _GetHandle = handle(__thiscall*)(GameObject*);
		static inline _GetHandle GetHandle = (_GetHandle)(0x00462380);

		// Credit to Janne for this function -VT
		static GameObject* GetObj(handle h)
		{
			return (GameObject*)(((h >> 0x14) * 0x400) + 0x260DB20);
		}

		using _SetAsUser = void(__thiscall*)(GameObject*);
		static inline _SetAsUser SetAsUser = (_SetAsUser)0x004DB930;

		// Use this to determine if you are in game since player will become null
		// after exiting a map
		static inline auto p_userObject = (void*)0x00917AFC;

		// the naming is based off the 1.5 pdb, the inconsistency is intentional
		static inline auto user_entity_ptr = (tagENTITY**)0x00920c78;

		uintptr_t vftableAttachable;
		uint8_t padding_1[0x14];
		uintptr_t vftableDistributedObject;
		uint8_t padding_2[0xD0];
		GameObjectClass* curPilot;
		tagENTITY* ent;
		OBJ76* obj;
		GameObjectClass* objClass;
		AiProcess* aiProcess;
		const char* label;
		int independence;
		VECTOR_3D pos;
		EULER euler;

		// offset to scanner: 0x198
		// offset to jammer: 0x19C
		// offset to carrier: 0x1A0

		// Gets the scanner object for a GameObject (the radar controller)
		BZR::Scanner* GetScanner()
		{
			BZR::Scanner* scanner;

			// didn't want to deal with pointer casting on the
			// unaligned gameobject class so here's some asm
			__asm
			{
				mov ecx, [ecx + 0x198] // ecx is always the this pointer
				mov [scanner], ecx
			}

			return scanner;
		}

		// Gets the jammer object for a GameObject (velocjam controller)
		BZR::Jammer* GetJammer()
		{
			BZR::Jammer* jammer;

			__asm
			{
				mov ecx, [ecx + 0x19C]
				mov [jammer], ecx
			}

			return jammer;
		}

		// Offsets to get to the ogre light object from GameObject* -> f0 -> a8
		void* GetLight()
		{
			void* light;
			__asm
			{
				mov ecx, [ecx+0xf0]
				mov ecx, [ecx+0xa8]
				mov [light], ecx
			}
			return light;
		}
	};

	namespace GraphicsOptions
	{
		inline auto isFullscreen = (bool*)0x009183B8;
		inline auto uiScaling = (int*)0x008E77A8;
	}

	namespace Multiplayer
	{
		inline auto isNetGame = (bool*)0x00917f7b;

		// Real life counter, does not update the scoreboard in real time however
		inline auto lives = (int*)0x008E8D04;

		inline auto myNetID = (uint8_t*)0x009180D4; // ID that's used with Send() and Receive()

		inline auto showScoreboard = (bool*)0x02A17494;

		// Call this function to update the scoreboard with the current life count
		using _UpdateLives = void(*)(void);
		inline _UpdateLives UpdateLives = (_UpdateLives)0x006260f0;
	}

	class OBJ76
	{
	public:
		uint8_t pad_1[0x20];
		Mat3 transform;
		uint8_t pad_2[44];
		GameObject* owner;
	};

	namespace Ogre
	{
		// Function offsets from OgreMain.dll
		constexpr uintptr_t getAmbientLightOffset = 0x3C8D90;
		constexpr uintptr_t setAmbientLightOffset = 0x3D35E0;
		constexpr uintptr_t getDiffuseColorOffset = 0x18219;
		constexpr uintptr_t setDiffuseColorOffset = 0x220280;
		constexpr uintptr_t getSpecularColorOffset = 0x21F490;
		constexpr uintptr_t setSpecularColorOffset = 0x2204B0;
		constexpr uintptr_t setSpotlightRangeOffset = 0x220550;
		constexpr uintptr_t setVisibleOffset = 0x220EF0;

		// Absolute address
		inline auto terrain_masterlight = (void**)0x00920CA0; // pointer to the sun light object

		inline auto sceneManagerStructure = (void**)0x00920EA0; // base of some ogre structure that leads to scene manager
		constexpr uintptr_t sceneManagerOffset = 0x08;
	}

	using DAMAGE = uint8_t[0x10];
	using gas_object = void;
	using ParticleRenderPointer = uint8_t[0x04];
	using CSteamID = uint64_t;
	using ExplosionClass = void;
	using ParticleRenderClass = void;

	class Ordnance; // Forward declaration

	class OrdnanceClass
	{
	private:
		using _Build = Ordnance*(__thiscall*)(OrdnanceClass* c, Mat3* mat, OBJ76* owner);

	public:
		uint8_t padding_1[0x08];
		OrdnanceClass* proto;
		uint32_t sig;
		char* label;
		uint8_t padding_2[0x04];
		uint64_t cfg;
		char odf[0x10];
		OBJ76* ord;
		tagENTITY* ent;
		OBJ76* freeOrd;
		ExplosionClass* xplGround;
		ExplosionClass* xplVehicle;
		ExplosionClass* xplBuilding;
		int32_t ammoCost;
		float lifeSpan;
		float shotSpeed;
		float damageValue;
		uint16_t damageTypes;
		bool notifyRemote;
		char shotSound[0x10];
		uint8_t padding_3;
		ParticleRenderClass* renderClass;
	
		static constexpr uintptr_t OrdnanceClassList = 0x009C915C;
		static inline _Build Build = (_Build)0x00586ff0;
	};

#pragma pack(push, 1)
	class Ordnance
	{
	private:
		struct vftable_t
		{
			void(__thiscall* scalar_deleting_destructor)(Ordnance* self, uint32_t param_1);
			void(__thiscall* Init)(Ordnance* self, Mat3* mat, OBJ76* obj);
			pure_virtual_t Cleanup;
			pure_virtual_t Control;
			pure_virtual_t Simulate;
			pure_virtual_t Hit;
			pure_virtual_t Submit;
			pure_virtual_t Pack;
			pure_virtual_t Unpack;
		};

	public:
		vftable_t* vftable;
		uint8_t padding_1[0x08];
		OrdnanceClass* ordnanceClass;
		float dt;
		OBJ76* obj;
		Euler euler;
		DAMAGE damage;
		gas_object* go;
		ParticleRenderPointer renderObj;
		float lifeTime;
		CSteamID source;
		int32_t bSend;
		Mat3 initMat;
		float initTime;
		void* OgreEntity;
		void* OgreNode;
		void* OgreSkeleton;
		OBJ76* owner;
		int32_t ownerHandle;

		static inline auto coeffBallistic = (float*)0x008A2858;
	};
#pragma pack(pop)

	namespace PlayOption
	{
		inline auto userProfilePtr = (void*)0x0094672C;
		inline uint8_t playOptionOffset = 0x30;

		inline auto difficulty = (uint8_t*)0x25CFA1C;
	}

	namespace Satellite
	{
		inline auto state = (bool*)0x008E8F9C; // old value bugged in MP 0x00917AF8
		inline auto cursorPos = (VECTOR_3D*)0x009C9194;
		inline auto camPos = (VECTOR_3D*)0x009C91B4;
		inline auto clickPos = (VECTOR_3D*)0x009C9188;
		inline auto panSpeed = (float*)0x009C91D0;
		inline auto minZoom = (float*)0x00872400;
		inline auto maxZoom = (float*)0x008723F4;
		inline auto zoom = (float*)0x009C91B0;
	}

	namespace SoundOptions
	{
		inline auto soundStruct1 = (uint8_t*)0x0094672C; // this points to the music *display* value
		inline auto soundStruct2 = (uint32_t*)0x00915594; // this points to the sfx and voice *real* values
		inline uint8_t musicOffset = 0x2A;
		inline uint8_t sfxOffset = 0x08;
		inline uint8_t voiceOffset = 0x0C;
	}

	namespace Steam
	{
		inline auto steam64 = (uint64_t*)0x0260B1D0;
	}

	namespace Radar
	{
		inline auto state = (uint8_t*)0x008EAAAC;
	}

	namespace Reticle
	{
		inline auto angle = (float*)0x025CE714;
		inline auto position = (VECTOR_3D*)0x025CE79C;
		inline auto range = (float*)0x00886B20;
		inline auto object = (int*)0x00979F40;
		inline auto matrix = (MAT_3D*)0x025CE6F8;
	}
}