#include <engine/skybox/lua_skybox.h>

int luaSkyboxCreate(lua_State *luaState)
{
	const char *px = luaL_checkstring(luaState, 1);	
	const char *nx = luaL_checkstring(luaState, 2);
	const char *py = luaL_checkstring(luaState, 3);
	const char *ny = luaL_checkstring(luaState, 4);
	const char *pz = luaL_checkstring(luaState, 5);
	const char *nz = luaL_checkstring(luaState, 6);

	glm::vec3 scale;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 7);
	scale.x = luaL_checknumber(luaState, -1);

	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 7);
	scale.y = luaL_checknumber(luaState, -1);

	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 7);
	scale.z = luaL_checknumber(luaState, -1);

	struct skyboxData *skybox = skyboxCreate(px, nx, py, ny, pz, nz, scale);
	lua_pushlightuserdata(luaState, skybox);

	return 1;
}

int luaSkyboxDestroy(lua_State *luaState)
{
	struct skyboxData *skybox = (struct skyboxData *) lua_touserdata(luaState, 1);
	skyboxDestroy(skybox);
	return 0;
}

int luaSkyboxDraw(lua_State *luaState)
{
	struct skyboxData *skybox = (struct skyboxData *) lua_touserdata(luaState, 1);
	skyboxDraw(skybox);
	return 0;
}

void luaSkyboxFunctionsRegister(void)
{
	luaFunctionRegister(luaSkyboxCreate, "skyboxCreate");
	luaFunctionRegister(luaSkyboxDestroy, "skyboxDestroy");
	luaFunctionRegister(luaSkyboxDraw, "skyboxDraw");
}

