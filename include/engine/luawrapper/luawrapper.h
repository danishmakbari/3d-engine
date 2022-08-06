#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

#include <stdarg.h>
#include <stdlib.h>

extern "C" {
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#include <engine/window/lua_window.h>
#include <engine/physics/lua_physics.h>
#include <engine/mvp/lua_mvp.h>
#include <engine/skybox/lua_skybox.h>
#include <engine/font/lua_font.h>
#include <engine/material/lua_material.h>
#include <engine/3dprimitives/lua_3dprimitives.h>
#include <engine/utility/lua_utility.h>
#include <engine/camera/lua_camera.h>
#include <engine/graphics/lua_graphics.h>

void luaInit(void);
void luaCleanup(void);

int luaFileExecute(const char *file);
int luaStringExecute(const char *string);

void luaGlobalIntVarSet(const char *name, int value);
void luaGlobalFloatVarSet(const char *name, float value);
void luaGlobalStringVarSet(const char *name, const char *fmt, ...);

int luaGlobalIntVarGet(const char *name);
float luaGlobalFloatVarGet(const char *name);
const char *luaGlobalStringVarGet(const char *name);

void luaFunctionRegister(int (*func)(lua_State *), const char *name);

#endif

