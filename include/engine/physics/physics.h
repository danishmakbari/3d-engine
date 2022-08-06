#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <ode/ode.h>

#include <engine/mvp/mvp.h>
#include <engine/main/main.h>
#include <engine/drawlist/drawlist.h>
#include <engine/light/light.h>

struct physicsData {
        dWorldID world;
        dSpaceID space;
        dJointGroupID contactgroup;
};

void physicsEngineInit(void);
void physicsEngineCleanup(void);
void physicsEngineMakeStep(void);
struct physicsData *physicsEngineDataGet(void);

struct drawList *physicsDrawListGet(void);
void physicsObjectDestroy(struct primitiveData *pdata);
void physicsAllObjectsDestroy(void);

struct lightList *physicsLightListGet(void);

void physicsLightDestroy(struct lightSettings *ldata);
void physicsAllLightsDestroy(void);

struct lightSettings *physicsDirectionLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
struct lightSettings *physicsDirectionShadowLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
	GLsizei smsize, float near, float far, float csmLambda);

struct lightSettings *physicsPointLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
struct lightSettings *physicsPointShadowLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, GLsizei smsize, float near, float far);

struct lightSettings *physicsSpotLightCreate(glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
struct lightSettings *physicsSpotShadowLightCreate(glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
	GLsizei smsize, float near, float far);

#endif

