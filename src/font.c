#include <engine/font/font.h>

static GLuint font_vao;
static GLuint font_vbo;
static int fontCounter = 0;

static GLuint shaderProgram;
static int shaderProgramCounter = 0;

static char vertexshader[] = {
	"#version 330 core\n"

	"layout(location = 0) in vec2 position;"
	"layout(location = 1) in vec2 texCoord;"

	"out vec2 tex;"

	"uniform mat4 mvp;"

	"void main()"
	"{"
	"	tex = texCoord;"
	"	gl_Position = mvp * vec4(position, 0, 1);"
	"}\n"
};

static char fragmentshader[] = {
	"#version 330 core\n"

	"in vec2 tex;"

	"layout(location = 0) out vec4 fragColor;"

	"uniform sampler2D glyph;"
	"uniform vec4 color;"

	"void main()"
	"{"
	"	fragColor = vec4(color.rgb, texture(glyph, tex).r);"
	"}\n"
};

struct fontData *fontRender(const char *path, int px)
{
	struct fontData *fdata = (struct fontData *) malloc(sizeof(struct fontData));

	FT_Library lib;
	FT_Face face;
	FT_Init_FreeType(&lib);
	FT_New_Face(lib, path, 0, &face);
	FT_Set_Pixel_Sizes(face, 0, px);

	if (!fontCounter) {
		glGenVertexArrays(1, &font_vao);
		glGenBuffers(1, &font_vbo);

		glBindVertexArray(font_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, font_vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void *) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void *) (sizeof(GLfloat) * 2));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	fontCounter++;
	fdata->vao = font_vao;
	fdata->vbo = font_vbo;

	if (!shaderProgramCounter)
		shaderProgram = shaderProgramCompile(vertexshader, fragmentshader, NULL);
	shaderProgramCounter++;
	fdata->program = shaderProgram;

	fdata->charnum = 128;
	fdata->textures = (GLuint *) malloc(sizeof(GLuint) * fdata->charnum);
	fdata->px_h = px;
	fdata->w = (int *) malloc(sizeof(int) * fdata->charnum);
	fdata->h = (int *) malloc(sizeof(int) * fdata->charnum);	
	fdata->bearing_x = (int *) malloc(sizeof(int) * fdata->charnum);
	fdata->bearing_y = (int *) malloc(sizeof(int) * fdata->charnum);
	fdata->advance_x = (int *) malloc(sizeof(int) * fdata->charnum);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(fdata->charnum, fdata->textures);

	for (int i = 0; i < fdata->charnum; i++) {
		FT_Load_Char(face, i, FT_LOAD_RENDER);
		glBindTexture(GL_TEXTURE_2D, fdata->textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
			GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		fdata->w[i] = face->glyph->bitmap.width;
		fdata->h[i] = face->glyph->bitmap.rows;
		fdata->bearing_x[i] = face->glyph->bitmap_left;
		fdata->bearing_y[i] = face->glyph->bitmap_top;
		fdata->advance_x[i] = face->glyph->advance.x >> 6;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(lib);

	return fdata;
}

void fontDestroy(struct fontData *fdata)
{
	shaderProgramCounter--;
	if (!shaderProgramCounter)
		shaderProgramDelete(fdata->program);

	fontCounter--;
	if (!fontCounter) {
		glDeleteVertexArrays(1, &fdata->vao);
		glDeleteBuffers(1, &fdata->vbo);
	}

	glDeleteTextures(fdata->charnum, fdata->textures);
	free(fdata->textures);
	free(fdata->w);
	free(fdata->h);
	free(fdata->bearing_x);
	free(fdata->bearing_y);
	free(fdata->advance_x);
	free(fdata);
}


glm::vec2 gprintf(struct fontData *fdata, glm::vec4 color, glm::vec2 position, const char* fmt, ...)
{
	/* Draw text only after all objects!!!!!! */

	va_list args;

	va_start(args, fmt);
	int size = vsnprintf(NULL, 0, fmt, args) + 1;
	char *buf = (char *) malloc(size);
	va_end(args);

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	struct mvp matrices;
	modelMatrixLoadIdentity(&matrices);
	viewMatrixLoadIdentity(&matrices);
	projMatrixLoadIdentity(&matrices);
	projMatrixOrtho(&matrices, 0, windowWidthGet() - 1, windowHeightGet() - 1, 0, 1, -1);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	shaderProgramUse(fdata->program);
	shaderProgramSet1i(fdata->program, "glyph", 0);
	shaderProgramSet4f(fdata->program, "color", color.r, color.g, color.b, color.a);
	shaderProgramSetMatrix4fv(fdata->program, "mvp", 1, GL_FALSE, glm::value_ptr(MVPMatrixGet(&matrices)));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fdata->vao);
	glBindBuffer(GL_ARRAY_BUFFER, fdata->vbo);
	glDepthFunc(GL_ALWAYS);

	position.x = glm::clamp((double) position.x, 0.0, 1.0) * windowWidthGet();
	position.y = glm::clamp((double) position.y, 0.0, 1.0) * windowHeightGet() + fdata->px_h;
	int xtemp = position.x;
	int ytemp = position.y;
	float retx = 0;
	float rety = 0;
	for (int i = 0; i < size - 1; i++) {
		if (buf[i] == '\n') {
			xtemp = position.x;
			ytemp += fdata->px_h;

			retx = xtemp;
			rety = ytemp + (fdata->h[(int) buf[i]] - fdata->bearing_y[(int) buf[i]]) - fdata->px_h;

			continue;
		}

		glBindTexture(GL_TEXTURE_2D, fdata->textures[(int) buf[i]]);

		GLfloat xpos = xtemp + fdata->bearing_x[(int) buf[i]];
		GLfloat ypos = ytemp + (fdata->h[(int) buf[i]] - fdata->bearing_y[(int) buf[i]]);
		GLfloat w = fdata->w[(int) buf[i]];
		GLfloat h = fdata->h[(int) buf[i]];

		GLfloat vertices[] = {
			xpos,     ypos - h, 0, 0,
			xpos,     ypos,     0, 1,
			xpos + w, ypos,     1, 1,

			xpos,     ypos - h, 0, 0,
			xpos + w, ypos,     1, 1,
			xpos + w, ypos - h, 1, 0
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		xtemp += fdata->advance_x[(int) buf[i]];

		retx = xtemp + fdata->bearing_x[(int) buf[i]];
		rety = ytemp + (fdata->h[(int) buf[i]] - fdata->bearing_y[(int) buf[i]]) - fdata->px_h;
	}
	glDepthFunc(GL_LESS);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shaderProgramUse(0);

	free(buf);
	
	glDisable(GL_BLEND);

	return glm::vec2(retx / windowWidthGet(), rety / windowHeightGet());
}

