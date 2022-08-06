#include <engine/camera/lua_camera.h>

int luaCameraSensitivitySet(lua_State *luaState)
{
	float s = luaL_checknumber(luaState, 1);
	cameraSensitivitySet(s);
	return 0;
}

int luaCameraSensitivityGet(lua_State *luaState)
{
	float s = cameraSensitivityGet();
	lua_pushnumber(luaState, s);
	return 1;
}

int luaCameraEyeSet(lua_State *luaState)
{
	glm::vec4 e;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	e.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	e.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	e.z = luaL_checknumber(luaState, -1);

	cameraEyeSet(e);

	return 0;
}

int luaCameraEyeGet(lua_State *luaState)
{
	glm::vec3 e = cameraEyeGet();

	lua_createtable(luaState, 3, 0);
	lua_pushnumber(luaState, e.x);
	lua_rawseti(luaState, -2, 1);
	lua_pushnumber(luaState, e.y);
	lua_rawseti(luaState, -2, 2);
	lua_pushnumber(luaState, e.z);
	lua_rawseti(luaState, -2, 3);

	return 1;
}

int luaCameraCenterSet(lua_State *luaState)
{
	glm::vec4 c;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	c.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	c.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	c.z = luaL_checknumber(luaState, -1);

	cameraCenterSet(c);

	return 0;
}

int luaCameraCenterGet(lua_State *luaState)
{
	glm::vec3 c = cameraCenterGet();

	lua_createtable(luaState, 3, 0);
	lua_pushnumber(luaState, c.x);
	lua_rawseti(luaState, -2, 1);
	lua_pushnumber(luaState, c.y);
	lua_rawseti(luaState, -2, 2);
	lua_pushnumber(luaState, c.z);
	lua_rawseti(luaState, -2, 3);

	return 1;
}

int luaFreeroamCameraMove(lua_State *luaState)
{
	float speed = luaL_checknumber(luaState, 1);
	freeroamCameraMove(speed);
	return 0;
}

void luaCameraFunctionsRegister(void)
{
	luaFunctionRegister(luaCameraSensitivitySet, "cameraSensitivitySet");
	luaFunctionRegister(luaCameraSensitivityGet, "cameraSensitivityGet");
	luaFunctionRegister(luaCameraEyeSet, "cameraEyeSet");
	luaFunctionRegister(luaCameraEyeGet, "cameraEyeGet");
	luaFunctionRegister(luaCameraCenterSet, "cameraCenterSet");
	luaFunctionRegister(luaCameraCenterGet, "cameraCenterGet");
	luaFunctionRegister(luaFreeroamCameraMove, "freeroamCameraMove");
}

