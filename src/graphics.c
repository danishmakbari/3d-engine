#include <engine/graphics/graphics.h>

void graphicsInit(float fovy, float near, float far)
{
	glewExperimental = 1;
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glViewport(0, 0, windowWidthGet(), windowHeightGet());

	projMatrixPerspective(fovy, (float) windowWidthGet() / windowHeightGet(), near, far);

	glClearColor(0, 0, 0, 1);
}

void drawScene(struct skyboxData *skybox)
{
	struct drawList *root = physicsDrawListGet();

	glViewport(0, 0, windowWidthGet(), windowHeightGet());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraUpdate();

	skyboxDraw(skybox);

	struct drawList *current = root;
	while (current->next) {
		current = current->next;
		switch (dGeomGetClass(current->pdata->geom)) {
		case dSphereClass:
			sphereDraw(current->pdata);
			break;
		case dBoxClass:
			boxDraw(current->pdata);
			break;
		case dCylinderClass:
			cylinderDraw(current->pdata);
			break;
		case dHeightfieldClass:
			terrainDraw(current->pdata);
			break;
		}
	}

}

static void dirMatricesCalculate(struct mvp *shadowMatrix, glm::vec3 lightVec, float near, float far)
{
	modelMatrixLoadIdentity(shadowMatrix);
	viewMatrixLoadIdentity(shadowMatrix);
	projMatrixLoadIdentity(shadowMatrix);

	glm::vec4 frustum[8] = {
		glm::vec4(-1, -1, -1,  1),
		glm::vec4( 1, -1, -1,  1),
		glm::vec4( 1,  1, -1,  1),
		glm::vec4(-1,  1, -1,  1),
		glm::vec4(-1, -1,  1,  1),
		glm::vec4( 1, -1,  1,  1),
		glm::vec4( 1,  1,  1,  1),
		glm::vec4(-1,  1,  1,  1)
	};

	glm::mat4 proj = glm::perspective(glm::radians(projMatrixPerspectiveFOVYGet()), (float) windowWidthGet() / windowHeightGet(), near, far);

	glm::mat4 view = viewMatrixGet();
	glm::mat4 inv_vp = glm::inverse(proj * view);
	glm::vec4 centroid = glm::vec4(0, 0, 0, 1);

	for (int i = 0; i < 8; i++) {
		frustum[i] = inv_vp * frustum[i];
		frustum[i] /= frustum[i].w;
		centroid += frustum[i];
	}

	centroid /= 8;

	viewMatrixLookAt(shadowMatrix, glm::vec3(glm::vec3(centroid) + glm::normalize(lightVec)), glm::vec3(centroid), glm::vec3(0, 1, 0));

	float minX = FLT_MAX;
	float maxX = -FLT_MAX;
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	float minZ = FLT_MAX;
	float maxZ = -FLT_MAX;

	glm::mat4 shadow_view = viewMatrixGet(shadowMatrix);
	for (int i = 0; i < 8; i++) {
		frustum[i] = shadow_view * frustum[i];
		minX = glm::min(minX, frustum[i].x);
		maxX = glm::max(maxX, frustum[i].x);
		minY = glm::min(minY, frustum[i].y);
		maxY = glm::max(maxY, frustum[i].y);
		minZ = glm::min(minZ, frustum[i].z);
		maxZ = glm::max(maxZ, frustum[i].z);
	}

	projMatrixOrtho(shadowMatrix, minX, maxX, minY, maxY, -maxZ, -minZ);
}

