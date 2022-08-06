#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <ode/ode.h>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <engine/drawlist/drawlist.h>
#include <engine/shader/shader.h>
#include <engine/light/light.h>
#include <vector>

#include <engine/physics/physics.h>
#include <engine/mvp/mvp.h>

struct primitiveData {
	bool back_cullface;
	bool front_cullface;
	GLuint vao;
	GLuint vbo;
	GLuint program;
	struct materialList *material;
	dGeomID geom;
	dBodyID body;
	size_t size;
};

struct primitiveData *sphereCreate(struct materialList *material, dReal mass, dReal radius, glm::vec3 position, bool back_cullface, bool front_cullface);
void sphereDraw(struct primitiveData *pdata);
void sphereDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void spherePointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6]);
void sphereSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void sphereDestroy(struct primitiveData *pdata);

struct primitiveData *boxCreate(struct materialList *material, dReal mass, glm::vec3 scale, glm::vec3 position, bool back_cullface, bool front_cullface);
void boxDraw(struct primitiveData *pdata);
void boxDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void boxPointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6]);
void boxSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void boxDestroy(struct primitiveData *pdata);

struct primitiveData *cylinderCreate(struct materialList *material, dReal mass, dReal radius, dReal height, glm::vec3 position, bool back_cullface, bool front_cullface);
void cylinderDraw(struct primitiveData *pdata);
void cylinderDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void cylinderPointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6]);
void cylinderSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void cylinderDestroy(struct primitiveData *pdata);

struct primitiveData *terrainCreate(struct materialList *material, const char *terrainMap, glm::vec3 scale, bool back_cullface, bool front_cullface);
void terrainDraw(struct primitiveData *pdata);
void terrainDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void terrainPointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6]);
void terrainSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices);
void terrainDestroy(struct primitiveData *pdata);

#endif

