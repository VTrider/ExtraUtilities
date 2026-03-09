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

#include <Windows.h>

#include <cstdarg>
#include <cstdio>
#include <lua.hpp>

#include <string>

namespace ExtraUtilities::Lua::GameObject
{
	namespace
	{
		void LogMaterialDebug(const char* fmt, ...)
		{
			char message[1024];
			va_list args;
			va_start(args, fmt);
			vsnprintf_s(message, sizeof(message), _TRUNCATE, fmt, args);
			va_end(args);

			OutputDebugStringA(message);
			OutputDebugStringA("\n");

			if (FILE* file = fopen("exu_material_debug.log", "a"))
			{
				fprintf(file, "%s\n", message);
				fclose(file);
			}
		}

		bool TryGetNumSubEntities(void* entity, uint32_t& outCount)
		{
			__try
			{
				outCount = Ogre::GetNumSubEntities(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] GetNumSubEntities crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outCount = 0;
				return false;
			}
		}

		bool TryGetSubEntityByIndex(void* entity, uint32_t index, void*& outSubEntity)
		{
			__try
			{
				outSubEntity = Ogre::GetSubEntityByIndex(entity, index);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] GetSubEntity crashed entity=%p index=%u code=0x%08X", entity, index, GetExceptionCode());
				outSubEntity = nullptr;
				return false;
			}
		}

		bool TryGetMaterialName(void* subEntity, std::string& outName)
		{
			__try
			{
				outName = Ogre::GetMaterialNameSubEntity(subEntity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] GetMaterialName crashed subEntity=%p code=0x%08X", subEntity, GetExceptionCode());
				outName.clear();
				return false;
			}
		}

		bool TrySetMaterialNameEntity(void* entity, const std::string& materialName, const std::string& resourceGroup)
		{
			__try
			{
				Ogre::SetMaterialNameEntity(entity, materialName, resourceGroup);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug(
					"[EXU::Material] SetMaterialNameEntity crashed entity=%p material=%s group=%s code=0x%08X",
					entity,
					materialName.c_str(),
					resourceGroup.c_str(),
					GetExceptionCode());
				return false;
			}
		}

		bool TrySetMaterialNameSubEntity(void* subEntity, const std::string& materialName, const std::string& resourceGroup)
		{
			__try
			{
				Ogre::SetMaterialNameSubEntity(subEntity, materialName, resourceGroup);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug(
					"[EXU::Material] SetMaterialNameSubEntity crashed subEntity=%p material=%s group=%s code=0x%08X",
					subEntity,
					materialName.c_str(),
					resourceGroup.c_str(),
					GetExceptionCode());
				return false;
			}
		}

		bool TryGetVisible(void* entity, bool& outVisible)
		{
			__try
			{
				outVisible = Ogre::GetVisible(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] GetVisible crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outVisible = false;
				return false;
			}
		}

