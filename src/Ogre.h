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

#pragma once

#include "BZR.h"
#include "Scanner.h"

#include <cstdint>
#include <memory>

namespace ExtraUtilities::Ogre
{
	struct Fog
	{
		float r;
		float g;
		float b;
		uint8_t padding[4] = { 0 };
		float start;
		float ending;

		Fog() = default;
		Fog(float r, float g, float b, float start, float ending)
			: r(r), g(g), b(b), start(start), ending(ending) {}
	};

	struct Color
	{
		float r, g, b;
	};

	inline Scanner terrain_masterlight(BZR::Ogre::terrain_masterlight, BasicScanner::Restore::DISABLED);
	inline Scanner sceneManager(BZR::Ogre::sceneManagerStructure, { BZR::Ogre::sceneManagerOffset }, BasicScanner::Restore::DISABLED);

	// Gets the fog structure from the scene manager
	inline Fog* GetFog()
	{
		void* sm = sceneManager.Read();
		Fog* fog;
		__asm
		{
			mov eax, [sm]
			add eax, 0x128
			mov [fog], eax
		}
		return fog;
	}

	// Function Pointers

	// This is bordering on voodoo

	using enum BasicScanner::BaseAddress;

	inline uintptr_t getAmbientLightAddr = BasicScanner::CalculateAddress(BZR::Ogre::getAmbientLightOffset, OGRE);
	using _GetAmbientLight = Color*(__thiscall*)(void*);
	inline _GetAmbientLight GetAmbientLight = (_GetAmbientLight)getAmbientLightAddr;

	inline uintptr_t setAmbientLightAddr = BasicScanner::CalculateAddress(BZR::Ogre::setAmbientLightOffset, OGRE);
	using _SetAmbientLight = void(__thiscall*)(void*, Color*);
	inline _SetAmbientLight SetAmbientLight = (_SetAmbientLight)setAmbientLightAddr;

