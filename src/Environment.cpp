#include "Environment.h"

namespace ExtraUtilities::Lua::Environment
{
	int GetGravity(lua_State* L)
	{
		BZR::VECTOR_3D g = gravity.Read();

		lua_getglobal(L, "SetVector");

		lua_pushnumber(L, g.x);
		lua_pushnumber(L, g.y);
		lua_pushnumber(L, g.z);

		lua_call(L, 3, 1);

		return 1;
	}

	int SetGravity(lua_State* L)
	{
		BZR::VECTOR_3D newGravity;
		if (lua_gettop(L) > 1)
		{
			newGravity.x = static_cast<float>(luaL_checknumber(L, 1));
			newGravity.y = static_cast<float>(luaL_checknumber(L, 2));
			newGravity.z = static_cast<float>(luaL_checknumber(L, 3));
		}
		else
		{
			int vectorIdx = lua_gettop(L);

			lua_getfield(L, vectorIdx, "x");
			newGravity.x = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, vectorIdx, "y");
			newGravity.y = static_cast<float>(luaL_checknumber(L, -1));

			lua_getfield(L, vectorIdx, "z");
			newGravity.z = static_cast<float>(luaL_checknumber(L, -1));
		}
		gravity.Write(newGravity);

		return 0;
	}
}