#include <engine/mvp/mvp.h>

static glm::mat4 modelMatrix = glm::mat4(1);
static glm::mat4 viewMatrix = glm::mat4(1);
static glm::mat4 projMatrix = glm::mat4(1);

void modelMatrixSet(glm::mat4 matrix)
{
	modelMatrix = matrix;
}

void viewMatrixSet(glm::mat4 matrix)
{
	viewMatrix = matrix;
}

void projMatrixSet(glm::mat4 matrix)
{
	projMatrix = matrix;
}

glm::mat4 MVPMatrixGet(void)
{
	return projMatrix * viewMatrix * modelMatrix;
}

glm::mat4 MVMatrixGet(void)
{
	return viewMatrix * modelMatrix;
}

glm::mat4 VPMatrixGet(void)
{
	return projMatrix * viewMatrix;
}

glm::mat4 modelMatrixGet(void)
{
	return modelMatrix;
}

glm::mat3 normalMatrixGet(void)
{
	return glm::mat3(glm::transpose(glm::inverse(modelMatrix)));
}

glm::mat4 viewMatrixGet(void)
{
	return viewMatrix;
}

glm::mat4 projMatrixGet(void)
{
	return projMatrix;
}

void projMatrixLoadIdentity(void)
{
	projMatrix = glm::mat4(1);
}

void viewMatrixLoadIdentity(void)
{
	viewMatrix = glm::mat4(1);
}

void modelMatrixLoadIdentity(void)
{
	modelMatrix = glm::mat4(1);
}

void viewMatrixLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	viewMatrix = glm::lookAt(eye, center, up);
}

void viewMatrixScale(glm::vec3 s)
{
	viewMatrix = glm::scale(viewMatrix, s);
}

void modelMatrixScale(glm::vec3 s)
{
	modelMatrix = glm::scale(modelMatrix, s);	
}

void viewMatrixRotate(float angle, glm::vec3 axis)
{
	viewMatrix = glm::rotate(viewMatrix, (float) glm::radians(angle), axis);
}

void modelMatrixRotate(float angle, glm::vec3 axis)
{
	modelMatrix = glm::rotate(modelMatrix, (float) glm::radians(angle), axis);	
}

void viewMatrixTranslate(glm::vec3 t)
{
	viewMatrix = glm::translate(viewMatrix, t);
}

void modelMatrixTranslate(glm::vec3 t)
{
	modelMatrix = glm::translate(modelMatrix, t);	
}

void projMatrixPerspective(float fovy, float aspect, float near, float far)
{
	projMatrix = glm::perspective(glm::radians(fovy), aspect, near, far);
}

void projMatrixOrtho(float left, float right, float bottom, float top, float near, float far)
{
	projMatrix = glm::ortho(left, right, bottom, top, near, far);
}

glm::vec3 viewMatrixCameraPosGet(void)
{
	const float *view = glm::value_ptr(glm::inverse(viewMatrix));
	return glm::vec3(view[12], view[13], view[14]);
}

glm::vec3 viewMatrixUpVecGet(void)
{
	const float *view = glm::value_ptr(glm::inverse(viewMatrix));
	return glm::vec3(view[4], view[5], view[6]);		
}

glm::vec3 viewMatrixForwardVecGet(void)
{
	const float *view = glm::value_ptr(glm::inverse(viewMatrix));
	return glm::vec3(view[8], view[9], view[10]);
}

glm::vec3 viewMatrixRightVecGet(void)
{
	const float *view = glm::value_ptr(glm::inverse(viewMatrix));
	return glm::vec3(view[0], view[1], view[2]);
}

float projMatrixPerspectiveFOVYGet(void)
{
	const float *proj = glm::value_ptr(projMatrix);
	return glm::degrees(2 * glm::atan((1 / proj[5])));
}

float projMatrixPerspectiveNearGet(void)
{
	const float *proj = glm::value_ptr(projMatrix);
	return proj[14] / (proj[10] - 1);
}

float projMatrixPerspectiveFarGet(void)
{
	const float *proj = glm::value_ptr(projMatrix);
	return proj[14] / (proj[10] + 1);
}

void modelMatrixConvertToGL(const dReal *pos, const dReal *rot)
{
	float *mat = glm::value_ptr(modelMatrix);
	mat[0] = rot[0]; mat[4] = rot[1]; mat[8]  = rot[2];  mat[12] = pos[0];
	mat[1] = rot[4]; mat[5] = rot[5]; mat[9]  = rot[6];  mat[13] = pos[1];
	mat[2] = rot[8]; mat[6] = rot[9]; mat[10] = rot[10]; mat[14] = pos[2];
	mat[3] = 0;	 mat[7] = 0;	  mat[11] = 0;	     mat[15] = 1;
}

void modelMatrixSet(struct mvp *mvp, glm::mat4 matrix)
{
	mvp->modelMatrix = matrix;
}

void viewMatrixSet(struct mvp *mvp, glm::mat4 matrix)
{
	mvp->viewMatrix = matrix;
}

void projMatrixSet(struct mvp *mvp, glm::mat4 matrix)
{
	mvp->projMatrix = matrix;
}

