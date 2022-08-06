#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ode/ode.h>

#include <SDL2/SDL.h>

#include <engine/main/main.h>
#include <engine/vehicle/vehicle.h>
#include <engine/mvp/mvp.h>

void cameraAnglesAdjust(Sint32 xrel, Sint32 yrel);
void cameraUpdate(void);
void cameraSensitivitySet(float sensitivity);
float cameraSensitivityGet(void);
void cameraEyeSet(glm::vec3 eye);
glm::vec3 cameraEyeGet(void);
void cameraCenterSet(glm::vec3 center);
glm::vec3 cameraCenterGet(void);

void freeroamCameraMove(float speed);

/*
void moveThirdPersonCamera(glm::vec3 camera, struct velocityData *vel, dBodyID body);
void moveVehicleThirdPersonCamera(glm::vec3 camera, struct vehicleData *vehicle_data);
*/

#endif

