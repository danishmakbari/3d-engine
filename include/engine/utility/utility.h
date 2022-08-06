#ifndef UTILITY_H
#define UTILITY_H

#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX 0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/gl.h>

void openglErrorsHandle(void);

Uint32 frametime(void);
float fps(void);

float nv_gpu_memusage_mb(void);
float nv_gpu_memmax_mb(void);

const GLubyte *gpu_vendor(void);
const GLubyte *gpu_model(void);

const GLubyte *opengl_version(void);
const GLubyte *glsl_version(void);

#endif

