#pragma once

#include "InlinePatch.h"

#include <lua.hpp>

#include <cstdint>

namespace ExtraUtilities::Patch
{
	constexpr uintptr_t wingmanWeaponAimVftableEntry = 0x0088A4FC;
	constexpr uintptr_t walkerUpdateWeaponAim = 0x0060F320;
}

namespace ExtraUtilities::Lua::Patches
{
	int GetShotConvergence(lua_State* L);
	int SetShotConvergence(lua_State* L);
}