glm::mat4 MVPMatrixGet(struct mvp *mvp)
{
	return mvp->projMatrix * mvp->viewMatrix * mvp->modelMatrix;
}

glm::mat4 MVMatrixGet(struct mvp *mvp)
{
	return mvp->viewMatrix * mvp->modelMatrix;
}

glm::mat4 VPMatrixGet(struct mvp *mvp)
{
	return mvp->projMatrix * mvp->viewMatrix;
}

glm::mat4 modelMatrixGet(struct mvp *mvp)
{
	return mvp->modelMatrix;
}

glm::mat3 normalMatrixGet(struct mvp *mvp)
{
	return glm::mat3(glm::transpose(glm::inverse(mvp->modelMatrix)));
}

glm::mat4 viewMatrixGet(struct mvp *mvp)
{
	return mvp->viewMatrix;
}

glm::mat4 projMatrixGet(struct mvp *mvp)
{
	return mvp->projMatrix;
}

void projMatrixLoadIdentity(struct mvp *mvp)
{
	mvp->projMatrix = glm::mat4(1);
}

void viewMatrixLoadIdentity(struct mvp *mvp)
{
	mvp->viewMatrix = glm::mat4(1);
}

void modelMatrixLoadIdentity(struct mvp *mvp)
{
	mvp->modelMatrix = glm::mat4(1);
}

void viewMatrixLookAt(struct mvp *mvp, glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	mvp->viewMatrix = glm::lookAt(eye, center, up);
}

void viewMatrixScale(struct mvp *mvp, glm::vec3 s)
{
	mvp->viewMatrix = glm::scale(mvp->viewMatrix, s);
}

void modelMatrixScale(struct mvp *mvp, glm::vec3 s)
{
	mvp->modelMatrix = glm::scale(mvp->modelMatrix, s);	
}

void viewMatrixRotate(struct mvp *mvp, float angle, glm::vec3 axis)
{
	mvp->viewMatrix = glm::rotate(mvp->viewMatrix, (float) glm::radians(angle), axis);
}

void modelMatrixRotate(struct mvp *mvp, float angle, glm::vec3 axis)
{
	mvp->modelMatrix = glm::rotate(mvp->modelMatrix, (float) glm::radians(angle), axis);	
}

void viewMatrixTranslate(struct mvp *mvp, glm::vec3 t)
{
	mvp->viewMatrix = glm::translate(mvp->viewMatrix, t);
}

void modelMatrixTranslate(struct mvp *mvp, glm::vec3 t)
{
	mvp->modelMatrix = glm::translate(mvp->modelMatrix, t);	
}

void projMatrixPerspective(struct mvp *mvp, float fovy, float aspect, float near, float far)
{
	mvp->projMatrix = glm::perspective(glm::radians(fovy), aspect, near, far);
}

void projMatrixOrtho(struct mvp *mvp, float left, float right, float bottom, float top, float near, float far)
{
	mvp->projMatrix = glm::ortho(left, right, bottom, top, near, far);
}

glm::vec3 viewMatrixCameraPosGet(struct mvp *mvp)
{
	const float *view = glm::value_ptr(glm::inverse(mvp->viewMatrix));
	return glm::vec3(view[12], view[13], view[14]);
}

glm::vec3 viewMatrixUpVecGet(struct mvp *mvp)
{
	const float *view = glm::value_ptr(glm::inverse(mvp->viewMatrix));
	return glm::vec3(view[4], view[5], view[6]);		
}

glm::vec3 viewMatrixForwardVecGet(struct mvp *mvp)
{
	const float *view = glm::value_ptr(glm::inverse(mvp->viewMatrix));
	return glm::vec3(view[8], view[9], view[10]);
}

float projMatrixPerspectiveFOVYGet(struct mvp *mvp)
{
	const float *proj = glm::value_ptr(mvp->projMatrix);
	return glm::degrees(2 * glm::atan((1 / proj[5])));
}

float projMatrixPerspectiveNearGet(struct mvp *mvp)
{
	const float *proj = glm::value_ptr(mvp->projMatrix);
	return proj[14] / (proj[10] - 1);
}

float projMatrixPerspectiveFarGet(struct mvp *mvp)
{
	const float *proj = glm::value_ptr(mvp->projMatrix);
	return proj[14] / (proj[10] + 1);
}

glm::vec3 viewMatrixRightVecGet(struct mvp *mvp)
{
	const float *view = glm::value_ptr(glm::inverse(mvp->viewMatrix));
	return glm::vec3(view[0], view[1], view[2]);
}

void modelMatrixConvertToGL(struct mvp *mvp, const dReal *pos, const dReal *rot)
{
	float *mat = glm::value_ptr(mvp->modelMatrix);
	mat[0] = rot[0]; mat[4] = rot[1]; mat[8]  = rot[2];  mat[12] = pos[0];
	mat[1] = rot[4]; mat[5] = rot[5]; mat[9]  = rot[6];  mat[13] = pos[1];
	mat[2] = rot[8]; mat[6] = rot[9]; mat[10] = rot[10]; mat[14] = pos[2];
	mat[3] = 0;	 mat[7] = 0;	  mat[11] = 0;	     mat[15] = 1;
}

