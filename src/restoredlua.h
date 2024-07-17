#pragma once
#include "lua.hpp"

extern "C" {
    int db_debug(lua_State* L);
    int db_getfenv(lua_State* L);
    int db_gethook(lua_State* L);
    int db_getinfo(lua_State* L);
    int db_getlocal(lua_State* L);
    int db_getregistry(lua_State* L);
    int db_getmetatable(lua_State* L);
    int db_getupvalue(lua_State* L);
    int db_setfenv(lua_State* L);
    int db_sethook(lua_State* L);
    int db_setlocal(lua_State* L);
    int db_setmetatable(lua_State* L);
    int db_setupvalue(lua_State* L);
    int db_errorfb(lua_State* L);
    int luaopen_debug(lua_State* L);
}