/* Copyright (C) 2023-2025 VTrider
 *
 * This file is part of Extra Utilities.
 */

#pragma once

#include "BZR.h"
#include <lua.hpp>

namespace ExtraUtilities::Culling
{
    void UpdateUnit(BZR::GameObject* obj);

    inline float cullDistance = 500.0f;
    inline bool enabled = false;
}

namespace ExtraUtilities::Lua::Culling
{
    int SetCullDistance(lua_State* L);
    int GetCullDistance(lua_State* L);
    int SetCullingEnabled(lua_State* L);
    int GetCullingEnabled(lua_State* L);
}
