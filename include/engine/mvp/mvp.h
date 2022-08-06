#ifndef MVP_H
#define MVP_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <ode/ode.h>

void modelMatrixSet(glm::mat4 matrix);
void viewMatrixSet(glm::mat4 matrix);
void projMatrixSet(glm::mat4 matrix);
glm::mat4 MVPMatrixGet(void);
glm::mat4 MVMatrixGet(void);
glm::mat4 VPMatrixGet(void);
glm::mat4 modelMatrixGet(void);
glm::mat3 normalMatrixGet(void);
glm::mat4 viewMatrixGet(void);
glm::mat4 projMatrixGet(void);
void projMatrixLoadIdentity(void);
void viewMatrixLoadIdentity(void);
void modelMatrixLoadIdentity(void);
void viewMatrixLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
void viewMatrixScale(glm::vec3 s);
void modelMatrixScale(glm::vec3 s);
void viewMatrixRotate(float angle, glm::vec3 axis);
void modelMatrixRotate(float angle, glm::vec3 axis);
void viewMatrixTranslate(glm::vec3 t);
void modelMatrixTranslate(glm::vec3 t);
void projMatrixPerspective(float fovy, float aspect, float near, float far);
void projMatrixOrtho(float left, float right, float bottom, float top, float near, float far);
glm::vec3 viewMatrixCameraPosGet(void);
glm::vec3 viewMatrixUpVecGet(void);
glm::vec3 viewMatrixForwardVecGet(void);
glm::vec3 viewMatrixRightVecGet(void);
float projMatrixPerspectiveFOVYGet(void);
float projMatrixPerspectiveNearGet(void);
float projMatrixPerspectiveFarGet(void);
void modelMatrixConvertToGL(const dReal *pos, const dReal *rot);

struct mvp {
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
};

void modelMatrixSet(struct mvp *mvp, glm::mat4 matrix);
void viewMatrixSet(struct mvp *mvp, glm::mat4 matrix);
void projMatrixSet(struct mvp *mvp, glm::mat4 matrix);
glm::mat4 MVPMatrixGet(struct mvp *mvp);
glm::mat4 MVMatrixGet(struct mvp *mvp);
glm::mat4 VPMatrixGet(struct mvp *mvp);
glm::mat4 modelMatrixGet(struct mvp *mvp);
glm::mat3 normalMatrixGet(struct mvp *mvp);
glm::mat4 viewMatrixGet(struct mvp *mvp);
glm::mat4 projMatrixGet(struct mvp *mvp);
void projMatrixLoadIdentity(struct mvp *mvp);
void viewMatrixLoadIdentity(struct mvp *mvp);
void modelMatrixLoadIdentity(struct mvp *mvp);
void viewMatrixLookAt(struct mvp *mvp, glm::vec3 eye, glm::vec3 center, glm::vec3 up);
void viewMatrixScale(struct mvp *mvp, glm::vec3 s);
void modelMatrixScale(struct mvp *mvp, glm::vec3 s);
void viewMatrixRotate(struct mvp *mvp, float angle, glm::vec3 axis);
void modelMatrixRotate(struct mvp *mvp, float angle, glm::vec3 axis);
void viewMatrixTranslate(struct mvp *mvp, glm::vec3 t);
void modelMatrixTranslate(struct mvp *mvp, glm::vec3 t);
void projMatrixPerspective(struct mvp *mvp, float fovy, float aspect, float near, float far);
void projMatrixOrtho(struct mvp *mvp, float left, float right, float bottom, float top, float near, float far);
glm::vec3 viewMatrixCameraPosGet(struct mvp *mvp);
glm::vec3 viewMatrixUpVecGet(struct mvp *mvp);
glm::vec3 viewMatrixForwardVecGet(struct mvp *mvp);
glm::vec3 viewMatrixRightVecGet(struct mvp *mvp);
float projMatrixPerspectiveFOVYGet(struct mvp *mvp);
float projMatrixPerspectiveNearGet(struct mvp *mvp);
float projMatrixPerspectiveFarGet(struct mvp *mvp);
void modelMatrixConvertToGL(struct mvp *mvp, const dReal *pos, const dReal *rot);

#endif

