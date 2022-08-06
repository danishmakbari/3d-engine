#include <engine/utility/utility.h>

static Uint32 framestart = 0, framestop = 0;

void openglErrorsHandle(void) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "%d\n", err);
		exit(EXIT_FAILURE);
	}
}

Uint32 frametime(void)
{
	framestart = framestop;
	framestop = SDL_GetTicks();

	Uint32 frametime = framestop - framestart;

	return frametime;
}


static Uint32 framestart2 = 0, framestop2 = 0;

float fps(void)
{
	framestart2 = framestop2;
	framestop2 = SDL_GetTicks();

	Uint32 frametime2 = framestop2 - framestart2;
	float fps = 1.0 / (frametime2 / 1000.0);

	return fps;
}

float nv_gpu_memmax_mb(void)
{
	GLint memmax;
	glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &memmax);
	
	return memmax / 1000.0;
}

float nv_gpu_memusage_mb(void)
{
	GLint memaval;
	glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &memaval);

	return nv_gpu_memmax_mb() - memaval / 1000.0;
}

const GLubyte *gpu_vendor(void)
{
	return glGetString(GL_VENDOR);
}

const GLubyte *gpu_model(void)
{
	return glGetString(GL_RENDERER);
}

const GLubyte *opengl_version(void)
{
	return glGetString(GL_VERSION);
}

const GLubyte *glsl_version(void)
{
	return glGetString(GL_SHADING_LANGUAGE_VERSION);
}


