#include <engine/camera/camera.h>

static float yaw = 0;
static float pitch = 0;
static float roll = 0;
static float sensitivity = 0.05;
static glm::vec3 eye = glm::vec3(0);
static glm::vec3 center = glm::vec3(0);

void cameraSensitivitySet(float s)
{
	sensitivity = s;
}

float cameraSensitivityGet(void)
{
	return sensitivity;
}

void cameraEyeSet(glm::vec3 e)
{
	eye = e;
}

glm::vec3 cameraEyeGet(void)
{
	return eye;
}

void cameraCenterSet(glm::vec3 c)
{
	center = c;
}

glm::vec3 cameraCenterGet(void)
{
	return center;
}

void cameraAnglesAdjust(Sint32 xrel, Sint32 yrel)
{
	yaw += xrel * sensitivity;
	pitch -= yrel * sensitivity;
	if (pitch > 90)
		pitch = 90;
	if (pitch < -90)
		pitch = -90;
}

void cameraUpdate(void)
{
	viewMatrixLoadIdentity();
	viewMatrixTranslate(-eye);
	viewMatrixRotate(pitch, glm::vec3(-1, 0, 0));
	viewMatrixRotate(yaw, glm::vec3(0, 1, 0));
	viewMatrixTranslate(-center);
}

void freeroamCameraMove(float speed)
{
	glm::vec3 forward = viewMatrixForwardVecGet();
	glm::vec3 right = viewMatrixRightVecGet();

	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard[SDL_SCANCODE_W]) {
		center -= forward * glm::vec3(speed);
	}

	if (keyboard[SDL_SCANCODE_S]) {
		center += forward * glm::vec3(speed);
	}

	if (keyboard[SDL_SCANCODE_A]) {
		center -= right * glm::vec3(speed);
	}

	if (keyboard[SDL_SCANCODE_D]) {
		center += right * glm::vec3(speed);
	}
}

