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

#include <cmath>
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

		bool TryGetRenderingDistance(void* entity, float& outDistance)
		{
			__try
			{
				outDistance = Ogre::GetRenderingDistance(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] GetRenderingDistance crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outDistance = 0.0f;
				return false;
			}
		}

		bool TrySetRenderingDistance(void* entity, float distance)
		{
			__try
			{
				Ogre::SetRenderingDistance(entity, distance);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] SetRenderingDistance crashed entity=%p distance=%g code=0x%08X", entity, distance, GetExceptionCode());
				return false;
			}
		}

		bool TryGetVisibilityFlags(void* entity, uint32_t& outFlags)
		{
			__try
			{
				outFlags = Ogre::GetVisibilityFlags(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] GetVisibilityFlags crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outFlags = 0;
				return false;
			}
		}

		bool TrySetVisibilityFlags(void* entity, uint32_t flags)
		{
			__try
			{
				Ogre::SetVisibilityFlags(entity, flags);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] SetVisibilityFlags crashed entity=%p flags=0x%08X code=0x%08X", entity, flags, GetExceptionCode());
				return false;
			}
		}

		bool TryGetQueryFlags(void* entity, uint32_t& outFlags)
		{
			__try
			{
				outFlags = Ogre::GetQueryFlags(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] GetQueryFlags crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outFlags = 0;
				return false;
			}
		}

		bool TrySetQueryFlags(void* entity, uint32_t flags)
		{
			__try
			{
				Ogre::SetQueryFlags(entity, flags);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] SetQueryFlags crashed entity=%p flags=0x%08X code=0x%08X", entity, flags, GetExceptionCode());
				return false;
			}
		}

		bool TryGetRenderQueueGroup(void* renderable, uint8_t& outGroup)
		{
			__try
			{
				outGroup = Ogre::GetRenderQueueGroup(renderable);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] GetRenderQueueGroup crashed renderable=%p code=0x%08X", renderable, GetExceptionCode());
				outGroup = 0;
				return false;
			}
		}

		bool TrySetRenderQueueGroup(void* entity, uint8_t group)
		{
			__try
			{
				Ogre::SetRenderQueueGroupMovable(entity, group);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Render] SetRenderQueueGroup crashed entity=%p group=%u code=0x%08X", entity, group, GetExceptionCode());
				return false;
			}
		}

		bool TryHasSkeleton(void* entity, bool& outHasSkeleton)
		{
			__try
			{
				outHasSkeleton = Ogre::HasSkeleton(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] HasSkeleton crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outHasSkeleton = false;
				return false;
			}
		}

		bool TryGetAllAnimationStates(void* entity, void*& outStates)
		{
			__try
			{
				outStates = Ogre::GetAllAnimationStates(entity);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] GetAllAnimationStates crashed entity=%p code=0x%08X", entity, GetExceptionCode());
				outStates = nullptr;
				return false;
			}
		}

		bool TryHasAnimationState(void* animationStates, const std::string& name, bool& outHasAnimation)
		{
			__try
			{
				outHasAnimation = Ogre::HasAnimationState(animationStates, name);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] HasAnimationState crashed states=%p name=%s code=0x%08X", animationStates, name.c_str(), GetExceptionCode());
				outHasAnimation = false;
				return false;
			}
		}

		bool TryGetAnimationState(void* animationStates, const std::string& name, void*& outAnimationState)
		{
			__try
			{
				outAnimationState = Ogre::GetAnimationState(animationStates, name);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] GetAnimationState crashed states=%p name=%s code=0x%08X", animationStates, name.c_str(), GetExceptionCode());
				outAnimationState = nullptr;
				return false;
			}
		}

		void* GetNamedAnimationState(void* entity, const std::string& name)
		{
			bool hasSkeleton = false;
			if (!TryHasSkeleton(entity, hasSkeleton) || !hasSkeleton)
			{
				return nullptr;
			}

			void* animationStates = nullptr;
			if (!TryGetAllAnimationStates(entity, animationStates) || animationStates == nullptr)
			{
				return nullptr;
			}

			bool hasAnimation = false;
			if (!TryHasAnimationState(animationStates, name, hasAnimation) || !hasAnimation)
			{
				return nullptr;
			}

			void* animationState = nullptr;
			if (!TryGetAnimationState(animationStates, name, animationState))
			{
				return nullptr;
			}

			return animationState;
		}

		bool TryGetAnimationLength(void* animationState, float& outValue)
		{
			__try
			{
				outValue = Ogre::GetAnimationLength(animationState);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] GetLength crashed animationState=%p code=0x%08X", animationState, GetExceptionCode());
				outValue = 0.0f;
				return false;
			}
		}

		bool TryGetAnimationTimePosition(void* animationState, float& outValue)
		{
			__try
			{
				outValue = Ogre::GetAnimationTimePosition(animationState);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] GetTimePosition crashed animationState=%p code=0x%08X", animationState, GetExceptionCode());
				outValue = 0.0f;
				return false;
			}
		}

		bool TryGetAnimationWeight(void* animationState, float& outValue)
		{
			__try
			{
				outValue = Ogre::GetAnimationWeight(animationState);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] GetWeight crashed animationState=%p code=0x%08X", animationState, GetExceptionCode());
				outValue = 0.0f;
				return false;
			}
		}

		bool TryGetAnimationLoop(void* animationState, bool& outValue)
		{
			__try
			{
				outValue = Ogre::GetAnimationLoop(animationState);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] GetLoop crashed animationState=%p code=0x%08X", animationState, GetExceptionCode());
				outValue = false;
				return false;
			}
		}

		bool TryGetAnimationEnabled(void* animationState, bool& outValue)
		{
			__try
			{
				outValue = Ogre::GetAnimationEnabled(animationState);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] GetEnabled crashed animationState=%p code=0x%08X", animationState, GetExceptionCode());
				outValue = false;
				return false;
			}
		}

		bool TrySetAnimationEnabled(void* animationState, bool enabled)
		{
			__try
			{
				Ogre::SetAnimationEnabled(animationState, enabled);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] SetEnabled crashed animationState=%p enabled=%d code=0x%08X", animationState, enabled ? 1 : 0, GetExceptionCode());
				return false;
			}
		}

		bool TrySetAnimationLoop(void* animationState, bool loop)
		{
			__try
			{
				Ogre::SetAnimationLoop(animationState, loop);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] SetLoop crashed animationState=%p loop=%d code=0x%08X", animationState, loop ? 1 : 0, GetExceptionCode());
				return false;
			}
		}

		bool TrySetAnimationWeight(void* animationState, float weight)
		{
			__try
			{
				Ogre::SetAnimationWeight(animationState, weight);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] SetWeight crashed animationState=%p weight=%g code=0x%08X", animationState, weight, GetExceptionCode());
				return false;
			}
		}

		bool TrySetAnimationTimePosition(void* animationState, float timePosition)
		{
			__try
			{
				Ogre::SetAnimationTimePosition(animationState, timePosition);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Animation] SetTimePosition crashed animationState=%p time=%g code=0x%08X", animationState, timePosition, GetExceptionCode());
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

		void* GetLightObject(BZR::GameObject* obj)
		{
			if (obj == nullptr)
			{
				return nullptr;
			}

			void* light = nullptr;
			__try
			{
				light = obj->GetLight();
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] Ogre light lookup crashed obj=%p code=0x%08X", obj, GetExceptionCode());
				return nullptr;
			}

			return light;
		}

		void* GetLightObject(BZR::handle h)
		{
			return GetLightObject(BZR::GameObject::GetObj(h));
		}

		bool TryGetLightPowerScale(void* light, float& outValue)
		{
			__try
			{
				outValue = Ogre::GetPowerScale(light);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] GetPowerScale crashed light=%p code=0x%08X", light, GetExceptionCode());
				outValue = 0.0f;
				return false;
			}
		}

		bool TrySetLightPowerScale(void* light, float value)
		{
			__try
			{
				Ogre::SetPowerScale(light, value);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] SetPowerScale crashed light=%p value=%g code=0x%08X", light, value, GetExceptionCode());
				return false;
			}
		}

		bool TryGetLightPosition(void* light, BZR::VECTOR_3D& outPosition)
		{
			__try
			{
				if (auto* position = Ogre::GetLightPosition(light))
				{
					outPosition = *position;
					return true;
				}
				return false;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] GetPosition crashed light=%p code=0x%08X", light, GetExceptionCode());
				outPosition = {};
				return false;
			}
		}

		bool TrySetLightPosition(void* light, const BZR::VECTOR_3D& position)
		{
			__try
			{
				Ogre::SetLightPosition(light, position.x, position.y, position.z);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] SetPosition crashed light=%p code=0x%08X", light, GetExceptionCode());
				return false;
			}
		}

		bool TryGetLightDirection(void* light, BZR::VECTOR_3D& outDirection)
		{
			__try
			{
				if (auto* direction = Ogre::GetDirection(light))
				{
					outDirection = *direction;
					return true;
				}
				return false;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] GetDirection crashed light=%p code=0x%08X", light, GetExceptionCode());
				outDirection = {};
				return false;
			}
		}

		bool TrySetLightDirection(void* light, const BZR::VECTOR_3D& direction)
		{
			__try
			{
				Ogre::SetDirection(light, direction.x, direction.y, direction.z);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] SetDirection crashed light=%p code=0x%08X", light, GetExceptionCode());
				return false;
			}
		}

		bool TrySetLightAttenuation(void* light, float range, float constant, float linear, float quadratic)
		{
			__try
			{
				Ogre::SetAttenuation(light, range, constant, linear, quadratic);
				return true;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				LogMaterialDebug("[EXU::Light] SetAttenuation crashed light=%p code=0x%08X", light, GetExceptionCode());
				return false;
			}
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

	int GetEntityRenderingDistance(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		float distance = 0.0f;
		if (!TryGetRenderingDistance(entity, distance))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushnumber(L, distance);
		return 1;
	}

	int GetEntityVisibilityFlags(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		uint32_t flags = 0;
		if (!TryGetVisibilityFlags(entity, flags))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushinteger(L, flags);
		return 1;
	}

	int GetEntityQueryFlags(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		uint32_t flags = 0;
		if (!TryGetQueryFlags(entity, flags))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushinteger(L, flags);
		return 1;
	}

	int GetEntityRenderQueueGroup(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		uint8_t group = 0;
		if (!TryGetRenderQueueGroup(entity, group))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushinteger(L, group);
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

	int SetEntityRenderingDistance(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float distance = static_cast<float>(luaL_checknumber(L, 2));
		if (!std::isfinite(distance) || distance < 0.0f)
		{
			return luaL_argerror(L, 2, "rendering distance must be a finite non-negative number");
		}

		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		TrySetRenderingDistance(entity, distance);
		return 0;
	}

	int SetEntityVisibilityFlags(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		uint32_t flags = static_cast<uint32_t>(luaL_checkinteger(L, 2));
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		TrySetVisibilityFlags(entity, flags);
		return 0;
	}

	int SetEntityQueryFlags(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		uint32_t flags = static_cast<uint32_t>(luaL_checkinteger(L, 2));
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		TrySetQueryFlags(entity, flags);
		return 0;
	}

	int SetEntityRenderQueueGroup(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		int groupValue = luaL_checkinteger(L, 2);
		if (groupValue < 0 || groupValue > 255)
		{
			return luaL_argerror(L, 2, "render queue group must be in range 0..255");
		}

		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		TrySetRenderQueueGroup(entity, static_cast<uint8_t>(groupValue));
		return 0;
	}

	int SetSubEntityVisible(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		void* subEntity = GetSubEntity(L, entity, 2);
		bool visible = CheckBool(L, 3);
		TrySetVisible(subEntity, visible);
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

	int GetLightPowerScale(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* light = GetLightObject(h);
		if (light == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		float powerScale = 0.0f;
		if (!TryGetLightPowerScale(light, powerScale))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushnumber(L, powerScale);
		return 1;
	}

	int SetLightPowerScale(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float powerScale = static_cast<float>(luaL_checknumber(L, 2));
		if (!std::isfinite(powerScale))
		{
			return luaL_argerror(L, 2, "power scale must be a finite number");
		}

		void* light = GetLightObject(h);
		if (light == nullptr)
		{
			return 0;
		}

		TrySetLightPowerScale(light, powerScale);
		return 0;
	}

	int GetLightPosition(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* light = GetLightObject(h);
		if (light == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		BZR::VECTOR_3D position{};
		if (!TryGetLightPosition(light, position))
		{
			lua_pushnil(L);
			return 1;
		}

		PushVector(L, position);
		return 1;
	}

	int SetLightPosition(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		auto position = CheckVectorOrSingles(L, 2);
		if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z))
		{
			return luaL_argerror(L, 2, "light position must use finite numeric values");
		}

		void* light = GetLightObject(h);
		if (light == nullptr)
		{
			return 0;
		}

		TrySetLightPosition(light, position);
		return 0;
	}

	int GetLightDirection(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		void* light = GetLightObject(h);
		if (light == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		BZR::VECTOR_3D direction{};
		if (!TryGetLightDirection(light, direction))
		{
			lua_pushnil(L);
			return 1;
		}

		PushVector(L, direction);
		return 1;
	}

	int SetLightDirection(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		auto direction = CheckVectorOrSingles(L, 2);
		if (!std::isfinite(direction.x) || !std::isfinite(direction.y) || !std::isfinite(direction.z))
		{
			return luaL_argerror(L, 2, "light direction must use finite numeric values");
		}

		void* light = GetLightObject(h);
		if (light == nullptr)
		{
			return 0;
		}

		TrySetLightDirection(light, direction);
		return 0;
	}

	int SetLightAttenuation(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		float range = static_cast<float>(luaL_checknumber(L, 2));
		float constant = static_cast<float>(luaL_checknumber(L, 3));
		float linear = static_cast<float>(luaL_checknumber(L, 4));
		float quadratic = static_cast<float>(luaL_checknumber(L, 5));
		if (!std::isfinite(range) || !std::isfinite(constant) || !std::isfinite(linear) || !std::isfinite(quadratic))
		{
			return luaL_argerror(L, 2, "light attenuation values must be finite numbers");
		}

		void* light = GetLightObject(h);
		if (light == nullptr)
		{
			return 0;
		}

		TrySetLightAttenuation(light, range, constant, linear, quadratic);
		return 0;
	}

	int HasEntityAnimation(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string animationName = luaL_checkstring(L, 2);
		void* entity = GetRenderableEntity(h);
		lua_pushboolean(L, entity != nullptr && GetNamedAnimationState(entity, animationName) != nullptr);
		return 1;
	}

	int GetEntityAnimationInfo(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string animationName = luaL_checkstring(L, 2);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		void* animationState = GetNamedAnimationState(entity, animationName);
		if (animationState == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		bool enabled = false;
		bool loop = false;
		float weight = 0.0f;
		float timePosition = 0.0f;
		float length = 0.0f;
		if (!TryGetAnimationEnabled(animationState, enabled) ||
			!TryGetAnimationLoop(animationState, loop) ||
			!TryGetAnimationWeight(animationState, weight) ||
			!TryGetAnimationTimePosition(animationState, timePosition) ||
			!TryGetAnimationLength(animationState, length))
		{
			lua_pushnil(L);
			return 1;
		}

		lua_createtable(L, 0, 5);
		lua_pushboolean(L, enabled ? 1 : 0);
		lua_setfield(L, -2, "enabled");
		lua_pushboolean(L, loop ? 1 : 0);
		lua_setfield(L, -2, "loop");
		lua_pushnumber(L, weight);
		lua_setfield(L, -2, "weight");
		lua_pushnumber(L, timePosition);
		lua_setfield(L, -2, "timePosition");
		lua_pushnumber(L, length);
		lua_setfield(L, -2, "length");
		return 1;
	}

	int SetEntityAnimationEnabled(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string animationName = luaL_checkstring(L, 2);
		bool enabled = CheckBool(L, 3);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		if (void* animationState = GetNamedAnimationState(entity, animationName))
		{
			TrySetAnimationEnabled(animationState, enabled);
		}
		return 0;
	}

	int SetEntityAnimationLoop(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string animationName = luaL_checkstring(L, 2);
		bool loop = CheckBool(L, 3);
		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		if (void* animationState = GetNamedAnimationState(entity, animationName))
		{
			TrySetAnimationLoop(animationState, loop);
		}
		return 0;
	}

	int SetEntityAnimationWeight(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string animationName = luaL_checkstring(L, 2);
		float weight = static_cast<float>(luaL_checknumber(L, 3));
		if (!std::isfinite(weight))
		{
			return luaL_argerror(L, 3, "animation weight must be a finite number");
		}

		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		if (void* animationState = GetNamedAnimationState(entity, animationName))
		{
			TrySetAnimationWeight(animationState, weight);
		}
		return 0;
	}

	int SetEntityAnimationTime(lua_State* L)
	{
		BZR::handle h = CheckHandle(L, 1);
		std::string animationName = luaL_checkstring(L, 2);
		float timePosition = static_cast<float>(luaL_checknumber(L, 3));
		if (!std::isfinite(timePosition))
		{
			return luaL_argerror(L, 3, "animation time must be a finite number");
		}

		void* entity = GetRenderableEntity(h);
		if (entity == nullptr)
		{
			return 0;
		}

		if (void* animationState = GetNamedAnimationState(entity, animationName))
		{
			TrySetAnimationTimePosition(animationState, timePosition);
		}
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
