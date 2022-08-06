#include <engine/physics/lua_physics.h>

int luaPhysicsEngineInit(lua_State *luaState)
{
	physicsEngineInit();
	return 0;
}

int luaPhysicsEngineCleanup(lua_State *luaState)
{
	physicsEngineCleanup();
	return 0;
}

int luaPhysicsEngineMakeStep(lua_State *luaState)
{
	physicsEngineMakeStep();
	return 0;
}

int luaPhysicsObjectDestroy(lua_State *luaState)
{
	struct primitiveData *pdata = (struct primitiveData *) lua_touserdata(luaState, 1);
	physicsObjectDestroy(pdata);
	return 0;
}

int luaPhysicsAllObjectsDestroy(lua_State *luaState)
{
	physicsAllObjectsDestroy();
	return 0;
}

int luaPhysicsLightDestroy(lua_State *luaState)
{
	struct lightSettings *ldata = (struct lightSettings *) lua_touserdata(luaState, 1);
	physicsLightDestroy(ldata);
	return 0;
}

int luaPhysicsAllLightsDestroy(lua_State *luaState)
{
	physicsAllLightsDestroy();
	return 0;
}

int luaPhysicsDirectionLightCreate(lua_State *luaState)
{
	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	position.z = luaL_checknumber(luaState, -1);
	
	glm::vec4 ambient;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	ambient.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	ambient.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	ambient.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	ambient.a = luaL_checknumber(luaState, -1);

	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 3);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 4);
	specular.a = luaL_checknumber(luaState, -1);

	struct lightSettings *ldata = physicsDirectionLightCreate(position, ambient, diffuse, specular);

	lua_pushlightuserdata(luaState, ldata);

	return 1;
}

int luaPhysicsDirectionShadowLightCreate(lua_State *luaState)
{
	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	position.z = luaL_checknumber(luaState, -1);
	
	glm::vec4 ambient;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	ambient.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	ambient.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	ambient.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	ambient.a = luaL_checknumber(luaState, -1);

	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 3);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 4);
	specular.a = luaL_checknumber(luaState, -1);

	int smsize = luaL_checkinteger(luaState, 5);

	float near = luaL_checknumber(luaState, 6);

	float far = luaL_checknumber(luaState, 7);

	float csmLambda = luaL_checknumber(luaState, 8);

	struct lightSettings *ldata = physicsDirectionShadowLightCreate(position, ambient, diffuse, specular, smsize, near, far, csmLambda);

	lua_pushlightuserdata(luaState, ldata);

	return 1;
}

int luaPhysicsPointLightCreate(lua_State *luaState)
{
	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	position.z = luaL_checknumber(luaState, -1);
	
	glm::vec4 ambient;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	ambient.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	ambient.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	ambient.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	ambient.a = luaL_checknumber(luaState, -1);

	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 3);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 4);
	specular.a = luaL_checknumber(luaState, -1);

	struct lightSettings *ldata = physicsPointLightCreate(position, ambient, diffuse, specular);

	lua_pushlightuserdata(luaState, ldata);

	return 1;
}

int luaPhysicsPointShadowLightCreate(lua_State *luaState)
{
	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	position.z = luaL_checknumber(luaState, -1);
	
	glm::vec4 ambient;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	ambient.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	ambient.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	ambient.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 2);
	ambient.a = luaL_checknumber(luaState, -1);

	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 3);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 3);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 3);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 3);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 4);
	specular.a = luaL_checknumber(luaState, -1);

	int smsize = luaL_checkinteger(luaState, 5);

	float near = luaL_checknumber(luaState, 6);

	float far = luaL_checknumber(luaState, 7);

	struct lightSettings *ldata = physicsPointShadowLightCreate(position, ambient, diffuse, specular, smsize, near, far);

	lua_pushlightuserdata(luaState, ldata);

	return 1;
}

int luaPhysicsSpotLightCreate(lua_State *luaState)
{
	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	position.z = luaL_checknumber(luaState, -1);
	
	glm::vec3 direction;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	direction.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	direction.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	direction.z = luaL_checknumber(luaState, -1);
	
	float cutoff = luaL_checknumber(luaState, 3);

	glm::vec4 ambient;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	ambient.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	ambient.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	ambient.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 4);
	ambient.a = luaL_checknumber(luaState, -1);

	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 5);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 5);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 5);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 5);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 6);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 6);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 6);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 6);
	specular.a = luaL_checknumber(luaState, -1);

	struct lightSettings *ldata = physicsSpotLightCreate(position, direction, cutoff, ambient, diffuse, specular);

	lua_pushlightuserdata(luaState, ldata);

	return 1;
}

int luaPhysicsSpotShadowLightCreate(lua_State *luaState)
{
	glm::vec3 position;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 1);
	position.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 1);
	position.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 1);
	position.z = luaL_checknumber(luaState, -1);
	
	glm::vec3 direction;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 2);
	direction.x = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 2);
	direction.y = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 2);
	direction.z = luaL_checknumber(luaState, -1);
	
	float cutoff = luaL_checknumber(luaState, 3);

	glm::vec4 ambient;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 4);
	ambient.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 4);
	ambient.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 4);
	ambient.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 4);
	ambient.a = luaL_checknumber(luaState, -1);

	glm::vec4 diffuse;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 5);
	diffuse.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 5);
	diffuse.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 5);
	diffuse.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 5);
	diffuse.a = luaL_checknumber(luaState, -1);

	glm::vec4 specular;
	lua_pushinteger(luaState, 1);
	lua_gettable(luaState, 6);
	specular.r = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 2);
	lua_gettable(luaState, 6);
	specular.g = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 3);
	lua_gettable(luaState, 6);
	specular.b = luaL_checknumber(luaState, -1);
	lua_pushinteger(luaState, 4);
	lua_gettable(luaState, 6);
	specular.a = luaL_checknumber(luaState, -1);

	int smsize = luaL_checkinteger(luaState, 7);

	float near = luaL_checknumber(luaState, 8);

	float far = luaL_checknumber(luaState, 9);

	struct lightSettings *ldata = physicsSpotShadowLightCreate(position, direction, cutoff, ambient, diffuse, specular, smsize, near, far);

	lua_pushlightuserdata(luaState, ldata);

	return 1;
}

void luaPhysicsEngineFunctionsRegister(void)
{
	luaFunctionRegister(luaPhysicsEngineInit, "physicsEngineInit");
	luaFunctionRegister(luaPhysicsEngineCleanup, "physicsEngineCleanup");
	luaFunctionRegister(luaPhysicsEngineMakeStep, "physicsEngineMakeStep");
	
	luaFunctionRegister(luaPhysicsObjectDestroy, "physicsObjectDestroy");
	luaFunctionRegister(luaPhysicsAllObjectsDestroy, "physicsAllObjectsDestroy");
	
	luaFunctionRegister(luaPhysicsLightDestroy, "lightDestroy");
	luaFunctionRegister(luaPhysicsAllLightsDestroy, "allLightsDestroy");

	luaFunctionRegister(luaPhysicsDirectionLightCreate, "directionLightCreate");
	luaFunctionRegister(luaPhysicsDirectionShadowLightCreate, "directionShadowLightCreate");
	
	luaFunctionRegister(luaPhysicsPointLightCreate, "pointLightCreate");
	luaFunctionRegister(luaPhysicsPointShadowLightCreate, "pointShadowLightCreate");
	
	luaFunctionRegister(luaPhysicsSpotLightCreate, "spotLightCreate");
	luaFunctionRegister(luaPhysicsSpotShadowLightCreate, "spotShadowLightCreate");
}

