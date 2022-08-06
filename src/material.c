#include <engine/material/material.h>

struct materialList *materialListHeadCreate(void)
{
	struct materialList *root = (struct materialList *) malloc(sizeof(struct materialList));
	root->mdata = NULL;
	root->next = NULL;
	root->prev = NULL;
	return root;
}

void materialListElementsDestroy(struct materialList *root)
{
	struct materialList *current = root->next;
	root->next = NULL;

	while (current) {
		struct materialList *temp = current->next;
	
		if (current->mdata->diffuseMap)
			glDeleteTextures(1, &current->mdata->diffuseMap);
		if (current->mdata->specularMap)
			glDeleteTextures(1, &current->mdata->specularMap);
		if (current->mdata->emissiveMap)
			glDeleteTextures(1, &current->mdata->emissiveMap);
		if (current->mdata->normalMap)
			glDeleteTextures(1, &current->mdata->normalMap);
	
		free(current->mdata);
		free(current);
		current = temp;
	}
}

void materialListHeadDestroy(struct materialList *root)
{
	materialListElementsDestroy(root);
	free(root);
}

struct materialSettings *materialList2DElementAdd(struct materialList *root, glm::vec4 diffuse,
	glm::vec4 specular, GLfloat shininess, glm::vec4 emissive, const char *diffuseMap, const char *specularMap, const char *emissiveMap, const char *normalMap)
{
	struct materialList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct materialList *) malloc(sizeof(struct materialList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->mdata = (struct materialSettings *) malloc(sizeof(struct materialSettings));

	current->next->mdata->diffuse = diffuse;
	current->next->mdata->specular = specular;
	current->next->mdata->shininess = shininess;
	current->next->mdata->emissive = emissive;

	current->next->mdata->diffuseMap = 0;
	current->next->mdata->specularMap = 0;
	current->next->mdata->emissiveMap = 0;
	current->next->mdata->normalMap = 0;

	if (diffuseMap || specularMap || emissiveMap || normalMap) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if (diffuseMap) {
			SDL_Surface *surface = IMG_Load(diffuseMap);

			glGenTextures(1, &current->next->mdata->diffuseMap);
			glBindTexture(GL_TEXTURE_2D, current->next->mdata->diffuseMap);
			if (surface->format->BytesPerPixel == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
			else if (surface->format->BytesPerPixel == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_2D);

			SDL_FreeSurface(surface);
		}

		if (specularMap) {
			SDL_Surface *surface = IMG_Load(specularMap);

			glGenTextures(1, &current->next->mdata->specularMap);
			glBindTexture(GL_TEXTURE_2D, current->next->mdata->specularMap);
			if (surface->format->BytesPerPixel == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
			else if (surface->format->BytesPerPixel == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_2D);

			SDL_FreeSurface(surface);
		}

		if (emissiveMap) {
			SDL_Surface *surface = IMG_Load(emissiveMap);

			glGenTextures(1, &current->next->mdata->emissiveMap);
			glBindTexture(GL_TEXTURE_2D, current->next->mdata->emissiveMap);
			if (surface->format->BytesPerPixel == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
			else if (surface->format->BytesPerPixel == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_2D);

			SDL_FreeSurface(surface);
		}

		if (normalMap) {
			SDL_Surface *surface = IMG_Load(normalMap);

			glGenTextures(1, &current->next->mdata->normalMap);
			glBindTexture(GL_TEXTURE_2D, current->next->mdata->normalMap);
			if (surface->format->BytesPerPixel == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
			else if (surface->format->BytesPerPixel == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_2D);

			SDL_FreeSurface(surface);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return current->next->mdata;
}

struct materialSettings *materialListCubeElementAdd(struct materialList *root, glm::vec4 diffuse,
	glm::vec4 specular, GLfloat shininess, glm::vec4 emissive, const char *diffuseMap[6], const char *specularMap[6], const char *emissiveMap[6], const char *normalMap[6])
{
	struct materialList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct materialList *) malloc(sizeof(struct materialList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->mdata = (struct materialSettings *) malloc(sizeof(struct materialSettings));

	current->next->mdata->diffuse = diffuse;
	current->next->mdata->specular = specular;
	current->next->mdata->shininess = shininess;
	current->next->mdata->emissive = emissive;

	current->next->mdata->diffuseMap = 0;
	current->next->mdata->specularMap = 0;
	current->next->mdata->emissiveMap = 0;
	current->next->mdata->normalMap = 0;

	if (diffuseMap || specularMap || emissiveMap || normalMap) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if (diffuseMap) {
			glGenTextures(1, &current->next->mdata->diffuseMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->diffuseMap);
			for (int i = 0; i < 6; i++) {
				SDL_Surface *surface = IMG_Load(diffuseMap[i]);
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);	
				SDL_FreeSurface(surface);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		if (specularMap) {
			glGenTextures(1, &current->next->mdata->specularMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->specularMap);
			for (int i = 0; i < 6; i++) {
				SDL_Surface *surface = IMG_Load(specularMap[i]);
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
				SDL_FreeSurface(surface);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		if (emissiveMap) {
			glGenTextures(1, &current->next->mdata->emissiveMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->emissiveMap);
			for (int i = 0; i < 6; i++) {
				SDL_Surface *surface = IMG_Load(emissiveMap[i]);
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
				SDL_FreeSurface(surface);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		if (normalMap) {
			glGenTextures(1, &current->next->mdata->normalMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->normalMap);
			for (int i = 0; i < 6; i++) {
				SDL_Surface *surface = IMG_Load(normalMap[i]);
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
				SDL_FreeSurface(surface);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	return current->next->mdata;
}

struct materialSettings *materialListCube2DElementAdd(struct materialList *root, glm::vec4 diffuse,
	glm::vec4 specular, GLfloat shininess, glm::vec4 emissive, const char *diffuseMap, const char *specularMap, const char *emissiveMap, const char *normalMap)
{
	struct materialList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct materialList *) malloc(sizeof(struct materialList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->mdata = (struct materialSettings *) malloc(sizeof(struct materialSettings));

	current->next->mdata->diffuse = diffuse;
	current->next->mdata->specular = specular;
	current->next->mdata->shininess = shininess;
	current->next->mdata->emissive = emissive;

	current->next->mdata->diffuseMap = 0;
	current->next->mdata->specularMap = 0;
	current->next->mdata->emissiveMap = 0;
	current->next->mdata->normalMap = 0;

	if (diffuseMap || specularMap || emissiveMap || normalMap) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if (diffuseMap) {
			glGenTextures(1, &current->next->mdata->diffuseMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->diffuseMap);
			SDL_Surface *surface = IMG_Load(diffuseMap);
			for (int i = 0; i < 6; i++) {
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);	
			}
			SDL_FreeSurface(surface);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		if (specularMap) {
			glGenTextures(1, &current->next->mdata->specularMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->specularMap);
			SDL_Surface *surface = IMG_Load(specularMap);
			for (int i = 0; i < 6; i++) {
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			}
			SDL_FreeSurface(surface);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		if (emissiveMap) {
			glGenTextures(1, &current->next->mdata->emissiveMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->emissiveMap);
			SDL_Surface *surface = IMG_Load(emissiveMap);
			for (int i = 0; i < 6; i++) {
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			}
			SDL_FreeSurface(surface);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		if (normalMap) {
			glGenTextures(1, &current->next->mdata->normalMap);
			glBindTexture(GL_TEXTURE_CUBE_MAP, current->next->mdata->normalMap);
			SDL_Surface *surface = IMG_Load(normalMap);
			for (int i = 0; i < 6; i++) {
				if (surface->format->BytesPerPixel == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
				else if (surface->format->BytesPerPixel == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
			}
			SDL_FreeSurface(surface);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	return current->next->mdata;
}


void materialListElementRemove(struct materialList *root, struct materialSettings *mdata)
{
	if (!root->next)
		return;

	struct materialList *current = root->next;
	while (current->mdata != mdata) {
		if (!current->next)
			return;
		current = current->next;
	}
	current->prev->next = current->next;
	if (current->next)
		current->next->prev = current->prev;

	if (current->mdata->diffuseMap)
		glDeleteTextures(1, &current->mdata->diffuseMap);
	if (current->mdata->specularMap)
		glDeleteTextures(1, &current->mdata->specularMap);
	if (current->mdata->emissiveMap)
		glDeleteTextures(1, &current->mdata->emissiveMap);
	if (current->mdata->normalMap)
		glDeleteTextures(1, &current->mdata->normalMap);
	
	free(current->mdata);
	free(current);
}

unsigned materialListSizeGet(struct materialList *root)
{
	unsigned counter = 0;
	struct materialList *current = root;
	while (current->next) {
		current = current->next;
		counter++;
	}

	return counter;
}

struct materialSettings *materialListElementGet(struct materialList *root, unsigned num)
{
	struct materialList *current = root;
	while (current->next && num) {
		current = current->next;
		num--;
	}

	if (!current->next)
		return NULL;

	return current->next->mdata;
}

