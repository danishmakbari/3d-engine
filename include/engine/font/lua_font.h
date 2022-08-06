#ifndef LUA_FONT_H
#define LUA_FONT_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/font/font.h>

void luaFontFunctionsRegister(void);

#endif

