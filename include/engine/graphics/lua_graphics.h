#ifndef LUA_GRAPHICS_H
#define LUA_GRAPHICS_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/graphics/graphics.h>

void luaGraphicsFunctionsRegister(void);

#endif

