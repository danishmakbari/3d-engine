#include <engine/luawrapper/luawrapper.h>

static lua_State *luaState;

void luaInit(void)
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	luaWindowFunctionsRegister();
	luaPhysicsEngineFunctionsRegister();
	luaMVPFunctionsRegister();
	luaSkyboxFunctionsRegister();
	luaFontFunctionsRegister();
	luaMaterialFunctionsRegister();
	lua3DPrimitivesFunctionsRegister();
	luaUtilityFunctionsRegister();	
	luaCameraFunctionsRegister();
	luaGraphicsFunctionsRegister();
}

void luaCleanup(void)
{
	lua_close(luaState);
}

int luaFileExecute(const char *file)
{
	return luaL_dofile(luaState, file);
}

int luaStringExecute(const char *string)
{
	return luaL_dostring(luaState, string);
}

void luaGlobalIntVarSet(const char *name, int value)
{
	lua_pushinteger(luaState, value);
	lua_setglobal(luaState, name);
}

void luaGlobalFloatVarSet(const char *name, float value)
{
	lua_pushnumber(luaState, value);
	lua_setglobal(luaState, name);
}

void luaGlobalStringVarSet(const char *name, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args) + 1;
	char *buf = (char *) malloc(size);
	va_end(args);

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	lua_pushstring(luaState, buf);
	lua_setglobal(luaState, name);

	free(buf);
}


int luaGlobalIntVarGet(const char *name)
{
	lua_getglobal(luaState, name);
	int ret = luaL_checkinteger(luaState, -1);
	lua_pop(luaState, 1);

	return ret;
}

float luaGlobalFloatVarGet(const char *name)
{
	lua_getglobal(luaState, name);
	float ret = luaL_checknumber(luaState, -1);
	lua_pop(luaState, 1);

	return ret;
}

const char *luaGlobalStringVarGet(const char *name)
{
	lua_getglobal(luaState, name);
	const char *ret = luaL_checkstring(luaState, -1);
	lua_pop(luaState, 1);

	return ret;
}

void luaFunctionRegister(int (*func)(lua_State *), const char *name)
{
	lua_pushcfunction(luaState, func);
	lua_setglobal(luaState, name);
}



