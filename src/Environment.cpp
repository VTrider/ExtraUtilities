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

#include "Environment.h"

#include "InlinePatch.h"
#include "LuaHelpers.h"

#include <Windows.h>

#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <string>

namespace ExtraUtilities::Lua::Environment
{
	void LogEnvironmentDebug(const char* fmt, ...)
	{
		char message[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf_s(message, sizeof(message), _TRUNCATE, fmt, args);
		va_end(args);

		OutputDebugStringA(message);
		OutputDebugStringA("\n");

		if (FILE* file = fopen("exu_environment_debug.log", "a"))
		{
			fprintf(file, "%s\n", message);
			fclose(file);
		}
	}

	std::string DescribeLuaCaller(lua_State* L)
	{
		lua_Debug ar{};
		if (lua_getstack(L, 1, &ar) && lua_getinfo(L, "Sln", &ar))
		{
			char caller[512];
			sprintf_s(
				caller,
				sizeof(caller),
				"%s:%d (%s)",
				ar.short_src[0] ? ar.short_src : "?",
				ar.currentline,
				ar.name ? ar.name : "anonymous");
			return caller;
		}

		return "unknown";
	}

	bool IsFiniteColor(const Ogre::Color& color)
	{
		return std::isfinite(color.r)
			&& std::isfinite(color.g)
			&& std::isfinite(color.b)
			&& std::isfinite(color.a);
	}

	bool IsExpectedColorRange(const Ogre::Color& color)
	{
		return color.r >= 0.0f && color.r <= 1.0f
			&& color.g >= 0.0f && color.g <= 1.0f
			&& color.b >= 0.0f && color.b <= 1.0f;
	}

	bool IsFiniteVector(const BZR::VECTOR_3D& vector)
	{
		return std::isfinite(vector.x)
			&& std::isfinite(vector.y)
			&& std::isfinite(vector.z);
	}

	bool IsFiniteScalar(float value)
	{
		return std::isfinite(value);
	}

	using SkyNodeGetter = void*(*)(void*);

	bool TryHasSkyNode(void* sceneManager, SkyNodeGetter getter, bool& outHasNode, const char* label)
	{
		__try
		{
			outHasNode = getter(sceneManager) != nullptr;
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Sky] %s node probe crashed sceneManager=%p code=0x%08X", label, sceneManager, GetExceptionCode());
			outHasNode = false;
			return false;
		}
	}

