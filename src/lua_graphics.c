#include <engine/graphics/lua_graphics.h>

int luaGraphicsInit(lua_State *luaState)
{
	float fovy = luaL_checknumber(luaState, 1);
	float near = luaL_checknumber(luaState, 2);
	float far = luaL_checknumber(luaState, 3);

	graphicsInit(fovy, near, far);

	return 0;
}

int luaDrawScene(lua_State *luaState)
{
	struct skyboxData *skybox = (struct skyboxData *) lua_touserdata(luaState, 1);
	drawScene(skybox);
	return 0;
}

int luaDrawShadows(lua_State *luaState)
{
	drawShadows();
	return 0;
}

void luaGraphicsFunctionsRegister(void)
{
	luaFunctionRegister(luaGraphicsInit, "graphicsInit");
	luaFunctionRegister(luaDrawScene, "drawScene");
	luaFunctionRegister(luaDrawShadows, "drawShadows");
}

