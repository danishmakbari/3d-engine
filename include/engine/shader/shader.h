#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <engine/light/light.h>
#include <engine/drawlist/drawlist.h>
#include <engine/shadow/shadow.h>
#include <engine/material/material.h>
#include <engine/mvp/mvp.h>

#define TEXTURE_2D 0
#define TEXTURE_CUBEMAP 1

void shaderProgramSetSpotShadowMatrices(GLuint program, struct mvp *matrices);
void shaderProgramSetPointShadowMatrices(GLuint program, struct mvp matrices[6]);
void shaderProgramSetDirShadowMatrices(GLuint program, struct mvp *matrices);

void shaderProgramSetMatrices(GLuint program);
void shaderProgramSetMaterials2D(GLuint program, struct materialList *material);
void shaderProgramSetMaterialsCube(GLuint program, struct materialList *material);
void shaderProgramSetLights(GLuint program, struct lightList *light);

GLuint shaderProgramCompileDefaultProgram(void);
GLuint shaderProgramCompileFromFiles(char *vshaderf, char *fshaderf, char *gshaderf);
GLuint shaderProgramCompile(char* vshadersrc, char* fshadersrc, char *gshadersrc);
void shaderProgramUse(GLuint program);
void shaderProgramDelete(GLuint program);

void shaderProgramSet1f(GLuint program, const char *name, GLfloat value);
void shaderProgramSet2f(GLuint program, const char *name, GLfloat value1, GLfloat value2);
void shaderProgramSet3f(GLuint program, const char *name, GLfloat value1, GLfloat value2, GLfloat value3);
void shaderProgramSet4f(GLuint program, const char *name, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);

void shaderProgramSet1i(GLuint program, const char *name, GLint value);
void shaderProgramSet2i(GLuint program, const char *name, GLint value1, GLint value2);
void shaderProgramSet3i(GLuint program, const char *name, GLint value1, GLint value2, GLint value3);
void shaderProgramSet4i(GLuint program, const char *name, GLint value1, GLint value2, GLint value3, GLint value4);

void shaderProgramSet1ui(GLuint program, const char *name, GLuint value);
void shaderProgramSet2ui(GLuint program, const char *name, GLuint value1, GLuint value2);
void shaderProgramSet3ui(GLuint program, const char *name, GLuint value1, GLuint value2, GLuint value3);
void shaderProgramSet4ui(GLuint program, const char *name, GLuint value1, GLuint value2, GLuint value3, GLuint value4);

void shaderProgramSet1fv(GLuint program, const char *name, GLsizei count, const GLfloat *value);
void shaderProgramSet2fv(GLuint program, const char *name, GLsizei count, const GLfloat *value);
void shaderProgramSet3fv(GLuint program, const char *name, GLsizei count, const GLfloat *value);
void shaderProgramSet4fv(GLuint program, const char *name, GLsizei count, const GLfloat *value);

void shaderProgramSet1iv(GLuint program, const char *name, GLsizei count, const GLint *value);
void shaderProgramSet2iv(GLuint program, const char *name, GLsizei count, const GLint *value);
void shaderProgramSet3iv(GLuint program, const char *name, GLsizei count, const GLint *value);
void shaderProgramSet4iv(GLuint program, const char *name, GLsizei count, const GLint *value);

void shaderProgramSet1uiv(GLuint program, const char *name, GLsizei count, const GLuint *value);
void shaderProgramSet2uiv(GLuint program, const char *name, GLsizei count, const GLuint *value);
void shaderProgramSet3uiv(GLuint program, const char *name, GLsizei count, const GLuint *value);
void shaderProgramSet4uiv(GLuint program, const char *name, GLsizei count, const GLuint *value);

void shaderProgramSetMatrix2fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
void shaderProgramSetMatrix3fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
void shaderProgramSetMatrix4fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);

void shaderProgramSetMatrix2x3fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
void shaderProgramSetMatrix3x2fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);

void shaderProgramSetMatrix2x4fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
void shaderProgramSetMatrix4x2fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);

void shaderProgramSetMatrix3x4fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
void shaderProgramSetMatrix4x3fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);

#endif