	bool TryGetSkyBoxGenParameters(void* sceneManager, Ogre::SkyBoxGenParameters& outParams)
	{
		__try
		{
			return Ogre::GetSkyBoxGenParameters(sceneManager, outParams);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Sky] get skybox params crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
			return false;
		}
	}

	bool TryGetSkyDomeGenParameters(void* sceneManager, Ogre::SkyDomeGenParameters& outParams)
	{
		__try
		{
			return Ogre::GetSkyDomeGenParameters(sceneManager, outParams);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Sky] get skydome params crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
			return false;
		}
	}

	bool TryGetSkyPlaneGenParameters(void* sceneManager, Ogre::SkyPlaneGenParameters& outParams)
	{
		__try
		{
			return Ogre::GetSkyPlaneGenParameters(sceneManager, outParams);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Sky] get skyplane params crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
			return false;
		}
	}

	void* GetSceneManager()
	{
		return Ogre::sceneManager.Read();
	}

	void* GetCurrentViewport()
	{
		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			return nullptr;
		}

		__try
		{
			return Ogre::GetCurrentViewport(sceneManager);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Viewport] get current viewport crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
			return nullptr;
		}
	}

	void* GetTerrainMasterLight()
	{
		return Ogre::terrain_masterlight.Read();
	}

	Ogre::Color DefaultSunColor()
	{
		return {};
	}

	int GetGravity(lua_State* L)
	{
		BZR::VECTOR_3D g = gravity.Read();

		PushVector(L, g);

		return 1;
	}

	int SetGravity(lua_State* L)
	{
		auto newGravity = CheckVectorOrSingles(L, 1);
		gravity.Write(newGravity);

		return 0;
	}

	int GetFog(lua_State* L)
	{
		Patch::TryInitializeOgre();

		if (GetSceneManager() == nullptr)
		{
			PushFog(L, {});
			return 1;
		}

		Ogre::Fog f = fog.Read();

		PushFog(L, f);

		return 1;
	}

	int SetFog(lua_State* L)
	{
		Patch::TryInitializeOgre();

		if (GetSceneManager() == nullptr)
		{
			return 0;
		}

		auto f = CheckFogOrSingles(L, 1);

		fog.Write(f);

		return 0;
	}

	int GetSunAmbient(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			PushColor(L, DefaultSunColor());
			return 1;
		}

		auto* sunAmbient = Ogre::GetAmbientLight(sceneManager);

		PushColor(L, *sunAmbient);

		return 1;
	}

	int SetSunAmbient(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		auto caller = DescribeLuaCaller(L);
		LogEnvironmentDebug(
			"[EXU::SetSunAmbient] enter caller=%s argType=%s sceneManager=%p",
			caller.c_str(),
			luaL_typename(L, 1),
			sceneManager);
		if (sceneManager == nullptr)
		{
			LogEnvironmentDebug("[EXU::SetSunAmbient] skipped: scene manager unavailable");
			return 0;
		}

		auto color = CheckColorOrSingles(L, 1);
		LogEnvironmentDebug(
			"[EXU::SetSunAmbient] parsed r=%.9g g=%.9g b=%.9g finite=%s inRange=%s",
			color.r,
			color.g,
			color.b,
			IsFiniteColor(color) ? "true" : "false",
			IsExpectedColorRange(color) ? "true" : "false");
		if (!IsFiniteColor(color))
		{
			LogEnvironmentDebug("[EXU::SetSunAmbient] rejecting non-finite color");
			return luaL_argerror(L, 1, "SetSunAmbient requires finite numeric color values");
		}

		LogEnvironmentDebug("[EXU::SetSunAmbient] calling Ogre::SetAmbientLight");
		Ogre::SetAmbientLight(sceneManager, &color);
		LogEnvironmentDebug("[EXU::SetSunAmbient] completed");

		return 0;
	}

	int GetAmbientLight(lua_State* L)
	{
		return GetSunAmbient(L);
	}

	int SetAmbientLight(lua_State* L)
	{
		return SetSunAmbient(L);
	}

	int GetSunDiffuse(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		if (terrainMasterLight == nullptr)
		{
			PushColor(L, DefaultSunColor());
			return 1;
		}

		auto* color = Ogre::GetDiffuseColor(terrainMasterLight);

		PushColor(L, *color);

		return 1;
	}

	int SetSunDiffuse(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		auto caller = DescribeLuaCaller(L);
		LogEnvironmentDebug(
			"[EXU::SetSunDiffuse] enter caller=%s argType=%s terrainMasterLight=%p",
			caller.c_str(),
			luaL_typename(L, 1),
			terrainMasterLight);
		if (terrainMasterLight == nullptr)
		{
			LogEnvironmentDebug("[EXU::SetSunDiffuse] skipped: terrain master light unavailable");
			return 0;
		}

		auto color = CheckColorOrSingles(L, 1);
		LogEnvironmentDebug(
			"[EXU::SetSunDiffuse] parsed r=%.9g g=%.9g b=%.9g finite=%s inRange=%s",
			color.r,
			color.g,
			color.b,
			IsFiniteColor(color) ? "true" : "false",
			IsExpectedColorRange(color) ? "true" : "false");
		if (!IsFiniteColor(color))
		{
			LogEnvironmentDebug("[EXU::SetSunDiffuse] rejecting non-finite color");
			return luaL_argerror(L, 1, "SetSunDiffuse requires finite numeric color values");
		}

		LogEnvironmentDebug("[EXU::SetSunDiffuse] calling Ogre::SetDiffuseColor");
		Ogre::SetDiffuseColor(terrainMasterLight, color.r, color.g, color.b);
		LogEnvironmentDebug("[EXU::SetSunDiffuse] completed");

		return 0;
	}

	int GetSunSpecular(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		if (terrainMasterLight == nullptr)
		{
			PushColor(L, DefaultSunColor());
			return 1;
		}

		auto* color = Ogre::GetSpecularColor(terrainMasterLight);

		PushColor(L, *color);

		return 1;
	}

	int SetSunSpecular(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		auto caller = DescribeLuaCaller(L);
		LogEnvironmentDebug(
			"[EXU::SetSunSpecular] enter caller=%s argType=%s terrainMasterLight=%p",
			caller.c_str(),
			luaL_typename(L, 1),
			terrainMasterLight);
		if (terrainMasterLight == nullptr)
		{
			LogEnvironmentDebug("[EXU::SetSunSpecular] skipped: terrain master light unavailable");
			return 0;
		}

		auto color = CheckColorOrSingles(L, 1);
		LogEnvironmentDebug(
			"[EXU::SetSunSpecular] parsed r=%.9g g=%.9g b=%.9g finite=%s inRange=%s",
			color.r,
			color.g,
			color.b,
			IsFiniteColor(color) ? "true" : "false",
			IsExpectedColorRange(color) ? "true" : "false");
		if (!IsFiniteColor(color))
		{
			LogEnvironmentDebug("[EXU::SetSunSpecular] rejecting non-finite color");
			return luaL_argerror(L, 1, "SetSunSpecular requires finite numeric color values");
		}

		LogEnvironmentDebug("[EXU::SetSunSpecular] calling Ogre::SetSpecularColor");
		Ogre::SetSpecularColor(terrainMasterLight, color.r, color.g, color.b);
		LogEnvironmentDebug("[EXU::SetSunSpecular] completed");

		return 0;
	}

	int GetSunDirection(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		if (terrainMasterLight == nullptr)
		{
			PushVector(L, {});
			return 1;
		}

		auto* direction = Ogre::GetDirection(terrainMasterLight);
		PushVector(L, *direction);
		return 1;
	}

	int SetSunDirection(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		auto caller = DescribeLuaCaller(L);
		LogEnvironmentDebug(
			"[EXU::SetSunDirection] enter caller=%s argType=%s terrainMasterLight=%p",
			caller.c_str(),
			luaL_typename(L, 1),
			terrainMasterLight);
		if (terrainMasterLight == nullptr)
		{
			LogEnvironmentDebug("[EXU::SetSunDirection] skipped: terrain master light unavailable");
			return 0;
		}

		auto direction = CheckVectorOrSingles(L, 1);
		LogEnvironmentDebug(
			"[EXU::SetSunDirection] parsed x=%.9g y=%.9g z=%.9g finite=%s",
			direction.x,
			direction.y,
			direction.z,
			IsFiniteVector(direction) ? "true" : "false");
		if (!IsFiniteVector(direction))
		{
			LogEnvironmentDebug("[EXU::SetSunDirection] rejecting non-finite direction");
			return luaL_argerror(L, 1, "SetSunDirection requires finite numeric vector values");
		}

		LogEnvironmentDebug("[EXU::SetSunDirection] calling Ogre::SetDirection");
		Ogre::SetDirection(terrainMasterLight, direction.x, direction.y, direction.z);
		LogEnvironmentDebug("[EXU::SetSunDirection] completed");
		return 0;
	}

	int GetSunPowerScale(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		if (terrainMasterLight == nullptr)
		{
			lua_pushnumber(L, 0.0);
			return 1;
		}

		lua_pushnumber(L, Ogre::GetPowerScale(terrainMasterLight));
		return 1;
	}

	int SetSunPowerScale(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		auto caller = DescribeLuaCaller(L);
		LogEnvironmentDebug(
			"[EXU::SetSunPowerScale] enter caller=%s terrainMasterLight=%p",
			caller.c_str(),
			terrainMasterLight);
		if (terrainMasterLight == nullptr)
		{
			LogEnvironmentDebug("[EXU::SetSunPowerScale] skipped: terrain master light unavailable");
			return 0;
		}

		float powerScale = static_cast<float>(luaL_checknumber(L, 1));
		LogEnvironmentDebug(
			"[EXU::SetSunPowerScale] parsed value=%.9g finite=%s",
			powerScale,
			IsFiniteScalar(powerScale) ? "true" : "false");
		if (!IsFiniteScalar(powerScale))
		{
			LogEnvironmentDebug("[EXU::SetSunPowerScale] rejecting non-finite value");
			return luaL_argerror(L, 1, "SetSunPowerScale requires a finite numeric value");
		}

		LogEnvironmentDebug("[EXU::SetSunPowerScale] calling Ogre::SetPowerScale");
		Ogre::SetPowerScale(terrainMasterLight, powerScale);
		LogEnvironmentDebug("[EXU::SetSunPowerScale] completed");
		return 0;
	}

	int GetSunShadowFarDistance(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		if (terrainMasterLight == nullptr)
		{
			lua_pushnumber(L, 0.0);
			return 1;
		}

		lua_pushnumber(L, Ogre::GetShadowFarDistance(terrainMasterLight));
		return 1;
	}

	int SetSunShadowFarDistance(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* terrainMasterLight = GetTerrainMasterLight();
		auto caller = DescribeLuaCaller(L);
		LogEnvironmentDebug(
			"[EXU::SetSunShadowFarDistance] enter caller=%s terrainMasterLight=%p",
			caller.c_str(),
			terrainMasterLight);
		if (terrainMasterLight == nullptr)
		{
			LogEnvironmentDebug("[EXU::SetSunShadowFarDistance] skipped: terrain master light unavailable");
			return 0;
		}

		float shadowFarDistance = static_cast<float>(luaL_checknumber(L, 1));
		LogEnvironmentDebug(
			"[EXU::SetSunShadowFarDistance] parsed value=%.9g finite=%s",
			shadowFarDistance,
			IsFiniteScalar(shadowFarDistance) ? "true" : "false");
		if (!IsFiniteScalar(shadowFarDistance))
		{
			LogEnvironmentDebug("[EXU::SetSunShadowFarDistance] rejecting non-finite value");
			return luaL_argerror(L, 1, "SetSunShadowFarDistance requires a finite numeric value");
		}

		LogEnvironmentDebug("[EXU::SetSunShadowFarDistance] calling Ogre::SetShadowFarDistance");
		Ogre::SetShadowFarDistance(terrainMasterLight, shadowFarDistance);
		LogEnvironmentDebug("[EXU::SetSunShadowFarDistance] completed");
		return 0;
	}

	int GetSkyBoxParams(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		bool hasNode = false;
		Ogre::SkyBoxGenParameters params{};
		if (!TryHasSkyNode(sceneManager, Ogre::GetSkyBoxNode, hasNode, "skybox") || !hasNode ||
			!TryGetSkyBoxGenParameters(sceneManager, params))
		{
			lua_pushnil(L);
			return 1;
		}

		PushSkyBoxParams(L, params);
		return 1;
	}

	int GetSkyDomeParams(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		bool hasNode = false;
		Ogre::SkyDomeGenParameters params{};
		if (!TryHasSkyNode(sceneManager, Ogre::GetSkyDomeNode, hasNode, "skydome") || !hasNode ||
			!TryGetSkyDomeGenParameters(sceneManager, params))
		{
			lua_pushnil(L);
			return 1;
		}

		PushSkyDomeParams(L, params);
		return 1;
	}

	int GetSkyPlaneParams(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		bool hasNode = false;
		Ogre::SkyPlaneGenParameters params{};
		if (!TryHasSkyNode(sceneManager, Ogre::GetSkyPlaneNode, hasNode, "skyplane") || !hasNode ||
			!TryGetSkyPlaneGenParameters(sceneManager, params))
		{
			lua_pushnil(L);
			return 1;
		}

		PushSkyPlaneParams(L, params);
		return 1;
	}

	int GetShowBoundingBoxes(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		bool enabled = false;
		__try
		{
			enabled = Ogre::GetShowBoundingBoxes(sceneManager);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Scene] getShowBoundingBoxes crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
		}

		lua_pushboolean(L, enabled ? 1 : 0);
		return 1;
	}

	int SetShowBoundingBoxes(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			return 0;
		}

		bool enabled = CheckBool(L, 1);
		__try
		{
			Ogre::ShowBoundingBoxes(sceneManager, enabled);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Scene] showBoundingBoxes crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
		}

		return 0;
	}

	int GetShowDebugShadows(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		bool enabled = false;
		__try
		{
			enabled = Ogre::GetShowDebugShadows(sceneManager);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Scene] getShowDebugShadows crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
		}

		lua_pushboolean(L, enabled ? 1 : 0);
		return 1;
	}

	int SetShowDebugShadows(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			return 0;
		}

		bool enabled = CheckBool(L, 1);
		__try
		{
			Ogre::SetShowDebugShadows(sceneManager, enabled);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Scene] setShowDebugShadows crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
		}

		return 0;
	}

	int GetViewportShadowsEnabled(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* viewport = GetCurrentViewport();
		if (viewport == nullptr)
		{
			lua_pushboolean(L, 0);
			return 1;
		}

		bool enabled = false;
		__try
		{
			enabled = Ogre::GetViewportShadowsEnabled(viewport);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Viewport] getShadowsEnabled crashed viewport=%p code=0x%08X", viewport, GetExceptionCode());
		}

		lua_pushboolean(L, enabled ? 1 : 0);
		return 1;
	}

	int SetViewportShadowsEnabled(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* viewport = GetCurrentViewport();
		if (viewport == nullptr)
		{
			return 0;
		}

		bool enabled = CheckBool(L, 1);
		__try
		{
			Ogre::SetViewportShadowsEnabled(viewport, enabled);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Viewport] setShadowsEnabled crashed viewport=%p code=0x%08X", viewport, GetExceptionCode());
		}

		return 0;
	}

	int GetSceneVisibilityMask(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			lua_pushnil(L);
			return 1;
		}

		uint32_t mask = 0;
		__try
		{
			mask = Ogre::GetSceneVisibilityMask(sceneManager);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Scene] getVisibilityMask crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
			lua_pushnil(L);
			return 1;
		}

		lua_pushinteger(L, mask);
		return 1;
	}

	int SetSceneVisibilityMask(lua_State* L)
	{
		Patch::TryInitializeOgre();

		auto* sceneManager = GetSceneManager();
		if (sceneManager == nullptr)
		{
			return 0;
		}

		uint32_t mask = static_cast<uint32_t>(luaL_checkinteger(L, 1));
		__try
		{
			Ogre::SetSceneVisibilityMask(sceneManager, mask);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			LogEnvironmentDebug("[EXU::Scene] setVisibilityMask crashed sceneManager=%p code=0x%08X", sceneManager, GetExceptionCode());
		}

		return 0;
	}

	int HasSkyBoxNode(lua_State* L)
	{
		Patch::TryInitializeOgre();

		bool hasNode = false;
		auto* sceneManager = GetSceneManager();
		if (sceneManager != nullptr)
		{
			TryHasSkyNode(sceneManager, Ogre::GetSkyBoxNode, hasNode, "skybox");
		}

		lua_pushboolean(L, hasNode);
		return 1;
	}

	int HasSkyDomeNode(lua_State* L)
	{
		Patch::TryInitializeOgre();

		bool hasNode = false;
		auto* sceneManager = GetSceneManager();
		if (sceneManager != nullptr)
		{
			TryHasSkyNode(sceneManager, Ogre::GetSkyDomeNode, hasNode, "skydome");
		}

		lua_pushboolean(L, hasNode);
		return 1;
	}

	int HasSkyPlaneNode(lua_State* L)
	{
		Patch::TryInitializeOgre();

		bool hasNode = false;
		auto* sceneManager = GetSceneManager();
		if (sceneManager != nullptr)
		{
			TryHasSkyNode(sceneManager, Ogre::GetSkyPlaneNode, hasNode, "skyplane");
		}

		lua_pushboolean(L, hasNode);
		return 1;
	}
}

