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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "Renderer.h"
#include "Ogre.h"
#include "LuaHelpers.h"

namespace ExtraUtilities::Lua::Renderer
{
	// Buffer for std::string ABI safety
	struct OgreString { char data[64]; }; // Enough for 32/64-bit std::string

	static bool g_isWireframe = false;
	static void* g_debuggerManualObject = nullptr;

	static bool EnsureOgreInitialized()
	{
		static bool initialized = false;
		if (!initialized)
		{
			if (Ogre::LateBinding::InitOgreHooks())
			{
				initialized = true;
				// Create the debugger object once
				// We need the SceneManager instance.
				// But we only have the *address* of the pointer to it?
				// BZR::Ogre::sceneManager is a Scanner object.
				// We need to read it.
			}
		}
		return initialized;
	}

	static void* GetSceneManager()
	{
		// sceneManager is a Scanner. Read() returns void*.
		// The pointer it reads IS the SceneManager*.
		return Ogre::sceneManager.Read();
	}

	// Helper to construct a string on the stack and return a pointer to it
	// If ctor is missing, returns str_BLANK (empty string/default material)
	static void* CreateOgreString(OgreString& buffer, const char* text)
	{
		if (Ogre::LateBinding::ConstructString && text)
		{
			Ogre::LateBinding::ConstructString(&buffer, text);
			return &buffer;
		}
		return Ogre::LateBinding::str_BLANK;
	}

	static void DestroyOgreString(OgreString& buffer)
	{
		if (Ogre::LateBinding::ConstructString && Ogre::LateBinding::DestructString)
		{
			Ogre::LateBinding::DestructString(&buffer);
		}
	}

	int SetWireframe(lua_State* L)
	{
		if (!EnsureOgreInitialized()) return 0;

		g_isWireframe = lua_toboolean(L, 1);
		
		void* cam = BZR::Camera::View_Record_MainCam;
		if (cam)
		{
			// Ogre::PolygonMode PM_SOLID=3, PM_WIREFRAME=2
			auto mode = g_isWireframe ? Ogre::LateBinding::PolygonMode::PM_WIREFRAME : Ogre::LateBinding::PolygonMode::PM_SOLID;
			Ogre::LateBinding::SetPolygonMode(cam, mode);
		}
		
		return 0;
	}

	int GetWireframe(lua_State* L)
	{
		lua_pushboolean(L, g_isWireframe);
		return 1;
	}

	int ClearVisuals(lua_State* L)
	{
		(void)L;
		if (g_debuggerManualObject && Ogre::LateBinding::Clear)
		{
			Ogre::LateBinding::Clear(g_debuggerManualObject);
		}
		return 0;
	}

	int DrawLine(lua_State* L)
	{
		if (!EnsureOgreInitialized()) return 0;

		BZR::VECTOR_3D start = CheckVectorOrSingles(L, 1);
		BZR::VECTOR_3D end = CheckVectorOrSingles(L, 2);
		Ogre::Color color = CheckColorOrSingles(L, 3);

		if (!g_debuggerManualObject)
		{
			void* sm = GetSceneManager();
			if (sm && Ogre::LateBinding::CreateManualObject)
			{
				g_debuggerManualObject = Ogre::LateBinding::CreateManualObject(sm);
				// We should probably set it to dynamic if we update it often
				// But for now, just clearing and rebuilding is fine for simple debug
			}
		}

		if (g_debuggerManualObject)
		{
			OgreString matNameBuf;
			void* matName = CreateOgreString(matNameBuf, "BaseWhiteNoLighting");

			// Begin(mo, material, OT_LINE_LIST, group)
			// group is DEFAULT
			Ogre::LateBinding::Begin(g_debuggerManualObject, matName, Ogre::LateBinding::OperationType::OT_LINE_LIST, Ogre::LateBinding::str_DEFAULT_RESOURCE_GROUP_NAME);
			
			Ogre::LateBinding::Position(g_debuggerManualObject, start.x, start.y, start.z);
			Ogre::LateBinding::Colour(g_debuggerManualObject, color.r, color.g, color.b, 1.0f);

			Ogre::LateBinding::Position(g_debuggerManualObject, end.x, end.y, end.z);
			Ogre::LateBinding::Colour(g_debuggerManualObject, color.r, color.g, color.b, 1.0f);

			Ogre::LateBinding::End(g_debuggerManualObject);

			DestroyOgreString(matNameBuf);
		}
		
		return 0;
	}

	int DrawBox(lua_State* L)
	{
		if (!EnsureOgreInitialized()) return 0;

		BZR::VECTOR_3D center = CheckVectorOrSingles(L, 1);
		BZR::VECTOR_3D extents = CheckVectorOrSingles(L, 2); // half-extents or full? Assume full size for now
		Ogre::Color color = CheckColorOrSingles(L, 3);

		if (!g_debuggerManualObject)
		{
			void* sm = GetSceneManager();
			if (sm && Ogre::LateBinding::CreateManualObject)
				g_debuggerManualObject = Ogre::LateBinding::CreateManualObject(sm);
		}

		if (g_debuggerManualObject)
		{
			OgreString matNameBuf;
			void* matName = CreateOgreString(matNameBuf, "BaseWhiteNoLighting");

			Ogre::LateBinding::Begin(g_debuggerManualObject, matName, Ogre::LateBinding::OperationType::OT_LINE_LIST, Ogre::LateBinding::str_DEFAULT_RESOURCE_GROUP_NAME);

			// 8 corners
			float x = extents.x / 2.0f;
			float y = extents.y / 2.0f;
			float z = extents.z / 2.0f;

			// A simple box is 12 lines
			// ... implementation omitted for brevity, adding just vertices ... 
			// Actually let's do it properly
			BZR::VECTOR_3D c = center;
			BZR::VECTOR_3D corners[8] = {
				{c.x - x, c.y - y, c.z - z}, {c.x + x, c.y - y, c.z - z},
				{c.x + x, c.y + y, c.z - z}, {c.x - x, c.y + y, c.z - z},
				{c.x - x, c.y - y, c.z + z}, {c.x + x, c.y - y, c.z + z},
				{c.x + x, c.y + y, c.z + z}, {c.x - x, c.y + y, c.z + z}
			};

			// Bottom face
			auto AddLine = [&](int i, int j) {
				Ogre::LateBinding::Position(g_debuggerManualObject, corners[i].x, corners[i].y, corners[i].z);
				Ogre::LateBinding::Colour(g_debuggerManualObject, color.r, color.g, color.b, 1.0f);
				Ogre::LateBinding::Position(g_debuggerManualObject, corners[j].x, corners[j].y, corners[j].z);
				Ogre::LateBinding::Colour(g_debuggerManualObject, color.r, color.g, color.b, 1.0f);
			};

			AddLine(0, 1); AddLine(1, 2); AddLine(2, 3); AddLine(3, 0); // Front
			AddLine(4, 5); AddLine(5, 6); AddLine(6, 7); AddLine(7, 4); // Back
			AddLine(0, 4); AddLine(1, 5); AddLine(2, 6); AddLine(3, 7); // Sides

			Ogre::LateBinding::End(g_debuggerManualObject);

			DestroyOgreString(matNameBuf);
		}

		return 0;
	}
}
