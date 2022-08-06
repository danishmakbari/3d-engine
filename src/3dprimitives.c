#include <engine/3dprimitives/3dprimitives.h>

static int sphere_level = 5;
static int cylinder_level = 50;

static GLuint shaderProgram;
static int shaderProgramCounter = 0;

static GLuint sphere_vao;
static GLuint sphere_vbo;
static int sphereCounter = 0;

static GLuint box_vao;
static GLuint box_vbo;
static int boxCounter = 0;

static GLuint cylinder_vao;
static GLuint cylinder_vbo;
static int cylinderCounter = 0;

static GLuint terrain_vao;
static GLuint terrain_vbo;
static int terrainCounter = 0;

static glm::vec3 uv2_calculateTangentVector(glm::vec3 vertices[3], glm::vec2 uvs[3])
{
	glm::vec3 tangent;

	glm::vec3 edge1 = glm::normalize(vertices[2] - vertices[0]);
	glm::vec3 edge2 = glm::normalize(vertices[1] - vertices[0]);
	glm::vec2 deltaUV1 = glm::normalize(uvs[2] - uvs[0]);
	glm::vec2 deltaUV2 = glm::normalize(uvs[1] - uvs[0]);

	float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	return glm::normalize(tangent);
}

static glm::vec3 uv3_calculateTangentVector(glm::vec3 vertices[3], glm::vec3 _uvs[3])
{
	/* not working */

	glm::vec3 tangent;

	float max1 = -FLT_MAX;
	float max2 = -FLT_MAX;
	float max3 = -FLT_MAX;
	
	int i1, i2, i3;
	std::vector<float> v1, v2, v3;
	for (int i = 0; i < 3; i++) {
		v1.push_back(_uvs[0][i]);
		v2.push_back(_uvs[1][i]);
		v3.push_back(_uvs[2][i]);
	}

	for (int i = 0; i < 3; i++) {
		if (max1 < glm::abs(v1[i])) {
			max1 = glm::abs(v1[i]);
			i1 = i;
		}
		if (max2 < glm::abs(v2[i])) {
			max2 = glm::abs(v2[i]);
			i2 = i;
		}
		if (max3 < glm::abs(v3[i])) {
			max3 = glm::abs(v3[i]);
			i3 = i;
		}

	}

	v1.erase(v1.begin() + i1);
	v2.erase(v2.begin() + i2);
	v3.erase(v3.begin() + i3);

	glm::vec2 uvs[3];
	for (int i = 0; i < 2; i++) {
		uvs[0][i] = v1[i];
		uvs[1][i] = v2[i];
		uvs[2][i] = v3[i];
	}

	glm::vec3 edge1 = glm::normalize(vertices[2] - vertices[0]);
	glm::vec3 edge2 = glm::normalize(vertices[1] - vertices[0]);
	glm::vec2 deltaUV1 = glm::normalize(uvs[2] - uvs[0]);
	glm::vec2 deltaUV2 = glm::normalize(uvs[1] - uvs[0]);

	float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	return glm::normalize(tangent);
}

static glm::vec3 uv2_calculateBitangentVector(glm::vec3 vertices[3], glm::vec2 uvs[3])
{
	glm::vec3 bitangent;

	glm::vec3 edge1 = glm::normalize(vertices[2] - vertices[0]);
	glm::vec3 edge2 = glm::normalize(vertices[1] - vertices[0]);
	glm::vec2 deltaUV1 = glm::normalize(uvs[2] - uvs[0]);
	glm::vec2 deltaUV2 = glm::normalize(uvs[1] - uvs[0]);

	float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);	

	return glm::normalize(bitangent);
}

static glm::vec3 uv3_calculateBitangentVector(glm::vec3 vertices[3], glm::vec3 _uvs[3])
{
	/* not working */

	glm::vec3 bitangent;

	float max1 = -FLT_MAX;
	float max2 = -FLT_MAX;
	float max3 = -FLT_MAX;
	
	int i1, i2, i3;
	std::vector<float> v1, v2, v3;
	for (int i = 0; i < 3; i++) {
		v1.push_back(_uvs[0][i]);
		v2.push_back(_uvs[1][i]);
		v3.push_back(_uvs[2][i]);
	}

	for (int i = 0; i < 3; i++) {
		if (max1 < glm::abs(v1[i])) {
			max1 = glm::abs(v1[i]);
			i1 = i;
		}
		if (max2 < glm::abs(v2[i])) {
			max2 = glm::abs(v2[i]);
			i2 = i;
		}
		if (max3 < glm::abs(v3[i])) {
			max3 = glm::abs(v3[i]);
			i3 = i;
		}

	}

	v1.erase(v1.begin() + i1);
	v2.erase(v2.begin() + i2);
	v3.erase(v3.begin() + i3);

	glm::vec2 uvs[3];
	for (int i = 0; i < 2; i++) {
		uvs[0][i] = v1[i];
		uvs[1][i] = v2[i];
		uvs[2][i] = v3[i];
	}

	glm::vec3 edge1 = glm::normalize(vertices[2] - vertices[0]);
	glm::vec3 edge2 = glm::normalize(vertices[1] - vertices[0]);
	glm::vec2 deltaUV1 = glm::normalize(uvs[2] - uvs[0]);
	glm::vec2 deltaUV2 = glm::normalize(uvs[1] - uvs[0]);

	float f = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);	

	return glm::normalize(bitangent);
}

