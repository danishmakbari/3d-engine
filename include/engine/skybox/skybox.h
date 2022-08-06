#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <engine/shader/shader.h>
#include <engine/mvp/mvp.h>

struct skyboxData {
	glm::vec3 scale;
	GLuint texture;
	GLuint program;
	GLuint vao;
	GLuint vbo;
};

struct skyboxData *skyboxCreate(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, glm::vec3 scale);
void skyboxDestroy(struct skyboxData *sdata);
void skyboxDraw(struct skyboxData *sdata);

#endif
