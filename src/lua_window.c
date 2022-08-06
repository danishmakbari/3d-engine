#include <engine/window/lua_window.h>

int luaWindowInit(lua_State *luaState)
{
	const char *windowName = luaL_checkstring(luaState, 1);
	int width = luaL_checkinteger(luaState, 2);
	int height = luaL_checkinteger(luaState, 3);
	windowInit(windowName, width, height);
	return 0;
}

int luaWindowEventsHandle(lua_State *luaState)
{
	bool retval = windowEventsHandle();
	lua_pushboolean(luaState, retval);
	return 1;
}

int luaWindowCleanup(lua_State *luaState)
{
	windowCleanup();
	return 0;
}

int luaWindowSizeSet(lua_State *luaState)
{
	int width = luaL_checkinteger(luaState, 1);
	int height = luaL_checkinteger(luaState, 2);
	windowSizeSet(width, height);
	return 0;
}

int luaWindowTitleSet(lua_State *luaState)
{
	const char *title = luaL_checkstring(luaState, 1);
	windowTitleSet(title);
	return 0;
}

int luaWindowModeSet(lua_State *luaState)
{
	std::string str(luaL_checkstring(luaState, 1));

	if (str == "windowmode_windowed")
		windowModeSet(WINDOWMODE_WINDOWED);
	else if (str == "windowmode_fullscreen")
		windowModeSet(WINDOWMODE_FULLSCREEN);
	else if (str == "windowmode_windowed_borderless")
		windowModeSet(WINDOWMODE_WINDOWED_BORDERLESS);

	return 0;
}

int luaWindowRelativeCursorSet(lua_State *luaState)
{
	bool mode = lua_toboolean(luaState, 1);

	windowRelativeCursorSet(mode);

	return 0;
}

int luaWindowWidthGet(lua_State *luaState)
{
	int retval = windowWidthGet();
	lua_pushinteger(luaState, retval);
	return 1;
}

int luaWindowHeightGet(lua_State *luaState)
{
	int retval = windowHeightGet();
	lua_pushinteger(luaState, retval);
	return 1;
}

int luaWindowSwapFramebuffers(lua_State *luaState)
{
	windowSwapFramebuffers();
	return 0;
}


void luaWindowFunctionsRegister(void)
{
	luaFunctionRegister(luaWindowInit, "windowInit");
	luaFunctionRegister(luaWindowEventsHandle, "windowEventsHandle");
	luaFunctionRegister(luaWindowCleanup, "windowCleanup");
	luaFunctionRegister(luaWindowSizeSet, "windowSizeSet");
	luaFunctionRegister(luaWindowTitleSet, "windowTitleSet");
	luaFunctionRegister(luaWindowModeSet, "windowModeSet");
	luaFunctionRegister(luaWindowRelativeCursorSet, "windowRelativeCursorSet");
	luaFunctionRegister(luaWindowWidthGet, "windowWidthGet");
	luaFunctionRegister(luaWindowHeightGet, "windowHeightGet");
	luaFunctionRegister(luaWindowSwapFramebuffers, "windowSwapFramebuffers");
}

