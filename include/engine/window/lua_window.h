#ifndef LUA_WINDOW_H
#define LUA_WINDOW_H

#include <string>

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/window/window.h>

void luaWindowFunctionsRegister(void);

#endif

