/* Copyright (C) 2023-2026 VTrider
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

#include <string>

namespace ExtraUtilities::Lua::GameObject
{
	int SetAsUser(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::GameObject* obj = BZR::GameObject::GetObj(h);
		BZR::GameObject::SetAsUser(obj);
		return 0;
	}

	int IsCommTowerPowered(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);

		lua_getglobal(L, "GetClassLabel");
		lua_pushlightuserdata(L, reinterpret_cast<void*>(h));
		lua_call(L, 1, 1);

		std::string classLabel = luaL_checkstring(L, -1);

		if (classLabel != "commtower")
		{
			luaL_error(L, "Extra Utilities: object is not a comm tower");
			return 0;
		}

		BZR::GameObject* obj = BZR::GameObject::GetObj(h);

		BZR::handle powerHandle;

		__asm
		{
			mov eax, [obj]
			mov eax, [eax+0x238]
			mov [powerHandle], eax;
		}
		
		lua_pushboolean(L, powerHandle == 0 ? 0 : 1);

		return 1;
	}

	int GetHandle(lua_State* L)
	{
		auto gameObject = reinterpret_cast<BZR::GameObject*>(CheckHandle(L, 1));
		BZR::handle h = BZR::GameObject::GetHandle(gameObject);
		lua_pushlightuserdata(L, reinterpret_cast<void*>(h));
		return 1;
	}

	int SetHeadlightDiffuse(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float r = static_cast<float>(luaL_checknumber(L, 2));
		float g = static_cast<float>(luaL_checknumber(L, 3));
		float b = static_cast<float>(luaL_checknumber(L, 4));

		auto obj = BZR::GameObject::GetObj(h);

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
		BZR::handle h = CheckHandle(L, 1);
		float r = static_cast<float>(luaL_checknumber(L, 2));
		float g = static_cast<float>(luaL_checknumber(L, 3));
		float b = static_cast<float>(luaL_checknumber(L, 4));

		auto obj = BZR::GameObject::GetObj(h);

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
		BZR::handle h = CheckHandle(L, 1);
		float innerAngle = static_cast<float>(luaL_checknumber(L, 2));
		float outerAngle = static_cast<float>(luaL_checknumber(L, 3));
		float falloff = static_cast<float>(luaL_checknumber(L, 4));

		auto obj = BZR::GameObject::GetObj(h);

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
		BZR::handle h = CheckHandle(L, 1);
		bool visible = CheckBool(L, 2);

		auto obj = BZR::GameObject::GetObj(h);

		void* light = obj->GetLight();

		if (light == nullptr)
		{
			return 0;
		}

		Ogre::SetVisible(light, visible);

		return 0;
	}

	int GetMass(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float mass = BZR::GameObject::GetObj(h)->euler.mass;
		lua_pushnumber(L, mass);
		return 1;
	}

	int SetMass(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float mass = static_cast<float>(luaL_checknumber(L, 2));
		auto obj = BZR::GameObject::GetObj(h);
		obj->euler.mass = mass;
		obj->euler.mass_inv = 1 / mass;
		return 0;
	}

	int GetObj(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::GameObject* obj = BZR::GameObject::GetObj(h);
		lua_pushlightuserdata(L, obj);
		return 1;
	}


	int GetRadarPeriod(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::Scanner* scanner = BZR::GameObject::GetObj(h)->GetScanner();
		lua_pushnumber(L, scanner->period);
		return 1;
	}

	int SetRadarPeriod(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float period = static_cast<float>(luaL_checknumber(L, 2));
		BZR::Scanner* scanner = BZR::GameObject::GetObj(h)->GetScanner();
		scanner->period = period;
		scanner->sweep = 0.01f; // this causes it to immediately update and skip the current sweep

		return 0;
	}

	int GetRadarRange(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::Scanner* scanner = BZR::GameObject::GetObj(h)->GetScanner();
		lua_pushnumber(L, scanner->range);
		return 1;
	}

	int SetRadarRange(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float range = static_cast<float>(luaL_checknumber(L, 2));
		BZR::Scanner* scanner = BZR::GameObject::GetObj(h)->GetScanner();
		scanner->range = range;
		return 0;
	}

	int GetVelocJam(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::Jammer* jammer = BZR::GameObject::GetObj(h)->GetJammer();
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
		BZR::handle h = CheckHandle(L, 1);
		float maxSpeed = static_cast<float>(luaL_checknumber(L, 2));
		BZR::Jammer* jammer = BZR::GameObject::GetObj(h)->GetJammer();
		if (jammer != nullptr)
		{
			jammer->maxSpeed = maxSpeed;
		}
		return 0;
	}

	int GetLabel(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		lua_pushstring(L, BZR::GameObject::GetObj(h)->label);
		return 1;
	}

	int GetPosition(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		PushVector(L, BZR::GameObject::GetObj(h)->pos);
		return 1;
	}

	int SetPosition(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::VECTOR_3D pos = CheckVectorOrSingles(L, 2);
		BZR::GameObject::GetObj(h)->pos = pos;
		return 0;
	}

	int GetVelocity(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		PushVector(L, BZR::GameObject::GetObj(h)->euler.v);
		return 1;
	}

	int SetVelocity(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		BZR::VECTOR_3D v = CheckVectorOrSingles(L, 2);
		BZR::GameObject::GetObj(h)->euler.v = v;
		return 0;
	}
}