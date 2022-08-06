#include <engine/utility/lua_utility.h>

int luaOpenglErrorsHandle(lua_State *luaState)
{
	openglErrorsHandle();
	return 0;
}

int luaFrametime(lua_State *luaState)
{
	Uint32 time = frametime();
	lua_pushinteger(luaState, time);
	return 1;
}

int luaFPS(lua_State *luaState)
{
	float frames = fps();
	lua_pushnumber(luaState, frames);
	return 1;
}

int luaNV_gpu_memmax_mb(lua_State *luaState)
{
	float memmax = nv_gpu_memmax_mb();
	lua_pushnumber(luaState, memmax);
	return 1;
}

int luaNV_gpu_memusage_mb(lua_State *luaState)
{
	float memusage = nv_gpu_memusage_mb();
	lua_pushnumber(luaState, memusage);
	return 1;
}

int luaGPU_vendor(lua_State *luaState)
{
	const char *str = (const char *) gpu_vendor();
	lua_pushstring(luaState, str);
	return 1;
}

int luaGPU_model(lua_State *luaState)
{
	const char *str = (const char *) gpu_model();
	lua_pushstring(luaState, str);
	return 1;
}

int luaOpenGL_version(lua_State *luaState)
{
	const char *str = (const char *) opengl_version();
	lua_pushstring(luaState, str);
	return 1;
}

int luaGLSL_version(lua_State *luaState)
{
	const char *str = (const char *) glsl_version();
	lua_pushstring(luaState, str);
	return 1;
}

void luaUtilityFunctionsRegister(void)
{
	luaFunctionRegister(luaOpenglErrorsHandle, "openglErrorsHandle");
	luaFunctionRegister(luaFrametime, "frametime");
	luaFunctionRegister(luaFPS, "fps");
	luaFunctionRegister(luaNV_gpu_memmax_mb, "nv_gpu_memmax_mb");
	luaFunctionRegister(luaNV_gpu_memusage_mb, "nv_gpu_memusage_mb");
	luaFunctionRegister(luaGPU_vendor, "gpu_vendor");
	luaFunctionRegister(luaGPU_model, "gpu_model");
	luaFunctionRegister(luaOpenGL_version, "opengl_version");
	luaFunctionRegister(luaGLSL_version, "glsl_version");
}

