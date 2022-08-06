#ifndef DRAWLIST_H
#define DRAWLIST_H

#include <ode/ode.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <engine/3dprimitives/3dprimitives.h>
#include <engine/light/light.h>
#include <engine/skybox/skybox.h>
#include <engine/window/window.h>
#include <engine/mvp/mvp.h>
#include <engine/camera/camera.h>

struct drawList {
	struct primitiveData *pdata;
	struct drawList *next;
	struct drawList *prev;
};

struct drawList *drawListHeadCreate(void);
void drawListHeadDestroy(struct drawList *root);
void drawListElementsDestroy(struct drawList *root);
void drawListElementAdd(struct drawList *root, struct primitiveData *pdata);
void drawListElementRemove(struct drawList *root, struct primitiveData *pdata);
unsigned drawListSizeGet(struct drawList *root);
struct primitiveData *drawListElementGet(struct drawList *root, unsigned num);

#endif