/*
void moveThirdPersonCamera(glm::vec3 camera, struct velocityData *vel, dBodyID body)
{
	const dReal *bodypos = dBodyGetPosition(body);
	
	viewMatrixLoadIdentity();

	viewMatrixTranslate(-camera);
	viewMatrixRotate(pitch, glm::vec3(-1, 0, 0));
	viewMatrixRotate(yaw, glm::vec3(0, 1, 0));
	viewMatrixTranslate(glm::vec3(-bodypos[0], -bodypos[1], -bodypos[2]));

	glm::vec3 currentvel = viewMatrixGet() * glm::make_vec4(dBodyGetLinearVel(body));
	glm::vec3 forward = viewMatrixForwardVecGet();
	glm::vec3 right = viewMatrixRightVecGet();

	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard[SDL_SCANCODE_W] && currentvel[2] > -vel->w_lim) {
		dVector3 impulse, force;
		dMass mass;
		dBodyGetMass(body, &mass);
		for (int i = 0; i < 3; i++)
			impulse[i] = mass.mass * (-vel->w_delta) * forward[i];
		dWorldImpulseToForce(dBodyGetWorld(body), STEPSIZE, impulse[0], impulse[1], impulse[2], force);
		dBodySetForce(body, force[0], 0, force[2]);
	}

	if (keyboard[SDL_SCANCODE_S] && currentvel[2] < vel->s_lim) {
		dVector3 impulse, force;
		dMass mass;
		dBodyGetMass(body, &mass);
		for (int i = 0; i < 3; i++)
			impulse[i] = mass.mass * vel->s_delta * forward[i];
		dWorldImpulseToForce(dBodyGetWorld(body), STEPSIZE, impulse[0], impulse[1], impulse[2], force);
		dBodySetForce(body, force[0], 0, force[2]);
	}

	if (keyboard[SDL_SCANCODE_A] && currentvel[0] > -vel->a_lim) {
		dVector3 impulse, force;
		dMass mass;
		dBodyGetMass(body, &mass);
		for (int i = 0; i < 3; i++)
			impulse[i] = mass.mass * (-vel->a_delta) * right[i];
		dWorldImpulseToForce(dBodyGetWorld(body), STEPSIZE, impulse[0], impulse[1], impulse[2], force);
		dBodySetForce(body, force[0], 0, force[2]);
	}

	if (keyboard[SDL_SCANCODE_D] && currentvel[0] < vel->d_lim) {
		dVector3 impulse, force;
		dMass mass;
		dBodyGetMass(body, &mass);
		for (int i = 0; i < 3; i++)
			impulse[i] = mass.mass * vel->d_delta * right[i];
		dWorldImpulseToForce(dBodyGetWorld(body), STEPSIZE, impulse[0], impulse[1], impulse[2], force);
		dBodySetForce(body, force[0], 0, force[2]);
	}
}


void moveVehicleThirdPersonCamera(glm::vec3 camera, struct vehicleData *vehicle_data)
{
	const dReal *geompos = dGeomGetPosition(vehicle_data->vehicle_geom);

	viewMatrixLoadIdentity();
	viewMatrixTranslate(-camera);
	viewMatrixRotate(pitch, glm::vec3(-1, 0, 0));
	viewMatrixRotate(yaw, glm::vec3(0, 1, 0));
	viewMatrixTranslate(glm::vec3(-geompos[0], -geompos[1], -geompos[2]));

	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
	if (keyboard[SDL_SCANCODE_W]) {
		if (vehicle_data->speed > 0)
			vehicle_data->speed = 0;
		vehicle_data->speed -= vehicle_data->w_delta;
	} else if (keyboard[SDL_SCANCODE_S]) {
		if (vehicle_data->speed < 0)
			vehicle_data->speed = 0;
		vehicle_data->speed += vehicle_data->s_delta;
	} else if (!keyboard[SDL_SCANCODE_W] && !keyboard[SDL_SCANCODE_S]) {
		vehicle_data->speed = 0;
	}

	if (keyboard[SDL_SCANCODE_A]) {
		if (vehicle_data->steer > 0)
			vehicle_data->steer = 0;
		vehicle_data->steer -= vehicle_data->steer_delta;
	} else if (keyboard[SDL_SCANCODE_D]) {
		if (vehicle_data->steer < 0)
			vehicle_data->steer = 0;
		vehicle_data->steer += vehicle_data->steer_delta;
	} else if (!keyboard[SDL_SCANCODE_A] && !keyboard[SDL_SCANCODE_D]) {
		vehicle_data->steer = 0;
	}

	for (int i = 0; i < vehicle_data->steering_num; i++) {
		dReal v = vehicle_data->steer - dJointGetHinge2Angle1(vehicle_data->steering_joints[i]);
		if (v > 0.1)
			v = 0.1;
		if (v < -0.1)
			v = -0.1;
		v *= 10.0;
		dJointSetHinge2Param(vehicle_data->steering_joints[i], dParamVel, v);
		dJointSetHinge2Param(vehicle_data->steering_joints[i], dParamFMax, vehicle_data->fmax1);
		dJointSetHinge2Param(vehicle_data->steering_joints[i], dParamLoStop, vehicle_data->lostop);
		dJointSetHinge2Param(vehicle_data->steering_joints[i], dParamHiStop, vehicle_data->histop);
		dJointSetHinge2Param(vehicle_data->steering_joints[i], dParamFudgeFactor, 0.1);
	}

	for (int i = 0; i < vehicle_data->simple_num; i++) {
		if (vehicle_data->speed > vehicle_data->s_lim)
			vehicle_data->speed = vehicle_data->s_lim;
		if (vehicle_data->speed < -vehicle_data->w_lim)
			vehicle_data->speed = -vehicle_data->w_lim;
		dJointSetHinge2Param(vehicle_data->simple_joints[i], dParamVel2, vehicle_data->speed);
		dJointSetHinge2Param(vehicle_data->simple_joints[i], dParamFMax2, vehicle_data->fmax2);
		dJointSetHinge2Param(vehicle_data->simple_joints[i], dParamLoStop, 0);
		dJointSetHinge2Param(vehicle_data->simple_joints[i], dParamHiStop, 0);
	}
}
*/