	inline uintptr_t getDiffuseColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::getDiffuseColorOffset, OGRE);
	using _GetDiffuseColor = Color*(__thiscall*)(void*);
	inline _GetDiffuseColor GetDiffuseColor = (_GetDiffuseColor)getDiffuseColorAddr;

	inline uintptr_t setDiffuseColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setDiffuseColorOffset, OGRE);
	using _SetDiffuseColor = void(__thiscall*)(void*, float, float, float);
	inline _SetDiffuseColor SetDiffuseColor = (_SetDiffuseColor)setDiffuseColorAddr;

	inline uintptr_t getSpecularColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::getSpecularColorOffset, OGRE);
	using _GetSpecularColor = Color*(__thiscall*)(void*);
	inline _GetSpecularColor GetSpecularColor = (_GetDiffuseColor)getSpecularColorAddr;

	inline uintptr_t setSpecularColorAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpecularColorOffset, OGRE);
	using _SetSpecularColor = void(__thiscall*)(void*, float, float, float);
	inline _SetSpecularColor SetSpecularColor = (_SetSpecularColor)setSpecularColorAddr;

	inline uintptr_t setSpotlightRangeAddr = BasicScanner::CalculateAddress(BZR::Ogre::setSpotlightRangeOffset, OGRE);
	using _SetSpotlightRange = void(__thiscall*)(void*, float*, float*, float);
	inline _SetSpotlightRange SetSpotlightRange = (_SetSpotlightRange)setSpotlightRangeAddr;

	inline uintptr_t setVisibleAddr = BasicScanner::CalculateAddress(BZR::Ogre::setVisibleOffset, OGRE);
	using _SetVisible = void(__thiscall*)(void*, bool);
	inline _SetVisible SetVisible = (_SetVisible)setVisibleAddr;
	// Late Binding - Resolved at Runtime to avoid linking against OgreMain.dll directly
	namespace LateBinding
	{
		enum class PolygonMode { PM_POINTS = 1, PM_WIREFRAME = 2, PM_SOLID = 3 };

		enum class OperationType {
			OT_POINT_LIST = 1,
			OT_LINE_LIST = 2,
			OT_LINE_STRIP = 3,
			OT_TRIANGLE_LIST = 4,
			OT_TRIANGLE_STRIP = 5,
			OT_TRIANGLE_FAN = 6
		};
		
		// Function Pointers
		using _SetPolygonMode = void(__thiscall*)(void*, PolygonMode);
		inline _SetPolygonMode SetPolygonMode = nullptr;

		// createManualObject(void) overload
		using _CreateManualObject = void*(__thiscall*)(void*); 
		inline _CreateManualObject CreateManualObject = nullptr; 

		// destroyManualObject(ManualObject*)
		using _DestroyManualObject = void(__thiscall*)(void*, void*);
		inline _DestroyManualObject DestroyManualObject = nullptr;

		// ManualObject methods
		// begin(const String& materialName, OperationType opType, const String& groupName)
		// We use void* for String& to avoid ABI issues. We will pass addresses of existing Ogre strings.
		using _Begin = void(__thiscall*)(void*, void*, OperationType, void*);
		inline _Begin Begin = nullptr;

		using _Position = void(__thiscall*)(void*, float, float, float);
		inline _Position Position = nullptr;

		using _Colour = void(__thiscall*)(void*, float, float, float, float);
		inline _Colour Colour = nullptr;

		using _End = void(__thiscall*)(void*); // Returns section but we ignore it
		inline _End End = nullptr;

		using _Clear = void(__thiscall*)(void*);
		inline _Clear Clear = nullptr;


		// Static String Addresses
		inline void* str_BLANK = nullptr;
		inline void* str_DEFAULT_RESOURCE_GROUP_NAME = nullptr;

		// std::string ABI workarounds
		// ctor(char*)
		using _ConstructString = void(__thiscall*)(void*, const char*);
		inline _ConstructString ConstructString = nullptr;

		// dtor()
		using _DestructString = void(__thiscall*)(void*);
		inline _DestructString DestructString = nullptr;

		inline bool InitOgreHooks()
		{
			HMODULE hOgre = GetModuleHandleA("OgreMain.dll");
			if (!hOgre) return false;

			SetPolygonMode = (_SetPolygonMode)GetProcAddress(hOgre, "?setPolygonMode@Camera@Ogre@@QAEXW4PolygonMode@2@@Z");
			CreateManualObject = (_CreateManualObject)GetProcAddress(hOgre, "?createManualObject@SceneManager@Ogre@@UAEPAVManualObject@2@XZ"); 
			DestroyManualObject = (_DestroyManualObject)GetProcAddress(hOgre, "?destroyManualObject@SceneManager@Ogre@@UAEXPAVManualObject@2@@Z");
			
			Begin = (_Begin)GetProcAddress(hOgre, "?begin@ManualObject@Ogre@@UAEXABVString@2@W4OperationType@RenderOperation@2@0@Z"); 
			// Wait, I need the EXACT string from the file for 'begin'. 
			// See verify step below.
			
			Position = (_Position)GetProcAddress(hOgre, "?position@ManualObject@Ogre@@UAEXMMM@Z");
			Colour = (_Colour)GetProcAddress(hOgre, "?colour@ManualObject@Ogre@@UAEXMMMM@Z"); // 4 floats
			End = (_End)GetProcAddress(hOgre, "?end@ManualObject@Ogre@@UAEPAVManualObjectSection@12@XZ");
			Clear = (_Clear)GetProcAddress(hOgre, "?clear@ManualObject@Ogre@@UAEXXZ");

			str_BLANK = (void*)GetProcAddress(hOgre, "?BLANK@StringUtil@Ogre@@2V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@B");
			if (!str_BLANK) // Try fallback if mangling differs
				str_BLANK = (void*)GetProcAddress(hOgre, "?BLANK@StringUtil@Ogre@@2VString@2@B");

			str_DEFAULT_RESOURCE_GROUP_NAME = (void*)GetProcAddress(hOgre, "?DEFAULT_RESOURCE_GROUP_NAME@ResourceGroupManager@Ogre@@2V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@A");

			// std::string ctor/dtor
			// Try to find them. If found, we can be safe. If not, we rely on existing strings.
			ConstructString = (_ConstructString)GetProcAddress(hOgre, "??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z");
			DestructString = (_DestructString)GetProcAddress(hOgre, "??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ");

			return SetPolygonMode && CreateManualObject && Begin && Position && End;
		}
	}
}