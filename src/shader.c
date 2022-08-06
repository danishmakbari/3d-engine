#include <engine/shader/shader.h>

static char vertexshader[] = {
	"#version 460 core\n"
	"#define CSM_SPLITS 4\n"
	"#define SPOT_LIGHTS_WITH_SHADOWS 6\n"

	"layout(location = 0) in vec3 vertexPosition;"
	"layout(location = 1) in vec3 vertexNormal;"
	"layout(location = 2) in vec3 vertexTexCoord;"
	"layout(location = 3) in vec3 vertexTangent;"
	"layout(location = 4) in vec3 vertexBitangent;"

	"out VS_OUT {"
	"	vec3 normal;"
	"	vec3 fragPos;"
	"	vec3 texCoord;"
	"	vec4 dirShadowCoord[CSM_SPLITS];"
	"	float zView;"
	"	vec4 spotShadowCoord[SPOT_LIGHTS_WITH_SHADOWS];"
	"	mat3 TBN;"
	"} vs_out;"

	"uniform bool dirShadowEnabled;"
	"uniform mat4 dirShadowMatrix[CSM_SPLITS];"

	"uniform bool spotShadowEnabled[SPOT_LIGHTS_WITH_SHADOWS];"
	"uniform mat4 spotShadowMatrix[SPOT_LIGHTS_WITH_SHADOWS];"

	"uniform mat4 mvp;"
	"uniform mat4 modelMatrix;"
	"uniform mat3 normalMatrix;"

	"void main()"
	"{"
	"	gl_Position = mvp * vec4(vertexPosition, 1);"
	"	vs_out.normal = normalize(normalMatrix * vertexNormal);"
	"	vs_out.fragPos = vec3(modelMatrix * vec4(vertexPosition, 1));"
	"	vs_out.texCoord = vertexTexCoord;"
	"	vs_out.zView = gl_Position.z;"

	"	if (dirShadowEnabled) {"
	"		for (int i = 0; i < CSM_SPLITS; i++) {"
	"			vs_out.dirShadowCoord[i] = dirShadowMatrix[i] * vec4(vs_out.fragPos, 1);"
	"		}"
	"	}"

	"	for (int i = 0; i < SPOT_LIGHTS_WITH_SHADOWS; i++) {"
	"		if (spotShadowEnabled[i]) {"
	"			vs_out.spotShadowCoord[i] = spotShadowMatrix[i] * vec4(vs_out.fragPos, 1);"
	"			vs_out.spotShadowCoord[i] /= vs_out.spotShadowCoord[i].w;"
	"		}"
	"	}"
	
	"	vec3 T = normalize(normalMatrix * vertexTangent);"
	"	vec3 B = normalize(normalMatrix * vertexBitangent);"
	"	vec3 N = vs_out.normal;"
	"	vs_out.TBN = mat3(T, B, N);"
	"}\n"
};

