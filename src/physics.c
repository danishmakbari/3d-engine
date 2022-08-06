#include <engine/physics/physics.h>


static struct drawList *drawlist;
static struct lightList *lightlist;
static struct physicsData physics_data;

void physicsEngineInit(void)
{
	dInitODE();
	physics_data.world = dWorldCreate();
	dWorldSetGravity(physics_data.world, 0, -9.8, 0);
	dWorldSetERP(physics_data.world, 0.45);
	dWorldSetCFM(physics_data.world, 1e-9);
	dWorldSetDamping(physics_data.world, 0.005, 0.005);
	dWorldSetMaxAngularSpeed(physics_data.world, 600);
	dWorldSetContactSurfaceLayer(physics_data.world, 0.001);

	physics_data.space = dHashSpaceCreate(0);
	physics_data.contactgroup = dJointGroupCreate(0);

	drawlist = drawListHeadCreate();
	lightlist = lightListHeadCreate();
}

void physicsEngineCleanup(void)
{
	lightListHeadDestroy(lightlist);
	drawListHeadDestroy(drawlist);
	dJointGroupDestroy(physics_data.contactgroup);
	dSpaceDestroy(physics_data.space);
	dWorldDestroy(physics_data.world);
	dCloseODE();
}

struct drawList *physicsDrawListGet(void)
{
	return drawlist;
}

void physicsObjectDestroy(struct primitiveData *pdata)
{
	drawListElementRemove(drawlist, pdata);
}

void physicsAllObjectsDestroy(void)
{
	drawListElementsDestroy(drawlist);
}

struct lightList *physicsLightListGet(void)
{
	return lightlist;
}

void physicsLightDestroy(struct lightSettings *ldata)
{
	lightListElementRemove(lightlist, ldata);
}

void physicsAllLightsDestroy(void)
{
	lightListElementsDestroy(lightlist);
}

struct lightSettings *physicsDirectionLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	return directionLightCreate(lightlist, position, ambient, diffuse, specular);
}

struct lightSettings *physicsDirectionShadowLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, GLsizei smsize, float near, float far, float csmLambda)
{
	return directionShadowLightCreate(lightlist, position, ambient, diffuse, specular, smsize, near, far, csmLambda);
}

struct lightSettings *physicsPointLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	return pointLightCreate(lightlist, position, ambient, diffuse, specular);
}

struct lightSettings *physicsPointShadowLightCreate(glm::vec3 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, GLsizei smsize, float near, float far)
{
	return pointShadowLightCreate(lightlist, position, ambient, diffuse, specular, smsize, near, far);
}

struct lightSettings *physicsSpotLightCreate(glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular)
{
	return spotLightCreate(lightlist, position, direction, cutoff, ambient, diffuse, specular);
}

struct lightSettings *physicsSpotShadowLightCreate(glm::vec3 position, glm::vec3 direction, float cutoff, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
	GLsizei smsize, float near, float far)
{
	return spotShadowLightCreate(lightlist, position, direction, cutoff, ambient, diffuse, specular, smsize, near, far);
}

static void nearCallback(void *data, dGeomID geom1, dGeomID geom2)
{
	struct physicsData *physics_data = (struct physicsData *) data;

	dBodyID body1 = dGeomGetBody(geom1);
	dBodyID body2 = dGeomGetBody(geom2);

	const int MAX_CONTACTS = 8;
	dContact contacts[MAX_CONTACTS];

	int numc = dCollide(geom1, geom2, MAX_CONTACTS, &contacts->geom, sizeof(dContact));
	for (int i = 0; i < numc; i++) {
		contacts[i].surface.mode = dContactApprox1 | dContactSoftERP;
		contacts[i].surface.mu = dInfinity;
		contacts[i].surface.soft_erp = 0.45;

		dJointID contact = dJointCreateContact(physics_data->world, physics_data->contactgroup, &contacts[i]);

		dJointAttach(contact, body1, body2);
	}
}

void physicsEngineMakeStep(void)
{
	dSpaceCollide(physics_data.space, &physics_data, nearCallback);
	dWorldQuickStep(physics_data.world, STEPSIZE);
	dJointGroupEmpty(physics_data.contactgroup);
}

struct physicsData *physicsEngineDataGet(void)
{
	return &physics_data;
}


