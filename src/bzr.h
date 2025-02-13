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

namespace BZR
{
	// Forward declarations
	class GameObject;

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

	struct VECTOR_3D
	{
		float x, y, z;
	};

	namespace Camera
	{
		inline auto zoomFactorFPP = (float*)0x008EAD10;
		inline auto zoomFactorGlobal = (float*)0x008EAB10;
		inline auto maxZoomFactor = (float*)0x008A2688;
		inline auto minZoomFactor = (float*)0x008A25FC;
		inline auto viewFrustum = (void*)0x008EABE0;
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

	class GameObject
	{
	public:
		using _GetHandle = unsigned int(__thiscall*)(int);
		static inline _GetHandle GetHandle = (_GetHandle)(0x00462380);

		// Credit to Janne for this function -VT
		static GameObject* GetObj(unsigned int handle)
		{
			return (GameObject*)(((handle >> 0x14) * 0x400) + 0x260DB20);
		}

		using _SetAsUser = void(__thiscall*)(GameObject*);
		static inline _SetAsUser SetAsUser = (_SetAsUser)0x004DB930;

		// Use this to determine if you are in game since player will become null
		// after exiting a map
		static inline auto p_userObject = (void*)0x00917AFC;

		using GameObjectClass = void;
		using tagENTITY = void;
		using OBJ76 = void;
		using AiProcess = void;

		uintptr_t vftableAttachable;
		uint8_t padding_1[0x14];
		uintptr_t vftableDistributedObject;
		uint8_t padding_2[0xD4];
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
				mov[jammer], ecx
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

	namespace Multiplayer
	{
		inline auto isNetGame = (bool*)0x00917f7b;

		// Real life counter, does not update the scoreboard in real time however
		inline auto lives = (int*)0x008E8D04;

		// Call this function to update the scoreboard with the current life count
		using _UpdateLives = void(*)(void);
		inline _UpdateLives UpdateLives = (_UpdateLives)0x006260f0;
	}

	namespace Ogre
	{
		// Function offsets from OgreMain.dll
		constexpr uintptr_t setDiffuseColorOffset = 0x220280;
		constexpr uintptr_t setSpecularColorOffset = 0x2204B0;
		constexpr uintptr_t setSpotlightRangeOffset = 0x220550;
		constexpr uintptr_t setVisibleOffset = 0x220EF0;
	}

	class Ordnance
	{
	public:
		static inline auto coeffBallistic = (float*)0x008A2858;
	};

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
		inline auto object = (GameObject**)0x00979F40;
		inline auto matrix = (MAT_3D*)0x025CE6F8;
	}
}