namespace ExtraUtilities::Patch
{
	// Prevents fog reset function from running.
	InlinePatch fogResetPatch(fogReset, BasicPatch::RET, BasicPatch::Status::INACTIVE);

	/*
	* This waits to initialize the ogre patch until you call an ogre function
	* in order to prevent the game crashing when alt tabbed in the loading screen.
	*
	* The old EXU sun reset hooks were unstable on BZR 2.2.301 and could cause
	* access violations during repeated lighting updates. Callers that need
	* dynamic sunlight can safely reapply SetSun* on their own update loop.
	*/
	void TryInitializeOgre()
	{
		static bool done = false;
		if (!done)
		{
			auto* sceneManager = Lua::Environment::GetSceneManager();
			auto* terrainMasterLight = Lua::Environment::GetTerrainMasterLight();
			if (sceneManager == nullptr || terrainMasterLight == nullptr)
			{
				Lua::Environment::LogEnvironmentDebug(
					"[EXU::TryInitializeOgre] waiting sceneManager=%p terrainMasterLight=%p",
					sceneManager,
					terrainMasterLight);
				return;
			}

			fogResetPatch.Reload();
			Lua::Environment::LogEnvironmentDebug(
				"[EXU::TryInitializeOgre] initialized sceneManager=%p terrainMasterLight=%p",
				sceneManager,
				terrainMasterLight);
			done = true;
		}
	}
}
