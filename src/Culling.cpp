/* Copyright (C) 2023-2025 VTrider
 *
 * This file is part of Extra Utilities.
 */

#include "Culling.h"
#include "Ogre.h"
#include "Camera.h"
#include "LuaHelpers.h"

namespace ExtraUtilities::Culling
{
    void UpdateUnit(BZR::GameObject* obj)
    {
        if (!enabled) return;

        BZR::BZR_Camera* cam = Lua::Camera::mainCam.Get();
        if (!cam || !obj) return;

        // Calculate distance (squared for performance)
        double dx = obj->pos.x - cam->Matrix.posit_x;
        double dy = obj->pos.y - cam->Matrix.posit_y;
        double dz = obj->pos.z - cam->Matrix.posit_z;
        double distSq = dx*dx + dy*dy + dz*dz;

        void* entity = obj->GetOgreEntity();
        if (entity)
        {
            bool visible = (distSq < (double)cullDistance * cullDistance);
            Ogre::SetVisible(entity, visible);
        }
    }
}

namespace ExtraUtilities::Lua::Culling
{
    int SetCullDistance(lua_State* L)
    {
        ExtraUtilities::Culling::cullDistance = (float)luaL_checknumber(L, 1);
        return 0;
    }

    int GetCullDistance(lua_State* L)
    {
        lua_pushnumber(L, ExtraUtilities::Culling::cullDistance);
        return 1;
    }

    int SetCullingEnabled(lua_State* L)
    {
        ExtraUtilities::Culling::enabled = CheckBool(L, 1);
        return 0;
    }

    int GetCullingEnabled(lua_State* L)
    {
        lua_pushboolean(L, ExtraUtilities::Culling::enabled);
        return 1;
    }
}
