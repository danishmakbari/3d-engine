#ifndef VEHICLE_H
#define VEHICLE_H

#include <ode/ode.h>

#include <engine/3dprimitives/3dprimitives.h>

struct vehicleData {
	dGeomID vehicle_geom;
	dJointGroupID jointgroup;

	dJointID *steering_joints;
	struct primitiveData **steering_geoms;
	int steering_num;
	dReal fmax1;
	dReal steer_delta;
	dReal steer;
	dReal lostop;
	dReal histop;

	dJointID *simple_joints;
	struct primitiveData **simple_geoms;
	int simple_num;
	dReal fmax2;
	dReal w_delta;
	dReal s_delta;
	dReal speed;
	dReal w_lim;
	dReal s_lim;
};

void vehicleConstruct(struct vehicleData *vehicle_data);
void vehicleDeconstruct(struct vehicleData *vehicle_data);

#endif

