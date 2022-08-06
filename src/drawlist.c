#include <engine/drawlist/drawlist.h>

struct drawList *drawListHeadCreate(void)
{
	struct drawList *root = (struct drawList *) malloc(sizeof(struct drawList));
	root->pdata = NULL;
	root->next = NULL;
	root->prev = NULL;
	return root;
}

void drawListElementsDestroy(struct drawList *root)
{
	struct drawList *current = root->next;
	root->next = NULL;

	while (current) {
		switch (dGeomGetClass(current->pdata->geom)) {
		case dSphereClass:
			sphereDestroy(current->pdata);
			break;
		case dBoxClass:
			boxDestroy(current->pdata);
			break;
		case dCylinderClass:
			cylinderDestroy(current->pdata);
			break;
		case dHeightfieldClass:
			terrainDestroy(current->pdata);
			break;
		}

		struct drawList *temp = current->next;
		free(current);
		current = temp;
	}
}


void drawListHeadDestroy(struct drawList *root)
{
	drawListElementsDestroy(root);
	free(root);
}

void drawListElementAdd(struct drawList *root, struct primitiveData *pdata)
{
	struct drawList *current = root;
	while (current->next)
		current = current->next;
	current->next = (struct drawList *) malloc(sizeof(struct drawList));
	current->next->next = NULL;
	current->next->prev = current;
	current->next->pdata = pdata;
}

void drawListElementRemove(struct drawList *root, struct primitiveData *pdata)
{
	if (!root->next)
		return;

	struct drawList *current = root->next;
	while (current->pdata != pdata) {
		if (!current->next)
			return;
		current = current->next;
	}

	switch (dGeomGetClass(current->pdata->geom)) {
	case dSphereClass:
		sphereDestroy(current->pdata);
		break;
	case dBoxClass:
		boxDestroy(current->pdata);
		break;
	case dCylinderClass:
		cylinderDestroy(current->pdata);
		break;
	case dHeightfieldClass:
		terrainDestroy(current->pdata);
		break;
	}

	current->prev->next = current->next;
	if (current->next)
		current->next->prev = current->prev;
	free(current);
}

unsigned drawListSizeGet(struct drawList *root)
{
	unsigned counter = 0;
	struct drawList *current = root;
	while (current->next) {
		current = current->next;
		counter++;
	}

	return counter;
}

struct primitiveData *drawListElementGet(struct drawList *root, unsigned num)
{
	struct drawList *current = root;
	while (current->next && num) {
		current = current->next;
		num--;
	}

	if (!current->next)
		return NULL;

	return current->next->pdata;
}