static char fragmentshader[] = {
	"#version 460 core\n"
	
	"#define MAX_LIGHTS 16\n"
	"#define CSM_SPLITS 4\n"
	"#define POINT_LIGHTS_WITH_SHADOWS 6\n"
	"#define SPOT_LIGHTS_WITH_SHADOWS 6\n"

	"#define MAX_MATERIALS 2\n"

	"#define TEXTURE_2D 0\n"
	"#define TEXTURE_CUBEMAP 1\n"

	"#define DIRECTION_LIGHT 0\n"
	"#define DIRECTION_SHADOW_LIGHT 1\n"
	"#define POINT_LIGHT 2\n"
	"#define POINT_SHADOW_LIGHT 3\n"
	"#define SPOT_LIGHT 4\n"
	"#define SPOT_SHADOW_LIGHT 5\n"

	"in VS_OUT {"
	"	vec3 normal;"
	"	vec3 fragPos;"
	"	vec3 texCoord;"
	"	vec4 dirShadowCoord[CSM_SPLITS];"
	"	float zView;"
	"	vec4 spotShadowCoord[SPOT_LIGHTS_WITH_SHADOWS];"
	"	mat3 TBN;"
	"} fs_in;"

	"layout(location = 0) out vec4 fragColor;"

	"struct lightSettings {"
	"	int lightType;"
	"	vec3 position;"
	"	vec4 ambient;"
	"	vec4 diffuse;"
	"	vec4 specular;"

	"	vec3 direction;"
	"	float cutoff;"
	"};"

	"struct materialSettings {"
	"	vec4 diffuse;"
	"	vec4 specular;"
	"       float shininess;"
	"	vec4 emissive;"
	
	"	sampler2D diffuseMap2D;"
	"	sampler2D specularMap2D;"
	"	sampler2D emissiveMap2D;"
	"	sampler2D normalMap2D;"
	
	"	samplerCube diffuseMapCube;"
	"	samplerCube specularMapCube;"
	"	samplerCube emissiveMapCube;"
	"	samplerCube normalMapCube;"

	"	bool diffuseMapEnabled;"
	"	bool specularMapEnabled;"
	"	bool emissiveMapEnabled;"
	"	bool normalMapEnabled;"

	"};"

	"uniform int lightCount;"
	"uniform lightSettings light[MAX_LIGHTS];"

	"uniform int materialCount;"
	"uniform int textureType;"
	"uniform materialSettings material[MAX_MATERIALS];"

	"uniform bool dirShadowEnabled;"
	"uniform int dirLightIndex;"
	"uniform sampler2DArray dirShadowMap;"
	"uniform float csmFarPlanes[CSM_SPLITS];"
	
	"uniform bool pointShadowEnabled[POINT_LIGHTS_WITH_SHADOWS];"
	"uniform int pointLightIndex[POINT_LIGHTS_WITH_SHADOWS];"
	"uniform samplerCube pointShadowMap[POINT_LIGHTS_WITH_SHADOWS];"
	"uniform float pointNear[POINT_LIGHTS_WITH_SHADOWS];"
	"uniform float pointFar[POINT_LIGHTS_WITH_SHADOWS];"

	"uniform bool spotShadowEnabled[SPOT_LIGHTS_WITH_SHADOWS];"
	"uniform int spotLightIndex[SPOT_LIGHTS_WITH_SHADOWS];"
	"uniform sampler2D spotShadowMap[SPOT_LIGHTS_WITH_SHADOWS];"

	"uniform vec3 cameraPos;"

	"float direction_hard_shadow(int layer)"
	"{"
	"	float shadow = 0;"

	"	float bias = 0.005;"
	"	vec4 shadowCoord = fs_in.dirShadowCoord[layer] * 0.5 + 0.5;"
	"	shadowCoord.z -= bias;"

	"	float currentDepth = shadowCoord.z;"

	"	float texelSize = 1.0 / textureSize(dirShadowMap, 0).x;"
	"	for (int x = -1; x <= 1; x++) {"
	"		for (int y = -1; y <= 1; y++) {"
	"			float shadowMapDepth = texture(dirShadowMap, vec3(shadowCoord.xy + vec2(x, y) * texelSize, layer)).r;"
	"			shadow += shadowMapDepth > currentDepth ? 1 : 0;"
	"		}"
	"	}"
	"	shadow /= 9;"

	"	return shadow;"
	"}"

	"float dirShadowGet(int lightIndex)"
	"{"
	"	if (dirShadowEnabled && lightIndex == dirLightIndex)"
	"		for (int layer = 0; layer < CSM_SPLITS; layer++)"
	"			if (fs_in.zView <= csmFarPlanes[layer])"
	"				return direction_hard_shadow(layer);"
	"	return 1;"
	"}"

	"float point_hard_shadow(samplerCube shadowMap, vec3 lightPos, float near, float far)"
	"{"
	"	float shadow = 0;"

	"	float bias = 0.005;"
	"	vec3 lightToFrag = fs_in.fragPos - lightPos;"

	"	float currentDepth = (length(lightToFrag) - near) / (far - near) - bias;"

	"	float texelSize = 1.0 / textureSize(shadowMap, 0).x;"
	"	for (int x = -1; x <= 1; x++) {"
	"		for (int y = -1; y <= 1; y++) {"
	"			for (int z = -1; z <= 1; z++) {"
	"				float shadowMapDepth = texture(shadowMap, lightToFrag + vec3(x, y, z) * texelSize).r;"
	"				shadow += shadowMapDepth > currentDepth ? 1 : 0;"
	"			}"
	"		}"
	"	}"
	"	shadow /= 27;"

	"	return shadow;"
	"}"

	"float pointShadowGet(int lightIndex)"
	"{"
	"	for (int i = 0; i < POINT_LIGHTS_WITH_SHADOWS; i++)"
	"		if (lightIndex == pointLightIndex[i] && pointShadowEnabled[i])"
	"			return point_hard_shadow(pointShadowMap[i], light[lightIndex].position, pointNear[i], pointFar[i]);"
	"	return 1;"
	"}"

	"float spot_hard_shadow(sampler2D shadowMap, vec4 spotShadowCoord)"
	"{"
	"	float shadow = 0;"

	"	float bias = 0.005;"
	"	vec4 shadowCoord = spotShadowCoord * 0.5 + 0.5;"
	"	shadowCoord.z -= bias;"

	"	float currentDepth = shadowCoord.z;"

	"	float texelSize = 1.0 / textureSize(shadowMap, 0).x;"
	"	for (int x = -1; x <= 1; x++) {"
	"		for (int y = -1; y <= 1; y++) {"
	"			float shadowMapDepth = texture(shadowMap, shadowCoord.xy + vec2(x, y) * texelSize).r;"
	"			shadow += shadowMapDepth > currentDepth ? 1 : 0;"
	"		}"
	"	}"
	"	shadow /= 9;"

	"	return shadow;"
	"}"

	"float spotShadowGet(int lightIndex)"
	"{"
	"	for (int i = 0; i < SPOT_LIGHTS_WITH_SHADOWS; i++)"
	"		if (lightIndex == spotLightIndex[i] && spotShadowEnabled[i])"
	"			return spot_hard_shadow(spotShadowMap[i], fs_in.spotShadowCoord[i]);"
	"	return 1;"
	"}"

	"vec4 ambientAndDiffuseGet(void)"
	"{"
	"	vec4 resultTexel = vec4(1);"
	"	if (material[0].diffuseMapEnabled)"
	"		if (textureType == TEXTURE_2D)"
	"			resultTexel = texture(material[0].diffuseMap2D, fs_in.texCoord.xy);"
	"		else if (textureType == TEXTURE_CUBEMAP)"
	"			resultTexel = texture(material[0].diffuseMapCube, fs_in.texCoord);"
	"	vec4 resultMaterial = material[0].diffuse;"

	"	for (int i = 1; i < materialCount; i++) {"
	"		vec4 currentTexel;"
	"		if (material[i].diffuseMapEnabled)"
	"			if (textureType == TEXTURE_2D)"
	"				currentTexel = texture(material[i].diffuseMap2D, fs_in.texCoord.xy);"
	"			else if (textureType == TEXTURE_CUBEMAP)"
	"				currentTexel = texture(material[i].diffuseMapCube, fs_in.texCoord);"

	"		resultTexel = mix(resultTexel, currentTexel, 0.5);"
	"		resultMaterial = mix(resultMaterial, material[i].diffuse, 0.5);"
	"	}"

	"	return resultTexel * resultMaterial;"
	"}"

	"vec4 specularGet(void)"
	"{"
	"	vec4 resultTexel = vec4(0);"
	"	if (material[0].specularMapEnabled)"
	"		if (textureType == TEXTURE_2D)"
	"			resultTexel = texture(material[0].specularMap2D, fs_in.texCoord.xy);"
	"		else if (textureType == TEXTURE_CUBEMAP)"
	"			resultTexel = texture(material[0].specularMapCube, fs_in.texCoord);"
	"	vec4 resultMaterial = material[0].specular;"

	"	for (int i = 1; i < materialCount; i++) {"
	"		vec4 currentTexel;"
	"		if (material[i].specularMapEnabled)"
	"			if (textureType == TEXTURE_2D)"
	"				currentTexel = texture(material[i].specularMap2D, fs_in.texCoord.xy);"
	"			else if (textureType == TEXTURE_CUBEMAP)"
	"				currentTexel = texture(material[i].specularMapCube, fs_in.texCoord);"

	"		resultTexel = mix(resultTexel, currentTexel, 0.5);"
	"		resultMaterial = mix(resultMaterial, material[i].specular, 0.5);"
	"	}"

	"	return resultTexel * resultMaterial;"
	"}"

	"vec4 emissiveGet(void)"
	"{"
	"	vec4 resultTexel = vec4(1);"
	"	if (material[0].emissiveMapEnabled)"
	"		if (textureType == TEXTURE_2D)"
	"			resultTexel = texture(material[0].emissiveMap2D, fs_in.texCoord.xy);"
	"		else if (textureType == TEXTURE_CUBEMAP)"
	"			resultTexel = texture(material[0].emissiveMapCube, fs_in.texCoord);"
	"	vec4 resultMaterial = material[0].emissive;"
	"	for (int i = 1; i < materialCount; i++) {"
	"		vec4 currentTexel;"
	"		if (material[i].emissiveMapEnabled)"
	"			if (textureType == TEXTURE_2D)"
	"				currentTexel = texture(material[i].emissiveMap2D, fs_in.texCoord.xy);"
	"			else if (textureType == TEXTURE_CUBEMAP)"
	"				currentTexel = texture(material[i].emissiveMapCube, fs_in.texCoord);"

	"		resultTexel = mix(resultTexel, currentTexel, 0.5);"
	"		resultMaterial = mix(resultMaterial, material[i].emissive, 0.5);"
	"	}"

	"	return resultTexel * resultMaterial;"
	"}"

	"vec3 normalGet(void)"
	"{"
	"	vec3 normal = fs_in.normal;"
	"	if (material[0].normalMapEnabled)"
	"		if (textureType == TEXTURE_2D) {"
	"			normal = texture(material[0].normalMap2D, fs_in.texCoord.xy).rgb;"
	"			normal = normal * 2 - 1;"
	"			normal = normalize(fs_in.TBN * normal);"
	"		} else if (textureType == TEXTURE_CUBEMAP) {"
	"			normal = texture(material[0].normalMapCube, fs_in.texCoord).rgb;"
	"			normal = normal * 2 - 1;"
	"			normal = normalize(fs_in.TBN * normal);"
	"		}"

	"	if (gl_FrontFacing)"
	"		return normal;"
	"	else"
	"		return -normal;"
	"}"

	"vec4 phongModel(void)"
	"{"
	"	vec4 ambient = vec4(0), diffuse = vec4(0), specular = vec4(0);"
	"	vec3 normal = normalGet();"
	"	vec3 viewdir = normalize(cameraPos - fs_in.fragPos);"
	"	for (int i = 0; i < lightCount; i++) {"
	"		if (light[i].lightType == DIRECTION_LIGHT || light[i].lightType == DIRECTION_SHADOW_LIGHT) {"
	"			vec3 lightdir = normalize(light[i].position);"
	"			vec3 halfway = normalize(viewdir + lightdir);"
	"			float shadowFactor = dirShadowGet(i);"
	"			vec4 ambAndDif = ambientAndDiffuseGet();"
	"			ambient += light[i].ambient * ambAndDif;"
	"			diffuse += shadowFactor * ambAndDif * light[i].diffuse * max(dot(lightdir, normal), 0.0);"
	"			specular += shadowFactor * specularGet() * light[i].specular * pow(max(dot(halfway, normal), 0.0), material[0].shininess);"
	"		} else if (light[i].lightType == POINT_LIGHT || light[i].lightType == POINT_SHADOW_LIGHT) {"
	"			vec3 lightdir = normalize(light[i].position - fs_in.fragPos);"
	"			vec3 halfway = normalize(viewdir + lightdir);"
	"			float shadowFactor = pointShadowGet(i);"
	"			vec4 ambAndDif = ambientAndDiffuseGet();"
	"			ambient += light[i].ambient * ambAndDif;"
	"			diffuse += shadowFactor * ambAndDif * light[i].diffuse * max(dot(lightdir, normal), 0.0);"
	"			specular += shadowFactor * specularGet() * light[i].specular * pow(max(dot(halfway, normal), 0.0), material[0].shininess);"
	"		} else {"
	"			vec3 lightdir = normalize(light[i].position - fs_in.fragPos);"
	"			float theta = dot(lightdir, normalize(-light[i].direction));"
	"			if (theta > light[i].cutoff) {"
	"				vec3 halfway = normalize(viewdir + lightdir);"
	"				float shadowFactor = spotShadowGet(i);"
	"				vec4 ambAndDif = ambientAndDiffuseGet();"
	"				ambient += light[i].ambient * ambAndDif;"
	"				diffuse += shadowFactor * ambAndDif * light[i].diffuse * max(dot(lightdir, normal), 0.0);"
	"				specular += shadowFactor * specularGet() * light[i].specular * pow(max(dot(halfway, normal), 0.0), material[0].shininess);"
	"			} else {"
	"				ambient += light[i].ambient * ambientAndDiffuseGet();"
	"			}"
	"		}"
	"	}"
	"	return ambient + diffuse + specular + emissiveGet();"
	"}"

	"void main()"
	"{"
	"	fragColor = phongModel();"
	"}\n"
};

