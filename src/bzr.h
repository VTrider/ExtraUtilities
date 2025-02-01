#pragma once

#include <cstdint>

namespace BZR
{
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

	namespace Environment
	{
		inline auto gravityVector = (VECTOR_3D*)0x00871A80;
	}

	class GameObject
	{
	public:
		using _GetHandle = unsigned int(__thiscall*)(int);
		static inline _GetHandle GetHandle = (_GetHandle)(0x00462380);

		static inline auto p_localPlayer = (void*)0x00917AFC;
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

	namespace Reticle
	{
		inline auto angle = (float*)0x025CE714;
		inline auto position = (BZR::VECTOR_3D*)0x025CE79C;
		inline auto range = (float*)0x00886B20;
		inline auto object = (GameObject**)0x00979F40;
		inline auto matrix = (void*)0x025CE6F8;
	}


}