void drawShadows(void)
{
	struct drawList *root = physicsDrawListGet();
	struct lightList *light = physicsLightListGet();

	glEnable(GL_DEPTH_CLAMP);

	for (unsigned i = 0; i < lightListSizeGet(light); i++) {
		struct lightSettings *ldata = lightListElementGet(light, i);
		struct dirShadowMapData *dsmdata = ldata->dsmdata;
		struct pointShadowMapData *psmdata = ldata->psmdata;
		struct spotShadowMapData *ssmdata = ldata->ssmdata;

		if (ldata->lightType == DIRECTION_SHADOW_LIGHT) {
			glViewport(0, 0, dsmdata->size, dsmdata->size);

			glBindFramebuffer(GL_FRAMEBUFFER, dsmdata->fbo);
			glClear(GL_DEPTH_BUFFER_BIT);
			
			for (int j = 0; j < CSM_SPLITS; j++)
				dirMatricesCalculate(dsmdata->matrices + j, ldata->position, dsmdata->csmPlanes[j], dsmdata->csmPlanes[j + 1]);
			
			struct drawList *current = root;
			while (current->next) {
				current = current->next;
				switch (dGeomGetClass(current->pdata->geom)) {
				case dSphereClass:
					sphereDirShadowDraw(current->pdata, ldata, dsmdata->matrices);
					break;
				case dBoxClass:
					boxDirShadowDraw(current->pdata, ldata, dsmdata->matrices);
					break;
				case dCylinderClass:
					cylinderDirShadowDraw(current->pdata, ldata, dsmdata->matrices);
					break;
				case dHeightfieldClass:
					terrainDirShadowDraw(current->pdata, ldata, dsmdata->matrices);
					break;
				}
			}
		} else if (ldata->lightType == POINT_SHADOW_LIGHT) {
			glViewport(0, 0, psmdata->size, psmdata->size);
			glBindFramebuffer(GL_FRAMEBUFFER, psmdata->fbo);
			glClear(GL_DEPTH_BUFFER_BIT);

			struct mvp shadowMatrices[6];
			for (int i = 0; i < 6; i++) {
				modelMatrixLoadIdentity(shadowMatrices + i);
				viewMatrixLoadIdentity(shadowMatrices + i);
				projMatrixLoadIdentity(shadowMatrices + i);
				projMatrixPerspective(shadowMatrices + i, 90, 1, psmdata->near, psmdata->far);
			}
			viewMatrixLookAt(shadowMatrices,     glm::vec3(ldata->position), glm::vec3(ldata->position) + glm::vec3(1, 0, 0), glm::vec3(0, -1,  0));
			viewMatrixLookAt(shadowMatrices + 1, glm::vec3(ldata->position), glm::vec3(ldata->position) + glm::vec3(-1, 0, 0), glm::vec3(0, -1,  0));
			viewMatrixLookAt(shadowMatrices + 2, glm::vec3(ldata->position), glm::vec3(ldata->position) + glm::vec3(0, 1, 0), glm::vec3(0,  0,  1));
			viewMatrixLookAt(shadowMatrices + 3, glm::vec3(ldata->position), glm::vec3(ldata->position) + glm::vec3(0, -1, 0), glm::vec3(0,  0, -1));
			viewMatrixLookAt(shadowMatrices + 4, glm::vec3(ldata->position), glm::vec3(ldata->position) + glm::vec3(0, 0, -1), glm::vec3(0, -1,  0));
			viewMatrixLookAt(shadowMatrices + 5, glm::vec3(ldata->position), glm::vec3(ldata->position) + glm::vec3(0, 0, -1), glm::vec3(0, -1,  0));

			struct drawList *current = root;
			while (current->next) {
				current = current->next;
				switch (dGeomGetClass(current->pdata->geom)) {
				case dSphereClass:
					spherePointShadowDraw(current->pdata, ldata, shadowMatrices);
					break;
				case dBoxClass:
					boxPointShadowDraw(current->pdata, ldata, shadowMatrices);
					break;
				case dCylinderClass:
					cylinderPointShadowDraw(current->pdata, ldata, shadowMatrices);
					break;
				case dHeightfieldClass:
					terrainPointShadowDraw(current->pdata, ldata, shadowMatrices);
					break;
				}
			}
		} else if (ldata->lightType == SPOT_SHADOW_LIGHT) {
			glViewport(0, 0, ssmdata->size, ssmdata->size);

			glBindFramebuffer(GL_FRAMEBUFFER, ssmdata->fbo);
			glClear(GL_DEPTH_BUFFER_BIT);

			modelMatrixLoadIdentity(ssmdata->matrices);
			viewMatrixLoadIdentity(ssmdata->matrices);
			projMatrixLoadIdentity(ssmdata->matrices);

			viewMatrixLookAt(ssmdata->matrices, ldata->position + ldata->direction, ldata->position, glm::vec3(0, 1, 0));
			projMatrixPerspective(ssmdata->matrices, 90, 1, ssmdata->near, ssmdata->far);
			//projMatrixOrtho(ssmdata->matrices, -ssmdata->far, ssmdata->far, -ssmdata->far, ssmdata->far, ssmdata->far, -ssmdata->far);

			struct drawList *current = root;
			while (current->next) {
				current = current->next;
				switch (dGeomGetClass(current->pdata->geom)) {
				case dSphereClass:
					sphereSpotShadowDraw(current->pdata, ldata, ssmdata->matrices);
					break;
				case dBoxClass:
					boxSpotShadowDraw(current->pdata, ldata, ssmdata->matrices);
					break;
				case dCylinderClass:
					cylinderSpotShadowDraw(current->pdata, ldata, ssmdata->matrices);
					break;
				case dHeightfieldClass:
					terrainSpotShadowDraw(current->pdata, ldata, ssmdata->matrices);
					break;
				}
			}
	
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_CLAMP);
}

