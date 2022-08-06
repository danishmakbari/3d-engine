#include <engine/material/lua_material.h>

int luaMaterialListHeadCreate(lua_State *luaState)
{
	struct materialList *list = materialListHeadCreate();
	lua_pushlightuserdata(luaState, list);
	return 1;
}

int luaMaterialListElementsDestroy(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	materialListElementsDestroy(list);
	return 0;
}

int luaMaterialListHeadDestroy(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	materialListHeadDestroy(list);
	return 0;
}

int luaMaterialList2DElementAdd(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	
	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 3);
	specular.a = luaL_checknumber(luaState, -1);

	float shininess = luaL_checknumber(luaState, 4);
	
	glm::vec4 emissive;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 5);
	emissive.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 5);
	emissive.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 5);
	emissive.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 5);
	emissive.a = luaL_checknumber(luaState, -1);

	const char *diffuseMap = lua_tostring(luaState, 6);	
	const char *specularMap = lua_tostring(luaState, 7);	
	const char *emissiveMap = lua_tostring(luaState, 8);
	const char *normalMap = lua_tostring(luaState, 9);

	struct materialSettings *mdata = materialList2DElementAdd(list, diffuse, specular, shininess, emissive, diffuseMap, specularMap, emissiveMap, normalMap);

	lua_pushlightuserdata(luaState, mdata);

	return 1;
}

int luaMaterialListCubeElementAdd(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	
	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 3);
	specular.a = luaL_checknumber(luaState, -1);

	float shininess = luaL_checknumber(luaState, 4);
	
	glm::vec4 emissive;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 5);
	emissive.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 5);
	emissive.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 5);
	emissive.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 5);
	emissive.a = luaL_checknumber(luaState, -1);

	const char *diffuseMap[6];
	const char **dMap = NULL;
	if (!lua_isnil(luaState, 6)) {
		dMap = diffuseMap;
		for (int i = 0; i < 6; i++) {
			lua_pushinteger(luaState, i + 1);
			lua_gettable(luaState, 6);
			diffuseMap[i] = lua_tostring(luaState, -1);
		}
	}

	const char *specularMap[6];
	const char **sMap = NULL;
	if (!lua_isnil(luaState, 7)) {
		sMap = specularMap;
		for (int i = 0; i < 6; i++) {
			lua_pushinteger(luaState, i + 1);
			lua_gettable(luaState, 7);
			specularMap[i] = lua_tostring(luaState, -1);
		}
	}

	const char *emissiveMap[6];
	const char **eMap = NULL;
	if (!lua_isnil(luaState, 8)) {
		eMap = emissiveMap;
		for (int i = 0; i < 6; i++) {
			lua_pushinteger(luaState, i + 1);
			lua_gettable(luaState, 8);
			emissiveMap[i] = lua_tostring(luaState, -1);
		}
	}

	const char *normalMap[6];
	const char **nMap = NULL;	
	if (!lua_isnil(luaState, 9)) {
		nMap = normalMap;
		for (int i = 0; i < 6; i++) {
			lua_pushinteger(luaState, i + 1);
			lua_gettable(luaState, 8);
			normalMap[i] = lua_tostring(luaState, -1);
		}
	}

	struct materialSettings *mdata = materialListCubeElementAdd(list, diffuse, specular, shininess, emissive, dMap, sMap, eMap, nMap);

	lua_pushlightuserdata(luaState, mdata);

	return 1;
}

int luaMaterialListCube2DElementAdd(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	
	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 3);
	specular.a = luaL_checknumber(luaState, -1);

	float shininess = luaL_checknumber(luaState, 4);
	
	glm::vec4 emissive;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 5);
	emissive.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 5);
	emissive.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 5);
	emissive.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 5);
	emissive.a = luaL_checknumber(luaState, -1);

	const char *diffuseMap = lua_tostring(luaState, 6);	
	const char *specularMap = lua_tostring(luaState, 7);	
	const char *emissiveMap = lua_tostring(luaState, 8);
	const char *normalMap = lua_tostring(luaState, 9);

	struct materialSettings *mdata = materialListCube2DElementAdd(list, diffuse, specular, shininess, emissive, diffuseMap, specularMap, emissiveMap, normalMap);

	lua_pushlightuserdata(luaState, mdata);

	return 1;
}

int luaMaterialListElementRemove(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	struct materialSettings *mdata = (struct materialSettings *) lua_touserdata(luaState, 2);
	materialListElementRemove(list, mdata);
	return 0;
}

int luaMaterialListSizeGet(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	unsigned ret = materialListSizeGet(list);
	lua_pushinteger(luaState, ret);
	return 1;
}

int luaMaterialListElementGet(lua_State *luaState)
{
	struct materialList *list = (struct materialList *) lua_touserdata(luaState, 1);
	unsigned num = luaL_checkinteger(luaState, 2);
	struct materialSettings *mdata = materialListElementGet(list, num);
	lua_pushlightuserdata(luaState, mdata);
	return 1;
}

void luaMaterialFunctionsRegister(void)
{
	luaFunctionRegister(luaMaterialListHeadCreate, "materialListHeadCreate");
	luaFunctionRegister(luaMaterialListElementsDestroy, "materialListElementsDestroy");
	luaFunctionRegister(luaMaterialListHeadDestroy, "materialListHeadDestroy");
	luaFunctionRegister(luaMaterialList2DElementAdd, "materialList2DElementAdd");
	luaFunctionRegister(luaMaterialListCubeElementAdd, "materialListCubeElementAdd");
	luaFunctionRegister(luaMaterialListCube2DElementAdd, "materialListCube2DElementAdd");
	luaFunctionRegister(luaMaterialListElementRemove, "materialListElementRemove");
	luaFunctionRegister(luaMaterialListSizeGet, "materialListSizeGet");
	luaFunctionRegister(luaMaterialListElementGet, "materialListElementGet");
}

