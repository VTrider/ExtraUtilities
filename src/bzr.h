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

	struct MAT_3D {
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
	};

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

