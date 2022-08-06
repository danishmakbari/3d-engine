#ifndef LUA_MVP_H
#define LUA_MVP_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/mvp/mvp.h>

void luaMVPFunctionsRegister(void);

#endif

