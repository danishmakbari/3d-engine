#include <engine/shadow/shadow.h>

static char dir_vertexshader[] = {
	"#version 460 core\n"

	"layout(location = 0) in vec3 vertexPosition;"

	"uniform mat4 modelMatrix;"

	"void main()"
	"{"
	"	gl_Position = modelMatrix * vec4(vertexPosition, 1);"
	"}\n"
};

static char dir_geometryshader[] = {
	"#version 460 core\n"
	
	"#define CSM_SPLITS 4\n"

	"layout (triangles) in;"
	"layout (triangle_strip, max_vertices = 3 * CSM_SPLITS) out;"

	"uniform mat4 shadowMatrix[CSM_SPLITS];"

	"void main()"
	"{"
	"	for (int i = 0; i < CSM_SPLITS; i++) {"
	"		gl_Layer = i;"
	"		for (int j = 0; j < 3; j++) {"
	"			gl_Position = shadowMatrix[i] * gl_in[j].gl_Position;"
	"			EmitVertex();"
	"		}"
	"		EndPrimitive();"
	"	}"
	"}\n"
};

static char dir_fragmentshader[] = {
	"#version 460 core\n"

	"void main()"
	"{"
	"	gl_FragDepth = gl_FragCoord.z;"
	"}\n"
};

struct dirShadowMapData *dirShadowMapCreate(GLsizei shadowMapSize, float near, float far, float csmLambda)
{
	struct dirShadowMapData *smdata = (struct dirShadowMapData *) malloc(sizeof(struct dirShadowMapData));
	smdata->size = shadowMapSize;
	smdata->program = shaderProgramCompile(dir_vertexshader, dir_fragmentshader, dir_geometryshader);
	smdata->matrices = (struct mvp *) malloc(sizeof(struct mvp) * CSM_SPLITS);

	smdata->csmPlanes[0] = near;
	for (int i = 1; i < CSM_SPLITS + 1; i++) {
		float iDivSplits = (float) i / CSM_SPLITS;
		float c1 = near * glm::pow(far / near, iDivSplits);
		float c2 = near + (far - near) * iDivSplits;
		smdata->csmPlanes[i] = csmLambda * c1 + (1 - csmLambda) * c2;
	}

	glGenFramebuffers(1, &smdata->fbo);
	glGenTextures(1, &smdata->texture);

	glBindTexture(GL_TEXTURE_2D_ARRAY, smdata->texture);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT24, smdata->size, smdata->size, CSM_SPLITS, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	float border[] = {1, 0, 0, 0};
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, smdata->fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, smdata->texture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

	return smdata;
}

void dirShadowMapDestroy(struct dirShadowMapData *smdata)
{
	glDeleteTextures(1, &smdata->texture);
	glDeleteFramebuffers(1, &smdata->fbo);
	shaderProgramDelete(smdata->program);
	free(smdata->matrices);
	free(smdata);
}

static char point_vertexshader[] = {
	"#version 460 core\n"

	"layout(location = 0) in vec3 vertexPosition;"

	"uniform mat4 modelMatrix;"

	"void main()"
	"{"
	"	gl_Position = modelMatrix * vec4(vertexPosition, 1);"
	"}\n"
};

static char point_geometryshader[] = {
	"#version 460 core\n"

	"layout (triangles) in;"
	"layout (triangle_strip, max_vertices = 18) out;"

	"uniform mat4 shadowMatrix[6];"

	"out vec4 fragPos;"

	"void main()"
	"{"
	"	for (int i = 0; i < 6; i++) {"
	"		gl_Layer = i;"
	"		for (int j = 0; j < 3; j++) {"
	"			fragPos = gl_in[j].gl_Position;"
	"			gl_Position = shadowMatrix[i] * fragPos;"
	"			EmitVertex();"
	"		}"
	"		EndPrimitive();"
	"	}"
	"}\n"
};

static char point_fragmentshader[] = {
	"#version 460 core\n"

	"in vec4 fragPos;"

	"uniform vec4 lightPos;"

	"uniform float near;"
	"uniform float far;"

	"void main()"
	"{"
	"	float dist = distance(lightPos, fragPos);"
	"	gl_FragDepth = (dist - near) / (far - near);"
	"}\n"
};

struct pointShadowMapData *pointShadowMapCreate(GLsizei shadowMapSize, float near, float far)
{
	struct pointShadowMapData *smdata = (struct pointShadowMapData *) malloc(sizeof(struct pointShadowMapData));
	smdata->size = shadowMapSize;
	smdata->program = shaderProgramCompile(point_vertexshader, point_fragmentshader, point_geometryshader);
	smdata->near = near;
	smdata->far = far;

	glGenFramebuffers(1, &smdata->fbo);
	glGenTextures(1, &smdata->texture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, smdata->texture);

	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, smdata->size, smdata->size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, smdata->fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, smdata->texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return smdata;
}

void pointShadowMapDestroy(struct pointShadowMapData *smdata)
{
	glDeleteTextures(1, &smdata->texture);
	glDeleteFramebuffers(1, &smdata->fbo);
	shaderProgramDelete(smdata->program);
	free(smdata);
}

static char spot_vertexshader[] = {
	"#version 460 core\n"

	"layout(location = 0) in vec3 vertexPosition;"

	"uniform mat4 shadowMatrix;"

	"void main()"
	"{"
	"	gl_Position = shadowMatrix * vec4(vertexPosition, 1);"
	"}\n"
};

static char spot_fragmentshader[] = {
	"#version 460 core\n"

	"void main()"
	"{"
	"	gl_FragDepth = gl_FragCoord.z;"
	"}\n"
};

struct spotShadowMapData *spotShadowMapCreate(GLsizei shadowMapSize, float near, float far)
{
	struct spotShadowMapData *smdata = (struct spotShadowMapData *) malloc(sizeof(struct spotShadowMapData));
	smdata->size = shadowMapSize;
	smdata->program = shaderProgramCompile(spot_vertexshader, spot_fragmentshader, NULL);
	smdata->matrices = (struct mvp *) malloc(sizeof(struct mvp));
	smdata->near = near;
	smdata->far = far;

	glGenFramebuffers(1, &smdata->fbo);
	glGenTextures(1, &smdata->texture);

	glBindTexture(GL_TEXTURE_2D, smdata->texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, smdata->size, smdata->size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	float border[] = {1, 0, 0, 0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, smdata->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, smdata->texture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return smdata;
}

void spotShadowMapDestroy(struct spotShadowMapData *smdata)
{
	glDeleteTextures(1, &smdata->texture);
	glDeleteFramebuffers(1, &smdata->fbo);
	shaderProgramDelete(smdata->program);
	free(smdata->matrices);
	free(smdata);
}


