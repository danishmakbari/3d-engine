#include <engine/skybox/skybox.h>

static GLuint skybox_vao;
static GLuint skybox_vbo;
static int skyboxCounter = 0;

static GLuint shaderProgram;
static int shaderProgramCounter = 0;

static GLfloat skyboxVertices[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

static char vertexshader[] = {
	"#version 330 core\n"

	"layout(location = 0) in vec3 position;"

	"out vec3 texCoord;"

	"uniform mat4 mvp;"

	"void main()"
	"{"
	"	texCoord = position;"
	"	gl_Position = mvp * vec4(position, 1);"
	"}\n"
};

static char fragmentshader[] = {
	"#version 330 core\n"

	"in vec3 texCoord;"

	"layout(location = 0) out vec4 fragColor;"

	"uniform samplerCube skybox;"

	"void main()"
	"{"
	"	fragColor = texture(skybox, texCoord);"
	"}\n"
};

struct skyboxData *skyboxCreate(const char *px, const char *nx, const char *py, const char *ny, const char *pz, const char *nz, glm::vec3 scale)
{
	struct skyboxData *sdata = (struct skyboxData *) malloc(sizeof(struct skyboxData));
	sdata->scale = scale;

	if (!skyboxCounter) {
		glGenVertexArrays(1, &skybox_vao);
		glGenBuffers(1, &skybox_vbo);
	
		glBindVertexArray(skybox_vao);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	skyboxCounter++;
	sdata->vao = skybox_vao;
	sdata->vbo = skybox_vbo;

	glGenTextures(1, &sdata->texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sdata->texture);

	const char *files[6] = {px, nx, py, ny, pz, nz};
	for (int i = 0; i < 6; i++) {
		SDL_Surface *surface = IMG_Load(files[i]);

		if (surface->format->BytesPerPixel == 3)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		else if (surface->format->BytesPerPixel == 4)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

		SDL_FreeSurface(surface);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	if (!shaderProgramCounter)
		shaderProgram = shaderProgramCompile(vertexshader, fragmentshader, NULL);
	shaderProgramCounter++;
	sdata->program = shaderProgram;

	return sdata;
}

void skyboxDestroy(struct skyboxData *sdata)
{
	glDeleteTextures(1, &sdata->texture);
	
	skyboxCounter--;
	if (!skyboxCounter) {
		glDeleteVertexArrays(1, &sdata->vao);
		glDeleteBuffers(1, &sdata->vbo);
	}

	shaderProgramCounter--;
	if (!shaderProgramCounter)
		shaderProgramDelete(sdata->program);
	
	free(sdata);
}

void skyboxDraw(struct skyboxData *sdata)
{
	/* Draw before all objects!!!! */

	glDepthMask(GL_FALSE);

	modelMatrixLoadIdentity();
	modelMatrixScale(sdata->scale);

	glm::mat4 model = modelMatrixGet();
	glm::mat4 view = glm::mat4(glm::mat3(viewMatrixGet()));
	glm::mat4 proj = projMatrixGet();
	glm::mat4 mvp = proj * view * model;

	shaderProgramUse(sdata->program);
	shaderProgramSet1i(sdata->program, "skybox", 0);
	shaderProgramSetMatrix4fv(sdata->program, "mvp", 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(sdata->vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sdata->texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	shaderProgramUse(0);

	glDepthMask(GL_TRUE);
}

