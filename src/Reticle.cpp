#include "Reticle.h"

namespace ExtraUtilities::Lua::Reticle
{
	int GetPosition(lua_State* L)
	{
		BZR::VECTOR_3D pos = position.Read();

		lua_getglobal(L, "SetVector");

		lua_pushnumber(L, pos.x);
		lua_pushnumber(L, pos.y);
		lua_pushnumber(L, pos.z);

		lua_call(L, 3, 1);

		return 1;
	}

	int GetRange(lua_State* L)
	{
		lua_pushnumber(L, range.Read());
		return 1;
	}

	int SetRange(lua_State* L)
	{
		float newRange = static_cast<float>(luaL_checknumber(L, 1));
		range.Write(newRange);
		return 0;
	}

	int GetObject(lua_State* L)
	{
		auto handle = BZR::GameObject::GetHandle(reinterpret_cast<int>(object.Read()));
		lua_pushlightuserdata(L, reinterpret_cast<void*>(handle));
		return 1;
	}

	int GetMatrix(lua_State* L)
	{
		lua_getglobal(L, "SetMatrix");
		return 1;
	}
}