static GLfloat octahedron[][9] = {
	{ 0,    0,    1,
	  1,  0,    0,
	  0,    1,  0},

	{-1,  0,    0,
	  0,    0,  1,
	  0,    1,  0},

	{ 1,  0,    0,
	  0,    0,   -1,
	  0,    1,  0},

	{-1,  0,    0,
	  0,    1,  0,
	  0,    0,   -1},

	{ 0,    0,    1,
	  0,   -1,  0,
	  1,  0,    0},

	{-1,  0,    0,
	  0,   -1,  0,
	  0,    0,    1},

	{ 1,  0,    0,
	  0,    -1,  0,
	  0,     0,   -1},

	{-1,  0,    0,
	 0,     0,   -1,
	 0,    -1,  0}
};

static void subdivide(GLfloat *triangle, GLfloat *sphere, int depth, int *pos)
{
	if (depth == sphere_level) {
		for (int i = 0; i < 9; i += 3) {
			glm::vec3 point = glm::normalize(glm::vec3(triangle[i], triangle[i + 1], triangle[i + 2]));
			sphere[*pos + i] = point[0];
			sphere[*pos + i + 1] = point[1];
			sphere[*pos + i + 2] = point[2];
		}
		*pos += 9;
		return;
	}

	GLfloat triangle1[] = {
		(triangle[0] + triangle[3]) / 2, (triangle[1] + triangle[4]) / 2, (triangle[2] + triangle[5]) / 2,
		(triangle[0] + triangle[6]) / 2, (triangle[1] + triangle[7]) / 2, (triangle[2] + triangle[8]) / 2,
		triangle[0], triangle[1], triangle[2]
	};
	GLfloat triangle2[] = {
		(triangle[0] + triangle[6]) / 2, (triangle[1] + triangle[7]) / 2, (triangle[2] + triangle[8]) / 2,
		(triangle[3] + triangle[6]) / 2, (triangle[4] + triangle[7]) / 2, (triangle[5] + triangle[8]) / 2,
		triangle[6], triangle[7], triangle[8]
	};

	GLfloat triangle3[] = {
		(triangle[3] + triangle[6]) / 2, (triangle[4] + triangle[7]) / 2, (triangle[5] + triangle[8]) / 2,
		(triangle[0] + triangle[3]) / 2, (triangle[1] + triangle[4]) / 2, (triangle[2] + triangle[5]) / 2,
		triangle[3], triangle[4], triangle[5]
	};

	GLfloat triangle4[] = {
		(triangle[0] + triangle[3]) / 2, (triangle[1] + triangle[4]) / 2, (triangle[2] + triangle[5]) / 2,
		(triangle[3] + triangle[6]) / 2, (triangle[4] + triangle[7]) / 2, (triangle[5] + triangle[8]) / 2,
		(triangle[0] + triangle[6]) / 2, (triangle[1] + triangle[7]) / 2, (triangle[2] + triangle[8]) / 2
	};

	subdivide(triangle1, sphere, depth + 1, pos);
	subdivide(triangle2, sphere, depth + 1, pos);
	subdivide(triangle3, sphere, depth + 1, pos);
	subdivide(triangle4, sphere, depth + 1, pos);
}

static GLfloat *sphereGen()
{
	GLfloat *sphere = (GLfloat *) malloc(sizeof(GLfloat) * 8 * pow(4, sphere_level) * 3 * 3 * 3);
	int pos = 0;
	for (int i = 0; i < 8; i++)
		subdivide(octahedron[i], sphere, 0, &pos);
	
	for (int i = 0; i < 8 * pow(4, sphere_level); i++) {
		glm::vec3 triangle[3];
		triangle[0] = glm::vec3(sphere[i * 9], sphere[i * 9 + 1], sphere[i * 9 + 2]);
		triangle[1] = glm::vec3(sphere[i * 9 + 3], sphere[i * 9 + 4], sphere[i * 9 + 5]);
		triangle[2] = glm::vec3(sphere[i * 9 + 6], sphere[i * 9 + 7], sphere[i * 9 + 8]);
		glm::vec3 tangent = uv3_calculateTangentVector(triangle, triangle);
		glm::vec3 bitangent = uv3_calculateBitangentVector(triangle, triangle);
		int index = 8 * pow(4, sphere_level) * 3 * 3  + i * 18;
		sphere[index] = tangent.x;
		sphere[index + 1] = tangent.y;
		sphere[index + 2] = tangent.z;
		sphere[index + 3] = bitangent.x;
		sphere[index + 4] = bitangent.y;
		sphere[index + 5] = bitangent.z;
		sphere[index + 6] = tangent.x;
		sphere[index + 7] = tangent.y;
		sphere[index + 8] = tangent.z;
		sphere[index + 9] = bitangent.x;
		sphere[index + 10] = bitangent.y;
		sphere[index + 11] = bitangent.z;
		sphere[index + 12] = tangent.x;
		sphere[index + 13] = tangent.y;
		sphere[index + 14] = tangent.z;
		sphere[index + 15] = bitangent.x;
		sphere[index + 16] = bitangent.y;
		sphere[index + 17] = bitangent.z;
	}

	return sphere;
}