GLuint shaderProgramCompileDefaultProgram(void)
{
	return shaderProgramCompile(vertexshader, fragmentshader, NULL);
}

void shaderProgramSetDirShadowMatrices(GLuint program, struct mvp *matrices)
{
	shaderProgramSetMatrix4fv(program, "modelMatrix", 1, GL_FALSE, glm::value_ptr(modelMatrixGet(matrices)));
	for (int i = 0; i < CSM_SPLITS; i++) {
		std::string str_i = std::to_string(i);
		shaderProgramSetMatrix4fv(program, ("shadowMatrix[" + str_i + "]").c_str(), 1, GL_FALSE, glm::value_ptr(VPMatrixGet(matrices + i)));
	}
}

void shaderProgramSetPointShadowMatrices(GLuint program, struct mvp matrices[6])
{
	shaderProgramSetMatrix4fv(program, "modelMatrix", 1, GL_FALSE, glm::value_ptr(modelMatrixGet(matrices)));
	for (int i = 0; i < 6; i++) {
		std::string str_i = std::to_string(i);
		shaderProgramSetMatrix4fv(program, ("shadowMatrix[" + str_i + "]").c_str(), 1, GL_FALSE, glm::value_ptr(VPMatrixGet(matrices + i)));
	}
}

void shaderProgramSetSpotShadowMatrices(GLuint program, struct mvp *matrices)
{
	shaderProgramSetMatrix4fv(program, "shadowMatrix", 1, GL_FALSE, glm::value_ptr(MVPMatrixGet(matrices)));
}


