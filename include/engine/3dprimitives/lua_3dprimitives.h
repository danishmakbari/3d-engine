#ifndef LUA_3DPRIMITIVES_H
#define LUA_3DPRIMITIVES_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/3dprimitives/3dprimitives.h>

void lua3DPrimitivesFunctionsRegister(void);

#endif