struct primitiveData *sphereCreate(struct materialList *material,
		dReal mass, dReal radius, glm::vec3 position, bool back_cullface, bool front_cullface)
{
	struct drawList *drawlist = physicsDrawListGet();
	struct physicsData *physics_data = physicsEngineDataGet();

	dBodyID body = dBodyCreate(physics_data->world);
	dBodySetPosition(body, position.x, position.y, position.z);
	dMass dmass;
	dMassSetSphereTotal(&dmass, mass, radius);
	dBodySetMass(body, &dmass);
	dGeomID sphere = dCreateSphere(physics_data->space, radius);
	dGeomSetBody(sphere, body);

	struct primitiveData *pdata = (struct primitiveData *) malloc(sizeof(struct primitiveData));
	pdata->geom = sphere;
	pdata->body = body;
	pdata->size = 8 * pow(4, sphere_level) * 3;
	pdata->material = material;
	pdata->back_cullface = back_cullface;
	pdata->front_cullface = front_cullface;

	if (!shaderProgramCounter)
		shaderProgram = shaderProgramCompileDefaultProgram();
	shaderProgramCounter++;
	pdata->program = shaderProgram;
	
	if (!sphereCounter) {
		GLfloat *sphereptr = sphereGen();

		glGenVertexArrays(1, &sphere_vao);
		glGenBuffers(1, &sphere_vbo);

		glBindVertexArray(sphere_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * pow(4, sphere_level) * 3 * 3 * 3, sphereptr, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3 * 2, (void *) (sizeof(GLfloat) * 8 * (int) pow(4, sphere_level) * 3 * 3));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3 * 2, (void *) (sizeof(GLfloat) * (8 * (int) pow(4, sphere_level) * 3 * 3 + 3)));
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		free(sphereptr);
	}
	sphereCounter++;
	pdata->vao = sphere_vao;
	pdata->vbo = sphere_vbo;

	drawListElementAdd(drawlist, pdata);

	return pdata;
}

void sphereDraw(struct primitiveData *pdata)
{
	if (pdata->back_cullface && pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else if (pdata->back_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else if (pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	dReal radius = dGeomSphereGetRadius(pdata->geom);

	modelMatrixLoadIdentity();
	modelMatrixConvertToGL(dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(glm::vec3(radius, radius, radius));

	glBindVertexArray(pdata->vao);

	shaderProgramUse(pdata->program);

	shaderProgramSetMatrices(pdata->program);
	shaderProgramSetMaterialsCube(pdata->program, pdata->material);
	shaderProgramSetLights(pdata->program, physicsLightListGet());

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);
	glBindVertexArray(0);

	if (pdata->back_cullface || pdata->front_cullface) {
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}
}

void sphereDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct dirShadowMapData *smdata = ldata->dsmdata;

	dReal radius = dGeomSphereGetRadius(pdata->geom);

	glBindVertexArray(pdata->vao);
	
	shaderProgramUse(smdata->program);

	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(radius, radius, radius));

	shaderProgramSetDirShadowMatrices(smdata->program, matrices);

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void spherePointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6])
{
	struct pointShadowMapData *smdata = ldata->psmdata;

	dReal radius = dGeomSphereGetRadius(pdata->geom);

	glBindVertexArray(pdata->vao);
	
	shaderProgramUse(smdata->program);

	for (int i = 0; i < 6; i++) {
		modelMatrixLoadIdentity(matrices + i);
		modelMatrixConvertToGL(matrices + i, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
		modelMatrixScale(matrices + i, glm::vec3(radius, radius, radius));
	}
	
	shaderProgramSetPointShadowMatrices(smdata->program, matrices);
	shaderProgramSet1f(smdata->program, "near", smdata->near);
	shaderProgramSet1f(smdata->program, "far", smdata->far);
	shaderProgramSet4fv(smdata->program, "lightPos", 1, glm::value_ptr(ldata->position));

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void sphereSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct spotShadowMapData *smdata = ldata->ssmdata;

	dReal radius = dGeomSphereGetRadius(pdata->geom);

	glBindVertexArray(pdata->vao);
	
	shaderProgramUse(smdata->program);

	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(radius, radius, radius));

	shaderProgramSetSpotShadowMatrices(smdata->program, matrices);

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void sphereDestroy(struct primitiveData *pdata)
{
	dBodyDestroy(pdata->body);
	dGeomDestroy(pdata->geom);
	
	sphereCounter--;
	if (!sphereCounter) {
		glDeleteBuffers(1, &pdata->vbo);
		glDeleteVertexArrays(1, &pdata->vao);
	}

	shaderProgramCounter--;
	if (!shaderProgramCounter)
		shaderProgramDelete(pdata->program);

	free(pdata);
}

static const GLfloat box_vertices[] = {
	-0.5, -0.5, -0.5,  0.0,  0.0, -1.0, -1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	 0.5,  0.5, -0.5,  0.0,  0.0, -1.0, -1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	 0.5, -0.5, -0.5,  0.0,  0.0, -1.0, -1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	-0.5,  0.5, -0.5,  0.0,  0.0, -1.0, -1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	 0.5,  0.5, -0.5,  0.0,  0.0, -1.0, -1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	-0.5, -0.5, -0.5,  0.0,  0.0, -1.0, -1.0,  0.0,  0.0,  0.0, -1.0,  0.0,

	-0.5, -0.5,  0.5,  0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	 0.5, -0.5,  0.5,  0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	 0.5,  0.5,  0.5,  0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	 0.5,  0.5,  0.5,  0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	-0.5,  0.5,  0.5,  0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0, -1.0,  0.0,
	-0.5, -0.5,  0.5,  0.0,  0.0,  1.0,  1.0,  0.0,  0.0,  0.0, -1.0,  0.0,

	-0.5,  0.5,  0.5, -1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0, -1.0,  0.0,
	-0.5,  0.5, -0.5, -1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0, -1.0,  0.0,
	-0.5, -0.5, -0.5, -1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0, -1.0,  0.0,
	-0.5, -0.5, -0.5, -1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0, -1.0,  0.0,
	-0.5, -0.5,  0.5, -1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0, -1.0,  0.0,
	-0.5,  0.5,  0.5, -1.0,  0.0,  0.0,  0.0,  0.0,  1.0,  0.0, -1.0,  0.0,

	 0.5,  0.5, -0.5,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,  0.0, -1.0,  0.0,
	 0.5,  0.5,  0.5,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,  0.0, -1.0,  0.0,
	 0.5, -0.5, -0.5,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,  0.0, -1.0,  0.0,
	 0.5, -0.5,  0.5,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,  0.0, -1.0,  0.0,
	 0.5, -0.5, -0.5,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,  0.0, -1.0,  0.0,
	 0.5,  0.5,  0.5,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,  0.0, -1.0,  0.0,

	-0.5, -0.5, -0.5,  0.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,
	 0.5, -0.5, -0.5,  0.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,
	 0.5, -0.5,  0.5,  0.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,
	 0.5, -0.5,  0.5,  0.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,
	-0.5, -0.5,  0.5,  0.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,
	-0.5, -0.5, -0.5,  0.0, -1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0, -1.0,

	 0.5,  0.5, -0.5,  0.0,  1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  1.0,
	-0.5,  0.5, -0.5,  0.0,  1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  1.0,
	 0.5,  0.5,  0.5,  0.0,  1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  1.0,
	-0.5,  0.5,  0.5,  0.0,  1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  1.0,
	 0.5,  0.5,  0.5,  0.0,  1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  1.0,
	-0.5,  0.5, -0.5,  0.0,  1.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  1.0
};

struct primitiveData *boxCreate(struct materialList *material,
		dReal mass, glm::vec3 scale, glm::vec3 position, bool back_cullface, bool front_cullface)
{
	struct drawList *drawlist = physicsDrawListGet();
	struct physicsData *physics_data = physicsEngineDataGet();

	dBodyID body = dBodyCreate(physics_data->world);
	dBodySetPosition(body, position.x, position.y, position.z);
	dMass dmass;
	dMassSetBoxTotal(&dmass, mass, scale.x, scale.y, scale.z);
	dBodySetMass(body, &dmass);

	dGeomID box = dCreateBox(physics_data->space, scale.x, scale.y, scale.z);
	dGeomSetBody(box, body);

	struct primitiveData *pdata = (struct primitiveData *) malloc(sizeof(struct primitiveData));
	pdata->geom = box;
	pdata->body = body;
	pdata->size = 36;
	pdata->material = material;
	pdata->back_cullface = back_cullface;
	pdata->front_cullface = front_cullface;

	if (!shaderProgramCounter)
		shaderProgram = shaderProgramCompileDefaultProgram();
	pdata->program = shaderProgram;
	shaderProgramCounter++;

	if (!boxCounter) {
		glGenVertexArrays(1, &box_vao);
		glGenBuffers(1, &box_vbo);

		glBindVertexArray(box_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, box_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *) (sizeof(GLfloat) * 3));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *) 0);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *) (sizeof(GLfloat) * 6));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *) (sizeof(GLfloat) * 9));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	boxCounter++;
	pdata->vao = box_vao;
	pdata->vbo = box_vbo;


	drawListElementAdd(drawlist, pdata);

	return pdata;
}

