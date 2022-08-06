#ifndef LUA_PHYSICS_H
#define LUA_PHYSICS_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/physics/physics.h>

void luaPhysicsEngineFunctionsRegister(void);

#endif

