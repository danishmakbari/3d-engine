#include <engine/font/lua_font.h>

int luaFontRender(lua_State *luaState)
{
	const char *path = luaL_checkstring(luaState, 1);
	int height = luaL_checkinteger(luaState, 2);
	struct fontData *fdata = fontRender(path, height);
	lua_pushlightuserdata(luaState, fdata);
	return 1;
}

int luaFontDestroy(lua_State *luaState)
{
	struct fontData *fdata = (struct fontData *) lua_touserdata(luaState, 1);
	fontDestroy(fdata);
	return 0;
}

int luaGprint(lua_State *luaState)
{
	struct fontData *fdata = (struct fontData *) lua_touserdata(luaState, 1);
	
	glm::vec4 color;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	color.r = luaL_checknumber(luaState, -1);

	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	color.g = luaL_checknumber(luaState, -1);

	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	color.b = luaL_checknumber(luaState, -1);

	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	color.a = luaL_checknumber(luaState, -1);

	glm::vec2 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	position.x = luaL_checknumber(luaState, -1);

	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	position.y = luaL_checknumber(luaState, -1);

	const char *fmt = luaL_checkstring(luaState, 4);

	glm::vec2 res = gprintf(fdata, color, position, fmt);

	lua_createtable(luaState, 2, 0);
	lua_pushnumber(luaState, res.x);
	lua_rawseti(luaState, -2, 1);
	lua_pushnumber(luaState, res.y);
	lua_rawseti(luaState, -2, 2);

	return 1;
}

static const char *gprint_wrap = {
	"function gprintf(font, color, position, fmt, ...)\n"
	"	local str = string.format(fmt, ...)\n"
	"	local ret = gprint(font, color, position, str)\n"
	"	return ret\n"
	"end\n"

};

void luaFontFunctionsRegister(void)
{
	luaFunctionRegister(luaFontRender, "fontRender");
	luaFunctionRegister(luaFontDestroy, "fontDestroy");
	
	luaFunctionRegister(luaGprint, "gprint");
	luaStringExecute(gprint_wrap);
}

