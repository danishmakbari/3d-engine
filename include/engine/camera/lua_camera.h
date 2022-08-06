#ifndef LUA_CAMERA_H
#define LUA_CAMERA_H

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/luawrapper/luawrapper.h>
#include <engine/camera/camera.h>

void luaCameraFunctionsRegister(void);

#endif

