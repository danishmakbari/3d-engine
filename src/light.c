#include <engine/light/light.h>

struct lightList *lightListHeadCreate(void)
{
	struct lightList *root = (struct lightList *) malloc(sizeof(struct lightList));
	root->ldata = NULL;
	root->next = NULL;
	root->prev = NULL;
	return root;
}

void lightListElementsDestroy(struct lightList *root)
{
	struct lightList *current = root->next;
	root->next = NULL;

	while (current) {
		struct lightList *temp = current->next;

		if (current->ldata->lightType == DIRECTION_SHADOW_LIGHT)
			dirShadowMapDestroy(current->ldata->dsmdata);
		if (current->ldata->lightType == POINT_SHADOW_LIGHT)
			pointShadowMapDestroy(current->ldata->psmdata);
		if (current->ldata->lightType == SPOT_SHADOW_LIGHT)
			spotShadowMapDestroy(current->ldata->ssmdata);

		free(current->ldata);
		free(current);
		current = temp;
	}
}

void lightListHeadDestroy(struct lightList *root)
{
	lightListElementsDestroy(root);
	free(root);
}

struct lightSettings *directionLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	struct lightList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct lightList *) malloc(sizeof(struct lightList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->ldata = (struct lightSettings *) malloc(sizeof(struct lightSettings));
	current->next->ldata->lightType = DIRECTION_LIGHT;
	current->next->ldata->position = position;
	current->next->ldata->ambient = ambient;
	current->next->ldata->diffuse = diffuse;
	current->next->ldata->specular = specular;
	current->next->ldata->dsmdata = NULL;
	current->next->ldata->psmdata = NULL;
	current->next->ldata->ssmdata = NULL;

	return current->next->ldata;
}

struct lightSettings *directionShadowLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
	GLsizei smsize, float near, float far, float csmLambda)
{
	struct lightList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct lightList *) malloc(sizeof(struct lightList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->ldata = (struct lightSettings *) malloc(sizeof(struct lightSettings));
	current->next->ldata->lightType = DIRECTION_SHADOW_LIGHT;
	current->next->ldata->position = position;
	current->next->ldata->ambient = ambient;
	current->next->ldata->diffuse = diffuse;
	current->next->ldata->specular = specular;
	current->next->ldata->dsmdata = dirShadowMapCreate(smsize, near, far, csmLambda);
	current->next->ldata->psmdata = NULL;
	current->next->ldata->ssmdata = NULL;

	return current->next->ldata;
}

struct lightSettings *pointLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	struct lightList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct lightList *) malloc(sizeof(struct lightList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->ldata = (struct lightSettings *) malloc(sizeof(struct lightSettings));
	current->next->ldata->lightType = POINT_LIGHT;
	current->next->ldata->position = position;
	current->next->ldata->ambient = ambient;
	current->next->ldata->diffuse = diffuse;
	current->next->ldata->specular = specular;
	current->next->ldata->dsmdata = NULL;
	current->next->ldata->psmdata = NULL;
	current->next->ldata->ssmdata = NULL;

	return current->next->ldata;
}

struct lightSettings *pointShadowLightCreate(struct lightList *root, glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, GLsizei smsize, float near, float far)
{
	struct lightList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct lightList *) malloc(sizeof(struct lightList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->ldata = (struct lightSettings *) malloc(sizeof(struct lightSettings));
	current->next->ldata->lightType = POINT_SHADOW_LIGHT;
	current->next->ldata->position = position;
	current->next->ldata->ambient = ambient;
	current->next->ldata->diffuse = diffuse;
	current->next->ldata->specular = specular;
	current->next->ldata->dsmdata = NULL;
	current->next->ldata->psmdata = pointShadowMapCreate(smsize, near, far);
	current->next->ldata->ssmdata = NULL;

	return current->next->ldata;
}

struct lightSettings *spotLightCreate(struct lightList *root, glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	struct lightList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct lightList *) malloc(sizeof(struct lightList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->ldata = (struct lightSettings *) malloc(sizeof(struct lightSettings));
	current->next->ldata->lightType = SPOT_LIGHT;
	current->next->ldata->position = position;
	current->next->ldata->ambient = ambient;
	current->next->ldata->diffuse = diffuse;
	current->next->ldata->specular = specular;
	current->next->ldata->dsmdata = NULL;
	current->next->ldata->psmdata = NULL;
	current->next->ldata->ssmdata = NULL;

	current->next->ldata->direction = direction;
	current->next->ldata->cutoff = glm::radians(cutoff);

	return current->next->ldata;
}

struct lightSettings *spotShadowLightCreate(struct lightList *root, glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
	GLsizei smsize, float near, float far)
{
	struct lightList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct lightList *) malloc(sizeof(struct lightList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->ldata = (struct lightSettings *) malloc(sizeof(struct lightSettings));
	current->next->ldata->lightType = SPOT_SHADOW_LIGHT;
	current->next->ldata->position = position;
	current->next->ldata->ambient = ambient;
	current->next->ldata->diffuse = diffuse;
	current->next->ldata->specular = specular;
	current->next->ldata->dsmdata = NULL;
	current->next->ldata->psmdata = NULL;
	current->next->ldata->ssmdata = spotShadowMapCreate(smsize, near, far);

	current->next->ldata->direction = glm::normalize(direction);
	current->next->ldata->cutoff = glm::radians(cutoff);

	return current->next->ldata;
}

void lightListElementRemove(struct lightList *root, struct lightSettings *ldata)
{
	if (!root->next)
		return;

	struct lightList *current = root->next;
	while (current->ldata != ldata) {
		if (!current->next)
			return;
		current = current->next;
	}
	current->prev->next = current->next;
	if (current->next)
		current->next->prev = current->prev;

	if (current->ldata->lightType == DIRECTION_SHADOW_LIGHT)
		dirShadowMapDestroy(current->ldata->dsmdata);
	if (current->ldata->lightType == POINT_SHADOW_LIGHT)
		pointShadowMapDestroy(current->ldata->psmdata);
	if (current->ldata->lightType == SPOT_SHADOW_LIGHT)
		spotShadowMapDestroy(current->ldata->ssmdata);

	free(current->ldata);
	free(current);
}

unsigned lightListSizeGet(struct lightList *root)
{
	unsigned counter = 0;
	struct lightList *current = root;
	while (current->next) {
		current = current->next;
		counter++;
	}

	return counter;
}

struct lightSettings *lightListElementGet(struct lightList *root, unsigned num)
{
	struct lightList *current = root;
	while (current->next && num) {
		current = current->next;
		num--;
	}

	if (!current->next)
		return NULL;

	return current->next->ldata;
}

