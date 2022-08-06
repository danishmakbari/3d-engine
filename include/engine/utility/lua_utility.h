#ifndef LUA_UTILITY_H
#define LUA_UTILITY_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/utility/utility.h>

void luaUtilityFunctionsRegister(void);

#endif

