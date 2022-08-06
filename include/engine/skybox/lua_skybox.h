#ifndef LUA_SKYBOX_H
#define LUA_SKYBOX_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/skybox/skybox.h>

void luaSkyboxFunctionsRegister(void);

#endif