void boxDraw(struct primitiveData *pdata)
{
	if (pdata->back_cullface && pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else if (pdata->back_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else if (pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	dVector3 lengths;
	dGeomBoxGetLengths(pdata->geom, lengths);

	modelMatrixLoadIdentity();
	modelMatrixConvertToGL(dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(glm::vec3(lengths[0], lengths[1], lengths[2]));

	glBindVertexArray(pdata->vao);

	shaderProgramUse(pdata->program);

	shaderProgramSetMatrices(pdata->program);
	shaderProgramSetMaterialsCube(pdata->program, pdata->material);
	shaderProgramSetLights(pdata->program, physicsLightListGet());
	
	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);
	glBindVertexArray(0);

	if (pdata->back_cullface || pdata->front_cullface) {
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}
}

void boxDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct dirShadowMapData *smdata = ldata->dsmdata;

	dVector3 lengths;
	dGeomBoxGetLengths(pdata->geom, lengths);

	glBindVertexArray(pdata->vao);

	shaderProgramUse(smdata->program);
				
	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(lengths[0], lengths[1], lengths[2]));
	
	shaderProgramSetDirShadowMatrices(smdata->program, matrices);

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);

}

void boxPointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6])
{
	struct pointShadowMapData *smdata = ldata->psmdata;

	dVector3 lengths;
	dGeomBoxGetLengths(pdata->geom, lengths);

	glBindVertexArray(pdata->vao);
	
	shaderProgramUse(smdata->program);

	for (int i = 0; i < 6; i++) {
		modelMatrixLoadIdentity(matrices + i);
		modelMatrixConvertToGL(matrices + i, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
		modelMatrixScale(matrices + i, glm::vec3(lengths[0], lengths[1], lengths[2]));
	}
	
	shaderProgramSetPointShadowMatrices(smdata->program, matrices);
	shaderProgramSet1f(smdata->program, "near", smdata->near);
	shaderProgramSet1f(smdata->program, "far", smdata->far);
	shaderProgramSet4fv(smdata->program, "lightPos", 1, glm::value_ptr(ldata->position));

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void boxSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct spotShadowMapData *smdata = ldata->ssmdata;

	dVector3 lengths;
	dGeomBoxGetLengths(pdata->geom, lengths);

	glBindVertexArray(pdata->vao);

	shaderProgramUse(smdata->program);
				
	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(lengths[0], lengths[1], lengths[2]));
	
	shaderProgramSetSpotShadowMatrices(smdata->program, matrices);

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void boxDestroy(struct primitiveData *pdata)
{
	dBodyDestroy(pdata->body);
	dGeomDestroy(pdata->geom);

	boxCounter--;
	if (!boxCounter) {
		glDeleteBuffers(1, &pdata->vbo);
		glDeleteVertexArrays(1, &pdata->vao);
	}

	shaderProgramCounter--;
	if (!shaderProgramCounter)
		shaderProgramDelete(pdata->program);

	free(pdata);
}

static GLfloat *cylinderGen()
{
	const GLfloat pi = glm::pi<GLfloat>();
	GLfloat *vertices = (GLfloat *) malloc(sizeof(GLfloat) * (12 * (cylinder_level + 2) + 6 * (cylinder_level + 1)));

	GLfloat rads = 0;
	/* vertices */
	vertices[0] = 0;
	vertices[1] = 0;
	vertices[2] = -0.5;
	vertices[3] = 0;
	vertices[4] = 0;
	vertices[5] = 0.5;
	for (int i = 6; i < 6 * (cylinder_level + 2); i += 6, rads += 2 * pi / cylinder_level) {
		/* vertices */
		vertices[i] = 0.5 * sin(rads);
		vertices[i + 1] = 0.5 * cos(rads);
		vertices[i + 2] = -0.5;
		vertices[i + 3] = vertices[i];
		vertices[i + 4] = vertices[i + 1];
		vertices[i + 5] = 0.5;
	}

	for (int i = 6 * (cylinder_level + 2); i < 12 * (cylinder_level + 2); i += 6, rads += 2 * pi / cylinder_level) {
		/* normals */
		vertices[i] = 0;
		vertices[i + 1] = 0;
		vertices[i + 2] = -1;
		vertices[i + 3] = 0;
		vertices[i + 4] = 0;
		vertices[i + 5] = 1;
	}

	rads = 0;
	for (int i = 12 * (cylinder_level + 2); i < 12 * (cylinder_level + 2) + 6 * (cylinder_level + 1); i += 6, rads += 2 * pi / cylinder_level) {
		/* normals */
		vertices[i] = 0.5 * sin(rads);;
		vertices[i + 1] = 0.5 * cos(rads);
		vertices[i + 2] = 0;
		vertices[i + 3] = vertices[i];
		vertices[i + 4] = vertices[i + 1];
		vertices[i + 5] = 0;
	}
		

	return vertices;
}

struct primitiveData *cylinderCreate(struct materialList *material,
		dReal mass, dReal radius, dReal height, glm::vec3 position, bool back_cullface, bool front_cullface)
{
	struct drawList *drawlist = physicsDrawListGet();
	struct physicsData *physics_data = physicsEngineDataGet();

	dBodyID body = dBodyCreate(physics_data->world);
	dBodySetPosition(body, position.x, position.y, position.z);
	dMass dmass;
	dMassSetCylinderTotal(&dmass, mass, 1, radius, height);
	dBodySetMass(body, &dmass);

	dGeomID cylinder = dCreateCylinder(physics_data->space, radius, height);
	dGeomSetBody(cylinder, body);

	struct primitiveData *pdata = (struct primitiveData *) malloc(sizeof(struct primitiveData));
	pdata->geom = cylinder;
	pdata->body = body;
	pdata->size = -1;
	pdata->material = material;
	pdata->back_cullface = back_cullface;
	pdata->front_cullface = front_cullface;

	if (!shaderProgramCounter)
		shaderProgram = shaderProgramCompileDefaultProgram();
	pdata->program = shaderProgram;
	shaderProgramCounter++;

	if (!cylinderCounter) {
		GLfloat *cylinderptr = cylinderGen();

		glGenVertexArrays(1, &cylinder_vao);
		glGenBuffers(1, &cylinder_vbo);

		glBindVertexArray(cylinder_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, cylinder_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (12 * (cylinder_level + 2) + 6 * (cylinder_level + 1)), cylinderptr, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		free(cylinderptr);
	}
	cylinderCounter++;
	pdata->vao = cylinder_vao;
	pdata->vbo = cylinder_vbo;

	drawListElementAdd(drawlist, pdata);

	return pdata;
}

void cylinderDraw(struct primitiveData *pdata)
{
	if (pdata->back_cullface && pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else if (pdata->back_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else if (pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	dReal radius, height;
	dGeomCylinderGetParams(pdata->geom, &radius, &height);

	modelMatrixLoadIdentity();
	modelMatrixConvertToGL(dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(glm::vec3(2 * radius, 2 * radius, height));

	glBindVertexArray(pdata->vao);

	shaderProgramUse(pdata->program);

	shaderProgramSetMatrices(pdata->program);
	shaderProgramSetMaterials2D(pdata->program, pdata->material);
	shaderProgramSetLights(pdata->program, physicsLightListGet());

	glBindBuffer(GL_ARRAY_BUFFER, pdata->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) ((6 * (cylinder_level + 2)) * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);

	glFrontFace(GL_CW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) ((6 * (cylinder_level + 2) + 3) * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);
	glFrontFace(GL_CCW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) (6 * sizeof(GLfloat)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) (12 * (cylinder_level + 2) * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (cylinder_level + 1));

	shaderProgramUse(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (pdata->back_cullface || pdata->front_cullface) {
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}
}

void cylinderDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct dirShadowMapData *smdata = ldata->dsmdata;

	dReal radius, height;
	dGeomCylinderGetParams(pdata->geom, &radius, &height);

	glBindVertexArray(pdata->vao);

	shaderProgramUse(smdata->program);

	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(2 * radius, 2 * radius, height));
		
	shaderProgramSetDirShadowMatrices(smdata->program, matrices);

	glBindBuffer(GL_ARRAY_BUFFER, pdata->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);

	glFrontFace(GL_CW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) (3 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);
	glFrontFace(GL_CCW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) (6 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (cylinder_level + 1));

	shaderProgramUse(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cylinderPointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6])
{
	struct pointShadowMapData *smdata = ldata->psmdata;

	dReal radius, height;
	dGeomCylinderGetParams(pdata->geom, &radius, &height);
	
	glBindVertexArray(pdata->vao);

	shaderProgramUse(smdata->program);

	for (int i = 0; i < 6; i++) {
		modelMatrixLoadIdentity(matrices + i);
		modelMatrixConvertToGL(matrices + i, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
		modelMatrixScale(matrices + i, glm::vec3(2 * radius, 2 * radius, height));
	}
	
	shaderProgramSetPointShadowMatrices(smdata->program, matrices);
	shaderProgramSet1f(smdata->program, "near", smdata->near);
	shaderProgramSet1f(smdata->program, "far", smdata->far);
	shaderProgramSet4fv(smdata->program, "lightPos", 1, glm::value_ptr(ldata->position));

	glBindBuffer(GL_ARRAY_BUFFER, pdata->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);

	glFrontFace(GL_CW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) (3 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);
	glFrontFace(GL_CCW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) (6 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (cylinder_level + 1));

	shaderProgramUse(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cylinderSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct spotShadowMapData *smdata = ldata->ssmdata;

	dReal radius, height;
	dGeomCylinderGetParams(pdata->geom, &radius, &height);

	glBindVertexArray(pdata->vao);

	shaderProgramUse(smdata->program);

	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(2 * radius, 2 * radius, height));
		
	shaderProgramSetSpotShadowMatrices(smdata->program, matrices);

	glBindBuffer(GL_ARRAY_BUFFER, pdata->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);

	glFrontFace(GL_CW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void *) (3 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinder_level + 2);
	glFrontFace(GL_CCW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) (6 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (cylinder_level + 1));

	shaderProgramUse(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cylinderDestroy(struct primitiveData *pdata)
{
	dBodyDestroy(pdata->body);
	dGeomDestroy(pdata->geom);

	cylinderCounter--;
	if (!cylinderCounter) {
		glDeleteBuffers(1, &pdata->vbo);
		glDeleteVertexArrays(1, &pdata->vao);
	}

	shaderProgramCounter--;
	if (!shaderProgramCounter)
		shaderProgramDelete(pdata->program);
	
	free(pdata);
}

static Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
	case 2:
		return *(Uint16 *)p;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *(Uint32 *)p;
	}

	return 0;
}


struct primitiveData *terrainCreate(struct materialList *material, const char *terrainMap,
	glm::vec3 scale, bool back_cullface, bool front_cullface)
{
	struct drawList *drawlist = physicsDrawListGet();
	struct physicsData *physics_data = physicsEngineDataGet();

	SDL_Surface *surface = IMG_Load(terrainMap);

	int width = surface->w, height = surface->h;
	
	struct primitiveData *pdata = (struct primitiveData *) malloc(sizeof(struct primitiveData));
	pdata->size = (height - 1) * (width - 1) * 6;
	pdata->material = material;
	pdata->back_cullface = back_cullface;
	pdata->front_cullface = front_cullface;

	GLfloat *heightmap = (GLfloat *) malloc(sizeof(GLfloat) * height * width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			SDL_Color rgb;
			Uint32 data = getpixel(surface, j, i);
			SDL_GetRGB(data, surface->format, &rgb.r, &rgb.g, &rgb.b);
			heightmap[i * width + j] = rgb.r - 128;
		}
	}
	SDL_FreeSurface(surface);
	
	dHeightfieldDataID dataid = dGeomHeightfieldDataCreate();
	dGeomHeightfieldDataBuildSingle(dataid, heightmap, 1, (height - 1) * scale.x, (width - 1) * scale.z, height, width, scale.y, 0, 1, 0);
	dGeomHeightfieldDataSetBounds(dataid, -128 * scale.y, 127 * scale.y);
	dGeomID geom = dCreateHeightfield(physics_data->space, dataid, 1);
	pdata->geom = geom;
	pdata->body = dGeomGetBody(geom);

	drawListElementAdd(drawlist, pdata);

	GLfloat *vertexmap = (GLfloat *) malloc(sizeof(GLfloat) * (height - 1) * (width - 1) * 18 * 5);
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			int index = 18 * (i * (width - 1) + j);
			vertexmap[index] = (i - (height - 1) / 2.0) * scale.x;
			vertexmap[index + 1] = heightmap[i * width + j] * scale.y;
			vertexmap[index + 2] = (j - (width - 1) / 2.0) * scale.z;

			vertexmap[index + 3] = (i + 1 - (height - 1) / 2.0) * scale.x;
			vertexmap[index + 4] = heightmap[(i + 1) * width + j] * scale.y;
			vertexmap[index + 5] = (j - (width - 1) / 2.0) * scale.z;

			vertexmap[index + 6] = (i - (height - 1) / 2.0) * scale.x;
			vertexmap[index + 7] = heightmap[i * width + j + 1] * scale.y;
			vertexmap[index + 8] = (j + 1 - (width - 1) / 2.0) * scale.z;

			vertexmap[index + 9] = (i + 1 - (height - 1) / 2.0) * scale.x;
			vertexmap[index + 10] = heightmap[(i + 1) * width + j + 1] * scale.y;
			vertexmap[index + 11] = (j + 1 - (width - 1) / 2.0) * scale.z;

			vertexmap[index + 12] = (i - (height - 1) / 2.0) * scale.x;
			vertexmap[index + 13] = heightmap[i * width + j + 1] * scale.y;
			vertexmap[index + 14] = (j + 1 - (width - 1) / 2.0) * scale.z;

			vertexmap[index + 15] = (i + 1 - (height - 1) / 2.0) * scale.x;
			vertexmap[index + 16] = heightmap[(i + 1) * width + j] * scale.y;
			vertexmap[index + 17] = (j - (width - 1) / 2.0) * scale.z;

			glm::vec3 point1 = glm::vec3(vertexmap[index], vertexmap[index + 1], vertexmap[index + 2]);
			glm::vec3 point2 = glm::vec3(vertexmap[index + 3], vertexmap[index + 4], vertexmap[index + 5]);
			glm::vec3 point3 = glm::vec3(vertexmap[index + 6], vertexmap[index + 7], vertexmap[index + 8]);
			glm::vec3 point4 = glm::vec3(vertexmap[index + 9], vertexmap[index + 10], vertexmap[index + 11]);

			glm::vec3 normal1 = glm::cross(point1 - point2, point3 - point1);
			glm::vec3 normal2 = glm::cross(point2 - point4, point3 - point4);

			int index1 = index + (height - 1) * (width - 1) * 18;
			vertexmap[index1] = normal1[0];
			vertexmap[index1 + 1] = normal1[1];
			vertexmap[index1 + 2] = normal1[2];

			vertexmap[index1 + 3] = normal1[0];
			vertexmap[index1 + 4] = normal1[1];
			vertexmap[index1 + 5] = normal1[2];

			vertexmap[index1 + 6] = normal1[0];
			vertexmap[index1 + 7] = normal1[1];
			vertexmap[index1 + 8] = normal1[2];

			vertexmap[index1 + 9] = normal2[0];
			vertexmap[index1 + 10] = normal2[1];
			vertexmap[index1 + 11] = normal2[2];

			vertexmap[index1 + 12] = normal2[0];
			vertexmap[index1 + 13] = normal2[1];
			vertexmap[index1 + 14] = normal2[2];

			vertexmap[index1 + 15] = normal2[0];
			vertexmap[index1 + 16] = normal2[1];
			vertexmap[index1 + 17] = normal2[2];

			int index2 = index1 + (height - 1) * (width - 1) * 18;
			vertexmap[index2] = 0;
			vertexmap[index2 + 1] = 0;
			vertexmap[index2 + 2] = 0;

			vertexmap[index2 + 3] = 0;
			vertexmap[index2 + 4] = 1;
			vertexmap[index2 + 5] = 0;

			vertexmap[index2 + 6] = 1;
			vertexmap[index2 + 7] = 0;
			vertexmap[index2 + 8] = 0;

			vertexmap[index2 + 9] = 1;
			vertexmap[index2 + 10] = 1;
			vertexmap[index2 + 11] = 0;

			vertexmap[index2 + 12] = 1;
			vertexmap[index2 + 13] = 0;
			vertexmap[index2 + 14] = 0;

			vertexmap[index2 + 15] = 0;
			vertexmap[index2 + 16] = 1;
			vertexmap[index2 + 17] = 0;
			
			int index3 = index2 + (height - 1) * (width - 1) * 18;
			glm::vec3 triangle1[3], triangle2[3];
			glm::vec2 uvs1[3], uvs2[3];

			triangle1[0].x = vertexmap[index];
			triangle1[0].y = vertexmap[index + 1];
			triangle1[0].z = vertexmap[index + 2];
			triangle1[1].x = vertexmap[index + 3];
			triangle1[1].y = vertexmap[index + 4];
			triangle1[1].z = vertexmap[index + 5];
			triangle1[2].x = vertexmap[index + 6];
			triangle1[2].y = vertexmap[index + 7];
			triangle1[2].z = vertexmap[index + 8];
			triangle2[0].x = vertexmap[index + 9];
			triangle2[0].y = vertexmap[index + 10];
			triangle2[0].z = vertexmap[index + 11];
			triangle2[1].x = vertexmap[index + 12];
			triangle2[1].y = vertexmap[index + 13];
			triangle2[1].z = vertexmap[index + 14];
			triangle2[2].x = vertexmap[index + 15];
			triangle2[2].y = vertexmap[index + 16];
			triangle2[2].z = vertexmap[index + 17];

			uvs1[0].x = vertexmap[index2];
			uvs1[0].y = vertexmap[index2 + 1];
			uvs1[1].x = vertexmap[index2 + 3];
			uvs1[1].y = vertexmap[index2 + 4];
			uvs1[2].x = vertexmap[index2 + 6];
			uvs1[2].y = vertexmap[index2 + 7];
			uvs2[0].x = vertexmap[index2 + 9];
			uvs2[0].y = vertexmap[index2 + 10];
			uvs2[1].x = vertexmap[index2 + 12];
			uvs2[1].y = vertexmap[index2 + 13];
			uvs2[2].x = vertexmap[index2 + 15];
			uvs2[2].y = vertexmap[index2 + 16];

			glm::vec3 tangent1 = uv2_calculateTangentVector(triangle1, uvs1);
			glm::vec3 tangent2 = uv2_calculateTangentVector(triangle2, uvs2);

			vertexmap[index3] = tangent1.x;
			vertexmap[index3 + 1] = tangent1.y;
			vertexmap[index3 + 2] = tangent1.z;

			vertexmap[index3 + 3] = tangent1.x;
			vertexmap[index3 + 4] = tangent1.y;
			vertexmap[index3 + 5] = tangent1.z;

			vertexmap[index3 + 6] = tangent1.x;
			vertexmap[index3 + 7] = tangent1.y;
			vertexmap[index3 + 8] = tangent1.z;

			vertexmap[index3 + 9] = tangent2.x;
			vertexmap[index3 + 10] = tangent2.y;
			vertexmap[index3 + 11] = tangent2.z;

			vertexmap[index3 + 12] = tangent2.x;
			vertexmap[index3 + 13] = tangent2.y;
			vertexmap[index3 + 14] = tangent2.z;

			vertexmap[index3 + 15] = tangent2.x;
			vertexmap[index3 + 16] = tangent2.y;
			vertexmap[index3 + 17] = tangent2.z;

			int index4 = index3 + (height - 1) * (width - 1) * 18;
			glm::vec3 bitangent1 = uv2_calculateBitangentVector(triangle1, uvs1);
			glm::vec3 bitangent2 = uv2_calculateBitangentVector(triangle2, uvs2);
		
			vertexmap[index4] = bitangent1.x;
			vertexmap[index4 + 1] = bitangent1.y;
			vertexmap[index4 + 2] = bitangent1.z;

			vertexmap[index4 + 3] = bitangent1.x;
			vertexmap[index4 + 4] = bitangent1.y;
			vertexmap[index4 + 5] = bitangent1.z;

			vertexmap[index4 + 6] = bitangent1.x;
			vertexmap[index4 + 7] = bitangent1.y;
			vertexmap[index4 + 8] = bitangent1.z;

			vertexmap[index4 + 9] = bitangent2.x;
			vertexmap[index4 + 10] = bitangent2.y;
			vertexmap[index4 + 11] = bitangent2.z;

			vertexmap[index4 + 12] = bitangent2.x;
			vertexmap[index4 + 13] = bitangent2.y;
			vertexmap[index4 + 14] = bitangent2.z;

			vertexmap[index4 + 15] = bitangent2.x;
			vertexmap[index4 + 16] = bitangent2.y;
			vertexmap[index4 + 17] = bitangent2.z;
		}
	}
	free(heightmap);

	if (!terrainCounter) {
		glGenVertexArrays(1, &terrain_vao);
		glGenBuffers(1, &terrain_vbo);

		glBindVertexArray(terrain_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, terrain_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (height - 1) * (width - 1) * 18 * 5, vertexmap, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(GLfloat) * (height - 1) * (width - 1) * 18));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(GLfloat) * (height - 1) * (width - 1) * 18 * 2));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(GLfloat) * (height - 1) * (width - 1) * 18 * 3));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(GLfloat) * (height - 1) * (width - 1) * 18 * 4));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	terrainCounter++;
	pdata->vao = terrain_vao;
	pdata->vbo = terrain_vbo;

	if (!shaderProgramCounter)
		shaderProgram = shaderProgramCompileDefaultProgram();
	shaderProgramCounter++;
	pdata->program = shaderProgram;

	free(vertexmap);

	return pdata;
}

