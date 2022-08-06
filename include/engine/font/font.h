#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <engine/shader/shader.h>
#include <engine/mvp/mvp.h>

struct fontData {
	int charnum;
	int px_h;
	GLuint *textures;
	int *w;
	int *h;
	int *bearing_x;
	int *bearing_y;
	int *advance_x;
	GLuint vao;
	GLuint vbo;
	GLuint program;
};

glm::vec2 gprintf(struct fontData *fdata, glm::vec4 color, glm::vec2 position, const char* fmt, ...);
struct fontData *fontRender(const char *path, int px);
void fontDestroy(struct fontData *fdata);

#endif

