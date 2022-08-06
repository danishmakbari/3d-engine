#ifndef LIGHT_H
#define LIGHT_H

#define MAX_LIGHTS 16

#define DIRECTION_LIGHT 0
#define DIRECTION_SHADOW_LIGHT 1
#define POINT_LIGHT 2
#define POINT_SHADOW_LIGHT 3
#define SPOT_LIGHT 4
#define SPOT_SHADOW_LIGHT 5

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include <engine/drawlist/drawlist.h>
#include <engine/shadow/shadow.h>
#include <engine/mvp/mvp.h>

struct lightList {
	struct lightSettings *ldata;
	struct lightList *next;
	struct lightList *prev;
};

struct lightSettings {
	int lightType;
	glm::vec3 position;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	glm::vec3 direction;
	float cutoff;

	struct dirShadowMapData *dsmdata;
	struct pointShadowMapData *psmdata;
	struct spotShadowMapData *ssmdata;
};

struct lightList *lightListHeadCreate(void);
void lightListElementsDestroy(struct lightList *root);
void lightListHeadDestroy(struct lightList *root);
void lightListElementRemove(struct lightList *root, struct lightSettings *ldata);
unsigned lightListSizeGet(struct lightList *root);
struct lightSettings *lightListElementGet(struct lightList *root, unsigned num);

struct lightSettings *directionLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
struct lightSettings *directionShadowLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
	GLsizei smsize, float near, float far, float csmLambda);

struct lightSettings *pointLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
struct lightSettings *pointShadowLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, GLsizei smsize, float near, float far);

struct lightSettings *spotLightCreate(struct lightList *root, glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
struct lightSettings *spotShadowLightCreate(struct lightList *root, glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
	GLsizei smsize, float near, float far);

#endif

