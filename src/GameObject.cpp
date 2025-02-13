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

#include "GameObject.h"

#include "LuaHelpers.h"
#include "Ogre.h"

#include <lua.hpp>

namespace ExtraUtilities::Lua::GameObject
{
	int GetHandle(lua_State* L)
	{
		int gameObject = CheckHandle(L, 1);
		unsigned int handle = BZR::GameObject::GetHandle(gameObject);
		lua_pushlightuserdata(L, reinterpret_cast<void*>(handle));
		return 1;
	}

	int GetObj(lua_State* L)
	{
		unsigned int handle = CheckHandle(L, 1);
		BZR::GameObject* obj = BZR::GameObject::GetObj(handle);
		lua_pushlightuserdata(L, obj);
		return 1;
	}

	int SetAsUser(lua_State* L)
	{
		unsigned int handle = CheckHandle(L, 1);
		BZR::GameObject* obj = BZR::GameObject::GetObj(handle);
		BZR::GameObject::SetAsUser(obj);
		return 0;
	}

	int GetRadarRange(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		BZR::Scanner* scanner = BZR::GameObject::GetObj(handle)->GetScanner();
		lua_pushnumber(L, scanner->range);
		return 1;
	}

	int SetRadarRange(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		float range = static_cast<float>(luaL_checknumber(L, 2));
		BZR::Scanner* scanner = BZR::GameObject::GetObj(handle)->GetScanner();
		scanner->range = range;
		return 0;
	}

	int GetRadarPeriod(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		BZR::Scanner* scanner = BZR::GameObject::GetObj(handle)->GetScanner();
		lua_pushnumber(L, scanner->period);
		return 1;
	}

	int SetRadarPeriod(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		float period = static_cast<float>(luaL_checknumber(L, 2));
		BZR::Scanner* scanner = BZR::GameObject::GetObj(handle)->GetScanner();
		scanner->period = period;
		scanner->sweep = 0.01f; // this causes it to immediately update and skip the current sweep

		return 0;
	}

	int GetVelocJam(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		BZR::Jammer* jammer = BZR::GameObject::GetObj(handle)->GetJammer();
		if (jammer != nullptr) // sometimes your gameobject may not have a jammer, like if you're a pilot for example
		{
			lua_pushnumber(L, jammer->maxSpeed);
		}
		else
		{
			lua_pushnil(L);
		}
		return 1;
	}

	int SetVelocJam(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		float maxSpeed = static_cast<float>(luaL_checknumber(L, 2));
		BZR::Jammer* jammer = BZR::GameObject::GetObj(handle)->GetJammer();
		if (jammer != nullptr)
		{
			jammer->maxSpeed = maxSpeed;
		}
		return 0;
	}

	int SetHeadlightDiffuse(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		float r = static_cast<float>(luaL_checknumber(L, 2));
		float g = static_cast<float>(luaL_checknumber(L, 3));
		float b = static_cast<float>(luaL_checknumber(L, 4));

		auto obj = BZR::GameObject::GetObj(handle);

		void* light = obj->GetLight();

		if (light == nullptr)
		{
			return 0;
		}

		Ogre::SetDiffuseColor(light, r, g, b);

		return 0;
	}

	int SetHeadlightSpecular(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		float r = static_cast<float>(luaL_checknumber(L, 2));
		float g = static_cast<float>(luaL_checknumber(L, 3));
		float b = static_cast<float>(luaL_checknumber(L, 4));

		auto obj = BZR::GameObject::GetObj(handle);

		void* light = obj->GetLight();

		if (light == nullptr)
		{
			return 0;
		}

		Ogre::SetSpecularColor(light, r, g, b);

		return 0;
	}

	int SetHeadlightRange(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		float innerAngle = static_cast<float>(luaL_checknumber(L, 2));
		float outerAngle = static_cast<float>(luaL_checknumber(L, 3));
		float falloff = static_cast<float>(luaL_checknumber(L, 4));

		auto obj = BZR::GameObject::GetObj(handle);

		void* light = obj->GetLight();

		if (light == nullptr)
		{
			return 0;
		}

		Ogre::SetSpotlightRange(light, &innerAngle, &outerAngle, falloff);

		return 0;
	}

	int SetHeadlightVisible(lua_State* L)
	{
		int handle = CheckHandle(L, 1);
		bool visible = CheckBool(L, 2);

		auto obj = BZR::GameObject::GetObj(handle);

		void* light = obj->GetLight();

		if (light == nullptr)
		{
			return 0;
		}

		Ogre::SetVisible(light, visible);

		return 0;
	}
}