void shaderProgramSetMatrices(GLuint program)
{
	shaderProgramSetMatrix4fv(program, "mvp", 1, GL_FALSE, glm::value_ptr(MVPMatrixGet()));
	shaderProgramSetMatrix4fv(program, "modelMatrix", 1, GL_FALSE, glm::value_ptr(modelMatrixGet()));
	shaderProgramSetMatrix3fv(program, "normalMatrix", 1, GL_FALSE, glm::value_ptr(normalMatrixGet()));
	shaderProgramSet3fv(program, "cameraPos", 1, glm::value_ptr(viewMatrixCameraPosGet()));
}

void shaderProgramSetMaterials2D(GLuint program, struct materialList *material)
{
	shaderProgramSet1i(program, "textureType", TEXTURE_2D);
	shaderProgramSet1i(program, "materialCount", materialListSizeGet(material));
	
	for (unsigned i = 0; i < materialListSizeGet(material); i++) {
		struct materialSettings *m = materialListElementGet(material, i);
		std::string str_i = std::to_string(i);
		
		shaderProgramSet4fv(program, ("material[" + str_i + "].diffuse").c_str(), 1, glm::value_ptr(m->diffuse));
		shaderProgramSet4fv(program, ("material[" + str_i + "].specular").c_str(), 1, glm::value_ptr(m->specular));
		shaderProgramSet1f(program, ("material[" + str_i + "].shininess").c_str(), m->shininess);
		shaderProgramSet4fv(program, ("material[" + str_i + "].emissive").c_str(), 1, glm::value_ptr(m->emissive));

		if (m->diffuseMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].diffuseMap2D").c_str(), TEXTURE_OFFSET + i * 4);
			shaderProgramSet1i(program, ("material[" + str_i + "].diffuseMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, m->diffuseMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].diffuseMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		if (m->specularMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].specularMap2D").c_str(), TEXTURE_OFFSET + i * 4 + 1);
			shaderProgramSet1i(program, ("material[" + str_i + "].specularMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, m->specularMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].specularMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		}

		if (m->emissiveMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].emissiveMap2D").c_str(), TEXTURE_OFFSET + i * 4 + 2);
			shaderProgramSet1i(program, ("material[" + str_i + "].emissiveMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, m->emissiveMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].emissiveMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 2);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		if (m->normalMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].normalMap2D").c_str(), TEXTURE_OFFSET + i * 4 + 3);
			shaderProgramSet1i(program, ("material[" + str_i + "].normalMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, m->normalMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].normalMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 3);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	}
}

void shaderProgramSetMaterialsCube(GLuint program, struct materialList *material)
{
	shaderProgramSet1i(program, "textureType", TEXTURE_CUBEMAP);
	shaderProgramSet1i(program, "materialCount", materialListSizeGet(material));

	for (unsigned i = 0; i < materialListSizeGet(material); i++) {
		struct materialSettings *m = materialListElementGet(material, i);
		std::string str_i = std::to_string(i);
		
		shaderProgramSet4fv(program, ("material[" + str_i + "].diffuse").c_str(), 1, glm::value_ptr(m->diffuse));
		shaderProgramSet4fv(program, ("material[" + str_i + "].specular").c_str(), 1, glm::value_ptr(m->specular));
		shaderProgramSet1f(program, ("material[" + str_i + "].shininess").c_str(), m->shininess);
		shaderProgramSet4fv(program, ("material[" + str_i + "].emissive").c_str(), 1, glm::value_ptr(m->emissive));

		if (m->diffuseMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].diffuseMapCube").c_str(), TEXTURE_OFFSET + i * 4);
			shaderProgramSet1i(program, ("material[" + str_i + "].diffuseMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m->diffuseMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].diffuseMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (m->specularMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].specularMapCube").c_str(), TEXTURE_OFFSET + i * 4 + 1);
			shaderProgramSet1i(program, ("material[" + str_i + "].specularMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m->specularMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].specularMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (m->emissiveMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].emissiveMapCube").c_str(), TEXTURE_OFFSET + i * 4 + 2);
			shaderProgramSet1i(program, ("material[" + str_i + "].emissiveMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 2);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m->emissiveMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].emissiveMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 2);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (m->normalMap) {
			shaderProgramSet1i(program, ("material[" + str_i + "].normalMapCube").c_str(), TEXTURE_OFFSET + i * 4 + 3);
			shaderProgramSet1i(program, ("material[" + str_i + "].normalMapEnabled").c_str(), true);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 3);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m->normalMap);
		} else {
			shaderProgramSet1i(program, ("material[" + str_i + "].normalMapEnabled").c_str(), false);
			glActiveTexture(GL_TEXTURE0 + TEXTURE_OFFSET + i * 4 + 3);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}


void shaderProgramSetLights(GLuint program, struct lightList *root)
{
	shaderProgramSet1i(program, "dirShadowEnabled", false);
	
	int pointShadowCounter = 0;
	for (int i = 0; i < POINT_LIGHTS_WITH_SHADOWS; i++) {
		std::string str_i = std::to_string(i);
		shaderProgramSet1i(program, ("pointShadowEnabled[" + str_i + "]").c_str(), false);
	}
	
	int spotShadowCounter = 0;
	for (int i = 0; i < SPOT_LIGHTS_WITH_SHADOWS; i++) {
		std::string str_i = std::to_string(i);
		shaderProgramSet1i(program, ("spotShadowEnabled[" + str_i + "]").c_str(), false);
	}

	for (unsigned i = 0; i < lightListSizeGet(root); i++) {
		struct lightSettings *light = lightListElementGet(root, i);
		struct dirShadowMapData *dsmdata = light->dsmdata;
		struct pointShadowMapData *psmdata = light->psmdata;
		struct spotShadowMapData *ssmdata = light->ssmdata;

		if (light->lightType == DIRECTION_SHADOW_LIGHT) {
			shaderProgramSet1i(program, "dirShadowEnabled", true);
			shaderProgramSet1i(program, "dirLightIndex", i);
			
			shaderProgramSet1i(program, "dirShadowMap", DIR_SM_OFFSET);
			glActiveTexture(GL_TEXTURE0 + DIR_SM_OFFSET);
			glBindTexture(GL_TEXTURE_2D_ARRAY, dsmdata->texture);

			for (int j = 0; j < CSM_SPLITS; j++) {
				std::string str_j = std::to_string(j);

				modelMatrixLoadIdentity(dsmdata->matrices + j);
				shaderProgramSetMatrix4fv(program, ("dirShadowMatrix[" + str_j + "]").c_str(), 1, GL_FALSE, glm::value_ptr(MVPMatrixGet(dsmdata->matrices + j)));
				shaderProgramSet1f(program, ("csmFarPlanes[" + str_j + "]").c_str(), dsmdata->csmPlanes[j + 1]);
			}
		} else if (light->lightType == POINT_SHADOW_LIGHT && pointShadowCounter < POINT_LIGHTS_WITH_SHADOWS) {
			std::string str_point = std::to_string(pointShadowCounter);

			glActiveTexture(GL_TEXTURE0 + POINT_SM_OFFSET + pointShadowCounter);
			glBindTexture(GL_TEXTURE_CUBE_MAP, psmdata->texture);
				
			shaderProgramSet1i(program, ("pointShadowMap[" + str_point + "]").c_str(), POINT_SM_OFFSET + pointShadowCounter);
			shaderProgramSet1i(program, ("pointShadowEnabled[" + str_point + "]").c_str(), true);
			shaderProgramSet1i(program, ("pointLightIndex[" + str_point + "]").c_str(), i);

			shaderProgramSet1f(program, ("pointNear[" + str_point + "]").c_str(), psmdata->near);
			shaderProgramSet1f(program, ("pointFar[" + str_point + "]").c_str(), psmdata->far);

			pointShadowCounter++;
		} else if (light->lightType == SPOT_SHADOW_LIGHT && spotShadowCounter < SPOT_LIGHTS_WITH_SHADOWS) {
			std::string str_spot = std::to_string(spotShadowCounter);

			glActiveTexture(GL_TEXTURE0 + SPOT_SM_OFFSET + spotShadowCounter);
			glBindTexture(GL_TEXTURE_2D, ssmdata->texture);

			shaderProgramSet1i(program, ("spotShadowMap[" + str_spot + "]").c_str(), SPOT_SM_OFFSET + spotShadowCounter);
			shaderProgramSet1i(program, ("spotShadowEnabled[" + str_spot + "]").c_str(), true);
			shaderProgramSet1i(program, ("spotLightIndex[" + str_spot + "]").c_str(), i);
		
			modelMatrixLoadIdentity(ssmdata->matrices);
			shaderProgramSetMatrix4fv(program, ("spotShadowMatrix[" + str_spot + "]").c_str(), 1, GL_FALSE, glm::value_ptr(MVPMatrixGet(ssmdata->matrices)));
		
			spotShadowCounter++;
		}
	}

	shaderProgramSet1i(program, "lightCount", lightListSizeGet(root));
	for (unsigned i = 0; i < lightListSizeGet(root); i++) {
		std::string str_i = std::to_string(i);
		struct lightSettings *light = lightListElementGet(root, i);
		shaderProgramSet1i(program, ("light[" + str_i + "].lightType").c_str(), light->lightType);
		shaderProgramSet3fv(program, ("light[" + str_i + "].position").c_str(), 1, glm::value_ptr(light->position));
		shaderProgramSet4fv(program, ("light[" + str_i + "].ambient").c_str(), 1, glm::value_ptr(light->ambient));
		shaderProgramSet4fv(program, ("light[" + str_i + "].diffuse").c_str(), 1, glm::value_ptr(light->diffuse));
		shaderProgramSet4fv(program, ("light[" + str_i + "].specular").c_str(), 1, glm::value_ptr(light->specular));
		
		shaderProgramSet3fv(program, ("light[" + str_i + "].direction").c_str(), 1, glm::value_ptr(light->direction));
		shaderProgramSet1f(program, ("light[" + str_i + "].cutoff").c_str(), light->cutoff);
	}
}

static GLuint shaderCompile(char* shadersrc, GLuint shadertype)
{
	GLuint shaderID = glCreateShader(shadertype);
	glShaderSource(shaderID, 1, &shadersrc, NULL);
	glCompileShader(shaderID);

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
		char *errorLog = (char *) malloc(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, errorLog);
		fprintf(stderr, "%s\n", errorLog);
		free(errorLog);
		glDeleteShader(shaderID);
		return 0;
	}

	return shaderID;
}

static GLuint shaderCompileFromFile(char* filename, GLuint shadertype)
{
	FILE *fl = fopen(filename, "rb");
	fseek(fl, 0, SEEK_END);
	size_t size = ftell(fl);
	rewind(fl);
	GLchar *shadersrc = (GLchar *) malloc(size);
	fread(shadersrc, 1, size, fl);
	fclose(fl);

	GLuint shaderID = shaderCompile(shadersrc, shadertype);

	free(shadersrc);

	return shaderID;
}

void shaderProgramUse(GLuint program)
{
	glUseProgram(program);
}

void shaderProgramDelete(GLuint program)
{
	glDeleteProgram(program);
}

GLuint shaderProgramCompileFromFiles(char *vshaderf, char *fshaderf, char *gshaderf)
{
	GLuint vertexShader = shaderCompileFromFile(vshaderf, GL_VERTEX_SHADER);
	GLuint fragmentShader = shaderCompileFromFile(fshaderf, GL_FRAGMENT_SHADER);
	GLuint geometryShader;

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (gshaderf) {
		geometryShader = shaderCompileFromFile(gshaderf, GL_GEOMETRY_SHADER);
		glAttachShader(shaderProgram, geometryShader);
	}
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (gshaderf)
		glDeleteShader(geometryShader);

	return shaderProgram;
}

GLuint shaderProgramCompile(char *vshadersrc, char *fshadersrc, char *gshadersrc)
{
	GLuint vertexShader = shaderCompile(vshadersrc, GL_VERTEX_SHADER);
	GLuint fragmentShader = shaderCompile(fshadersrc, GL_FRAGMENT_SHADER);
	GLuint geometryShader;

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if (gshadersrc) {
		geometryShader = shaderCompile(gshadersrc, GL_GEOMETRY_SHADER);
		glAttachShader(shaderProgram, geometryShader);
	}
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (gshadersrc)
		glDeleteShader(geometryShader);

	return shaderProgram;
}

void shaderProgramSet1f(GLuint program, const char *name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

void shaderProgramSet2f(GLuint program, const char *name, GLfloat value1, GLfloat value2)
{
	glUniform2f(glGetUniformLocation(program, name), value1, value2);
}

void shaderProgramSet3f(GLuint program, const char *name, GLfloat value1, GLfloat value2, GLfloat value3)
{
	glUniform3f(glGetUniformLocation(program, name), value1, value2, value3);
}

void shaderProgramSet4f(GLuint program, const char *name, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4)
{
	glUniform4f(glGetUniformLocation(program, name), value1, value2, value3, value4);
}


void shaderProgramSet1i(GLuint program, const char *name, GLint value)
{
	glUniform1i(glGetUniformLocation(program, name), value);
}

void shaderProgramSet2i(GLuint program, const char *name, GLint value1, GLint value2)
{
	glUniform2i(glGetUniformLocation(program, name), value1, value2);
}

void shaderProgramSet3i(GLuint program, const char *name, GLint value1, GLint value2, GLint value3)
{
	glUniform3i(glGetUniformLocation(program, name), value1, value2, value3);
}

void shaderProgramSet4i(GLuint program, const char *name, GLint value1, GLint value2, GLint value3, GLint value4)
{
	glUniform4i(glGetUniformLocation(program, name), value1, value2, value3, value4);
}


void shaderProgramSet1ui(GLuint program, const char *name, GLuint value)
{
	glUniform1ui(glGetUniformLocation(program, name), value);
}

void shaderProgramSet2ui(GLuint program, const char *name, GLuint value1, GLuint value2)
{
	glUniform2ui(glGetUniformLocation(program, name), value1, value2);
}

void shaderProgramSet3ui(GLuint program, const char *name, GLuint value1, GLuint value2, GLuint value3)
{
	glUniform3ui(glGetUniformLocation(program, name), value1, value2, value3);
}

void shaderProgramSet4ui(GLuint program, const char *name, GLuint value1, GLuint value2, GLuint value3, GLuint value4)
{
	glUniform4ui(glGetUniformLocation(program, name), value1, value2, value3, value4);
}


void shaderProgramSet1fv(GLuint program, const char *name, GLsizei count, const GLfloat *value)
{
	glUniform1fv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet2fv(GLuint program, const char *name, GLsizei count, const GLfloat *value)
{
	glUniform2fv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet3fv(GLuint program, const char *name, GLsizei count, const GLfloat *value)
{
	glUniform3fv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet4fv(GLuint program, const char *name, GLsizei count, const GLfloat *value)
{
	glUniform4fv(glGetUniformLocation(program, name), count, value);
}


void shaderProgramSet1iv(GLuint program, const char *name, GLsizei count, const GLint *value)
{
	glUniform1iv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet2iv(GLuint program, const char *name, GLsizei count, const GLint *value)
{
	glUniform2iv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet3iv(GLuint program, const char *name, GLsizei count, const GLint *value)
{
	glUniform3iv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet4iv(GLuint program, const char *name, GLsizei count, const GLint *value)
{
	glUniform4iv(glGetUniformLocation(program, name), count, value);
}


void shaderProgramSet1uiv(GLuint program, const char *name, GLsizei count, const GLuint *value)
{
	glUniform1uiv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet2uiv(GLuint program, const char *name, GLsizei count, const GLuint *value)
{
	glUniform2uiv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet3uiv(GLuint program, const char *name, GLsizei count, const GLuint *value)
{
	glUniform3uiv(glGetUniformLocation(program, name), count, value);
}

void shaderProgramSet4uiv(GLuint program, const char *name, GLsizei count, const GLuint *value)
{
	glUniform4uiv(glGetUniformLocation(program, name), count, value);
}


void shaderProgramSetMatrix2fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix2fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix3fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix3fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix4fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix4fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix2x3fv(GLuint program, const char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix2x3fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix3x2fv(GLuint program, char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix3x2fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix2x4fv(GLuint program, char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix2x4fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix4x2fv(GLuint program, char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix4x2fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix3x4fv(GLuint program, char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix3x4fv(glGetUniformLocation(program, name), count, transpose, value);
}

void shaderProgramSetMatrix4x3fv(GLuint program, char *name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix4x3fv(glGetUniformLocation(program, name), count, transpose, value);
}

