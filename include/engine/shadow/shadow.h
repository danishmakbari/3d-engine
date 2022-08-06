#ifndef SHADOW_H
#define SHADOW_H

#define CSM_SPLITS 4
#define POINT_LIGHTS_WITH_SHADOWS 6
#define SPOT_LIGHTS_WITH_SHADOWS 6

#define DIR_SM_OFFSET 0
#define POINT_SM_OFFSET 1
#define SPOT_SM_OFFSET 7
#define TEXTURE_OFFSET 13

#include <stdlib.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <engine/mvp/mvp.h>
#include <engine/drawlist/drawlist.h>
#include <engine/light/light.h>
#include <engine/shader/shader.h>

struct dirShadowMapData {
	float csmPlanes[CSM_SPLITS + 1];
	struct mvp *matrices;
	GLuint program;
	GLuint fbo;
	GLuint texture;
	GLuint size;
};

struct pointShadowMapData {
	float near;
	float far;
	GLuint program;
	GLuint fbo;
	GLuint texture;
	GLuint size;
};

struct spotShadowMapData {
	float near;
	float far;
	struct mvp *matrices;
	GLuint program;
	GLuint fbo;
	GLuint texture;
	GLuint size;
};

struct dirShadowMapData *dirShadowMapCreate(GLsizei shadowMapSize, float near, float far, float csmLambda);
void dirShadowMapDestroy(struct dirShadowMapData *smdata);

struct pointShadowMapData *pointShadowMapCreate(GLsizei shadowMapSize, float near, float far);
void pointShadowMapDestroy(struct pointShadowMapData *smdata);

struct spotShadowMapData *spotShadowMapCreate(GLsizei shadowMapSize, float near, float far);
void spotShadowMapDestroy(struct spotShadowMapData *smdata);

#endif

