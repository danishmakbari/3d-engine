#include <engine/mvp/lua_mvp.h>

int luaViewMatrixCameraPosGet(lua_State *luaState)
{
	glm::vec3 pos = viewMatrixCameraPosGet();
	lua_createtable(luaState, 3, 0);
	lua_pushnumber(luaState, pos.x);
	lua_rawseti(luaState, -2, 1);
	lua_pushnumber(luaState, pos.y);
	lua_rawseti(luaState, -2, 2);
	lua_pushnumber(luaState, pos.z);
	lua_rawseti(luaState, -2, 3);
	return 1;
}

int luaViewMatrixForwardVecGet(lua_State *luaState)
{
	glm::vec3 vec = viewMatrixForwardVecGet();
	lua_createtable(luaState, 3, 0);
	lua_pushnumber(luaState, vec.x);
	lua_rawseti(luaState, -2, 1);
	lua_pushnumber(luaState, vec.y);
	lua_rawseti(luaState, -2, 2);
	lua_pushnumber(luaState, vec.z);
	lua_rawseti(luaState, -2, 3);
	return 1;
}

int luaViewMatrixRightVecGet(lua_State *luaState)
{
	glm::vec3 vec = viewMatrixRightVecGet();
	lua_createtable(luaState, 3, 0);
	lua_pushnumber(luaState, vec.x);
	lua_rawseti(luaState, -2, 1);
	lua_pushnumber(luaState, vec.y);
	lua_rawseti(luaState, -2, 2);
	lua_pushnumber(luaState, vec.z);
	lua_rawseti(luaState, -2, 3);
	return 1;
}

int luaViewMatrixUpVecGet(lua_State *luaState)
{
	glm::vec3 vec = viewMatrixUpVecGet();
	lua_createtable(luaState, 3, 0);
	lua_pushnumber(luaState, vec.x);
	lua_rawseti(luaState, -2, 1);
	lua_pushnumber(luaState, vec.y);
	lua_rawseti(luaState, -2, 2);
	lua_pushnumber(luaState, vec.z);
	lua_rawseti(luaState, -2, 3);
	return 1;
}

void luaMVPFunctionsRegister(void)
{
	luaFunctionRegister(luaViewMatrixCameraPosGet, "cameraPosGet");
	luaFunctionRegister(luaViewMatrixUpVecGet, "cameraUpVecGet");
	luaFunctionRegister(luaViewMatrixForwardVecGet, "cameraForwardVecGet");
	luaFunctionRegister(luaViewMatrixRightVecGet, "cameraRightVecGet");
}