void terrainDraw(struct primitiveData *pdata)
{
	if (pdata->back_cullface && pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else if (pdata->back_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else if (pdata->front_cullface) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	modelMatrixLoadIdentity();
	modelMatrixConvertToGL(dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(glm::vec3(-1, 1, 1));
	modelMatrixRotate(-90, glm::vec3(0, 1, 0));

	glBindVertexArray(pdata->vao);

	shaderProgramUse(pdata->program);
	shaderProgramSetMatrices(pdata->program);
	shaderProgramSetMaterials2D(pdata->program, pdata->material);
	shaderProgramSetLights(pdata->program, physicsLightListGet());

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);
	glBindVertexArray(0);

	if (pdata->back_cullface || pdata->front_cullface) {
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	}
}

void terrainDirShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct dirShadowMapData *smdata = ldata->dsmdata;
	
	glBindVertexArray(pdata->vao);
	
	shaderProgramUse(smdata->program);

	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(-1, 1, 1));
	modelMatrixRotate(matrices, -90, glm::vec3(0, 1, 0));

	shaderProgramSetDirShadowMatrices(smdata->program, matrices);

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void terrainPointShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp matrices[6])
{
	struct pointShadowMapData *smdata = ldata->psmdata;

	dVector3 lengths;
	dGeomBoxGetLengths(pdata->geom, lengths);

	glBindVertexArray(pdata->vao);
	
	shaderProgramUse(smdata->program);

	for (int i = 0; i < 6; i++) {			
		modelMatrixLoadIdentity(matrices + i);
		modelMatrixConvertToGL(matrices + i, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
		modelMatrixScale(matrices + i, glm::vec3(-1, 1, 1));
		modelMatrixRotate(matrices + i, -90, glm::vec3(0, 1, 0));
	}
	
	shaderProgramSetPointShadowMatrices(smdata->program, matrices);
	shaderProgramSet1f(smdata->program, "near", smdata->near);
	shaderProgramSet1f(smdata->program, "far", smdata->far);
	shaderProgramSet4fv(smdata->program, "lightPos", 1, glm::value_ptr(ldata->position));

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void terrainSpotShadowDraw(struct primitiveData *pdata, struct lightSettings *ldata, struct mvp *matrices)
{
	struct spotShadowMapData *smdata = ldata->ssmdata;
	
	glBindVertexArray(pdata->vao);
	
	shaderProgramUse(smdata->program);

	modelMatrixLoadIdentity(matrices);
	modelMatrixConvertToGL(matrices, dGeomGetPosition(pdata->geom), dGeomGetRotation(pdata->geom));
	modelMatrixScale(matrices, glm::vec3(-1, 1, 1));
	modelMatrixRotate(matrices, -90, glm::vec3(0, 1, 0));

	shaderProgramSetSpotShadowMatrices(smdata->program, matrices);

	glDrawArrays(GL_TRIANGLES, 0, pdata->size);

	shaderProgramUse(0);

	glBindVertexArray(0);
}

void terrainDestroy(struct primitiveData *pdata)
{
	dGeomDestroy(pdata->geom);

	terrainCounter--;
	if (!terrainCounter) {
		glDeleteBuffers(1, &pdata->vbo);
		glDeleteVertexArrays(1, &pdata->vao);
	}

	shaderProgramCounter--;
	if (!shaderProgramCounter)
		shaderProgramDelete(pdata->program);

	free(pdata);
}

