#include <engine/3dprimitives/lua_3dprimitives.h>

int luaSphereCreate(lua_State *luaState)
{
	struct materialList *material = (struct materialList *) lua_touserdata(luaState, 1);
	float mass = luaL_checknumber(luaState, 2);
	float radius = luaL_checknumber(luaState, 3);
	
	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	position.z = luaL_checknumber(luaState, -1);

	bool back_cullface = lua_toboolean(luaState, 5);
	bool front_cullface = lua_toboolean(luaState, 6);

	struct primitiveData *pdata = sphereCreate(material, mass, radius, position, back_cullface, front_cullface);
	lua_pushlightuserdata(luaState, pdata);

	return 1;
}

int luaBoxCreate(lua_State *luaState)
{
	struct materialList *material = (struct materialList *) lua_touserdata(luaState, 1);
	float mass = luaL_checknumber(luaState, 2);
	
	glm::vec3 scale;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	scale.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	scale.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	scale.z = luaL_checknumber(luaState, -1);

	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	position.z = luaL_checknumber(luaState, -1);

	bool back_cullface = lua_toboolean(luaState, 5);
	bool front_cullface = lua_toboolean(luaState, 6);

	struct primitiveData *pdata = boxCreate(material, mass, scale, position, back_cullface, front_cullface);
	lua_pushlightuserdata(luaState, pdata);

	return 1;
}

int luaCylinderCreate(lua_State *luaState)
{
	struct materialList *material = (struct materialList *) lua_touserdata(luaState, 1);
	float mass = luaL_checknumber(luaState, 2);
	float radius = luaL_checknumber(luaState, 3);
	float height = luaL_checknumber(luaState, 4);

	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 5);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 5);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 5);
	position.z = luaL_checknumber(luaState, -1);

	bool back_cullface = lua_toboolean(luaState, 6);
	bool front_cullface = lua_toboolean(luaState, 7);

	struct primitiveData *pdata = cylinderCreate(material, mass, radius, height, position, back_cullface, front_cullface);
	lua_pushlightuserdata(luaState, pdata);

	return 1;
}

int luaTerrainCreate(lua_State *luaState)
{
	struct materialList *material = (struct materialList *) lua_touserdata(luaState, 1);
	const char *terrainMap = luaL_checkstring(luaState, 2);
	
	glm::vec3 scale;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	scale.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	scale.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	scale.z = luaL_checknumber(luaState, -1);

	bool back_cullface = lua_toboolean(luaState, 4);
	bool front_cullface = lua_toboolean(luaState, 5);

	struct primitiveData *pdata = terrainCreate(material, terrainMap, scale, back_cullface, front_cullface);
	lua_pushlightuserdata(luaState, pdata);

	return 1;
}

void lua3DPrimitivesFunctionsRegister(void)
{
	luaFunctionRegister(luaSphereCreate, "sphereCreate");
	luaFunctionRegister(luaBoxCreate, "boxCreate");
	luaFunctionRegister(luaCylinderCreate, "cylinderCreate");
	luaFunctionRegister(luaTerrainCreate, "terrainCreate");
}

