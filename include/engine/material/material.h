#ifndef MATERIAL_H
#define MATERIAL_H

#define MAX_MATERIALS 2

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <engine/utility/utility.h>

struct materialList {
	struct materialSettings *mdata;
	struct materialList *next;
	struct materialList *prev;
};

struct materialSettings {
	glm::vec4 diffuse;
	glm::vec4 specular;
	GLfloat shininess;
	glm::vec4 emissive;

	GLuint diffuseMap;
	GLuint specularMap;
	GLuint emissiveMap;
	GLuint normalMap;
};

struct materialList *materialListHeadCreate(void);
void materialListElementsDestroy(struct materialList *root);
void materialListHeadDestroy(struct materialList *root);

struct materialSettings *materialList2DElementAdd(struct materialList *root, glm::vec4 diffuse,
	glm::vec4 specular, GLfloat shininess, glm::vec4 emissive, const char *diffuseMap, const char *specularMap, const char *emissiveMap, const char *normalMap);

struct materialSettings *materialListCubeElementAdd(struct materialList *root, glm::vec4 diffuse,
	glm::vec4 specular, GLfloat shininess, glm::vec4 emissive, const char *diffuseMap[6], const char *specularMap[6], const char *emissiveMap[6], const char *normalMap[6]);

struct materialSettings *materialListCube2DElementAdd(struct materialList *root, glm::vec4 diffuse,
	glm::vec4 specular, GLfloat shininess, glm::vec4 emissive, const char *diffuseMap, const char *specularMap, const char *emissiveMap, const char *normalMap);

void materialListElementRemove(struct materialList *root, struct materialSettings *mdata);
unsigned materialListSizeGet(struct materialList *root);
struct materialSettings *materialListElementGet(struct materialList *root, unsigned num);

#endif

