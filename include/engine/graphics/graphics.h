#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <engine/window/window.h>
#include <engine/mvp/mvp.h>
#include <engine/drawlist/drawlist.h>

void graphicsInit(float fovy, float near, float far);

void drawScene(struct skyboxData *skybox);
void drawShadows(void);

#endif

