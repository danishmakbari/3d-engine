#ifndef LUA_MATERIAL_H
#define LUA_MATERIAL_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/material/material.h>

void luaMaterialFunctionsRegister(void);

#endif