		bool TrySetVisible(void* entity, bool visible)
		{
			__try
			{
				Ogre::SetVisible(entity, visible);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] SetVisible crashed entity=%p visible=%d code=0x%08X", entity, visible ? 1 : 0, GetExceptionCode());
				return false;
			}
		}

		bool TryGetCastShadows(void* entity, bool& outCastShadows)
		{
			__try
			{
				outCastShadows = Ogre::GetCastShadows(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] GetCastShadows crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outCastShadows = false;
				return false;
			}
		}

		bool TrySetCastShadows(void* entity, bool castShadows)
		{
			__try
			{
				Ogre::SetCastShadows(entity, castShadows);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] SetCastShadows crashed entity=%p castShadows=%d code=0x%08X", entity, castShadows ? 1 : 0, GetExceptionCode());
				return false;
			}
		}

		bool IsRenderableEntityCandidate(void* candidate)
		{
			if (candidate == nullptr)
			{
				return false;
			}

			uint32_t count = 0;
			if (!TryGetNumSubEntities(candidate, count) || count == 0 || count > 64)
			{
				return false;
			}

			void* subEntity = nullptr;
			if (!TryGetSubEntityByIndex(candidate, 0, subEntity) || subEntity == nullptr)
			{
				return false;
			}

			std::string materialName;
			if (!TryGetMaterialName(subEntity, materialName))
			{
				return false;
			}

			return true;
		}

		void* GetRenderableEntity(BZR::GameObject* obj)
		{
			if (obj == nullptr)
			{
				return nullptr;
			}

			void* entity = nullptr;
			__try
			{
				entity = obj->GetOgreEntity();
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Material] Ogre pointer lookup crashed obj=%p code=0x%08X", obj, GetExceptionCode());
				return nullptr;
			}

			if (IsRenderableEntityCandidate(entity))
			{
				return entity;
			}

			LogMaterialDebug("[EXU::Material] Ogre entity validation failed obj=%p entity=%p", obj, entity);
			return nullptr;
		}

		void* GetRenderableEntity(BZR::handle h)
		{
			return GetRenderableEntity(BZR::GameObject::GetObj(h));
		}

		uint32_t CheckSubEntityIndex(lua_State* L, void* entity, int idx)
		{
			int requestedIndex = luaL_checkinteger(L, idx);
			if (requestedIndex < 0)
			{
				luaL_argerror(L, idx, "sub-entity index must be >= 0");
			}

			const auto subEntityIndex = static_cast<uint32_t>(requestedIndex);
			uint32_t numSubEntities = 0;
			if (!TryGetNumSubEntities(entity, numSubEntities))
			{
				luaL_error(L, "Extra Utilities: render entity probe crashed while reading sub-entity count");
			}
			if (subEntityIndex >= numSubEntities)
			{
				luaL_argerror(L, idx, "sub-entity index is out of range");
			}

			return subEntityIndex;
		}

		void* GetFirstSubEntity(void* entity)
		{
			uint32_t count = 0;
			if (!TryGetNumSubEntities(entity, count) || count == 0)
			{
				return nullptr;
			}

			void* subEntity = nullptr;
			if (!TryGetSubEntityByIndex(entity, 0, subEntity))
			{
				return nullptr;
			}

			return subEntity;
		}

		std::string CheckOptionalResourceGroup(lua_State* L, int idx)
		{
			if (lua_isnoneornil(L, idx))
			{
				return "General";
			}

			return luaL_checkstring(L, idx);
		}

		void* GetSubEntity(lua_State* L, void* entity, int idx)
		{
			void* subEntity = nullptr;
			if (!TryGetSubEntityByIndex(entity, CheckSubEntityIndex(L, entity, idx), subEntity))
			{
				luaL_error(L, "Extra Utilities: render entity probe crashed while getting sub-entity");
			}
			return subEntity;
		}
	}

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

	int GetNumSubEntities(lua_State* L)
	{
		return GetSubEntityCount(L);
	}

	int GetSubEntityCount(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		uint32_t count = 0;
		if (!TryGetNumSubEntities(entity, count))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushinteger(L, count);
		return 1;
	}

	int GetMaterialName(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		void* subEntity = nullptr;
		if (!lua_isnoneornil(L, 2))
		{
			subEntity = GetSubEntity(L, entity, 2);
		}
		else
		{
			subEntity = GetFirstSubEntity(entity);
			if (subEntity == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}
		}

		std::string materialName;
		if (!TryGetMaterialName(subEntity, materialName))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushstring(L, materialName.c_str());
		return 1;
	}

	int GetSubEntityMaterial(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		void* subEntity = GetSubEntity(L, entity, 2);

		std::string materialName;
		if (!TryGetMaterialName(subEntity, materialName))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushstring(L, materialName.c_str());
		return 1;
	}

	int SetMaterialName(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string materialName = luaL_checkstring(L, 2);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		if (lua_type(L, 3) == LUA_TNUMBER)
		{
			void* subEntity = GetSubEntity(L, entity, 3);
			std::string resourceGroup = CheckOptionalResourceGroup(L, 4);
			TrySetMaterialNameSubEntity(subEntity, materialName, resourceGroup);
			return 0;
		}

		std::string resourceGroup = CheckOptionalResourceGroup(L, 3);
		TrySetMaterialNameEntity(entity, materialName, resourceGroup);
		return 0;
	}

	int SetEntityMaterial(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string materialName = luaL_checkstring(L, 2);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		std::string resourceGroup = CheckOptionalResourceGroup(L, 3);
		TrySetMaterialNameEntity(entity, materialName, resourceGroup);
		return 0;
	}

	int SetSubEntityMaterial(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		luaL_checkinteger(L, 2);
		std::string materialName = luaL_checkstring(L, 3);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		void* subEntity = GetSubEntity(L, entity, 2);
		std::string resourceGroup = CheckOptionalResourceGroup(L, 4);
		TrySetMaterialNameSubEntity(subEntity, materialName, resourceGroup);
		return 0;
	}

	int GetEntityVisible(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		bool visible = false;
		if (!TryGetVisible(entity, visible))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushboolean(L, visible ? 1 : 0);
		return 1;
	}

	int GetEntityCastShadows(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		bool castShadows = false;
		if (!TryGetCastShadows(entity, castShadows))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushboolean(L, castShadows ? 1 : 0);
		return 1;
	}

	int SetEntityVisible(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		bool visible = CheckBool(L, 2);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		TrySetVisible(entity, visible);
		return 0;
	}

	int SetEntityCastShadows(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		bool castShadows = CheckBool(L, 2);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		TrySetCastShadows(entity, castShadows);
		return 0;
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
}
