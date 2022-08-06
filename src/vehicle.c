#include <engine/vehicle/vehicle.h>

void vehicleConstruct(struct vehicleData *vehicle_data)
{
	struct physicsData *physics_data = physicsEngineDataGet();

	vehicle_data->jointgroup = dJointGroupCreate(0);
	vehicle_data->steering_joints = (dJointID *) malloc(vehicle_data->steering_num * sizeof(dJointID));
	vehicle_data->simple_joints = (dJointID *) malloc(vehicle_data->simple_num * sizeof(dJointID));

	for (int i = 0; i < vehicle_data->steering_num; i++) {
		vehicle_data->steering_joints[i] = dJointCreateHinge2(physics_data->world, vehicle_data->jointgroup);
		dJointAttach(vehicle_data->steering_joints[i], dGeomGetBody(vehicle_data->vehicle_geom), dGeomGetBody(vehicle_data->steering_geoms[i]->geom));

		const dReal *geom_pos = dGeomGetPosition(vehicle_data->steering_geoms[i]->geom);
		dJointSetHinge2Anchor(vehicle_data->steering_joints[i], geom_pos[0], geom_pos[1], geom_pos[2]);

		const dReal temp1[] = {0, 1, 0};
		const dReal temp2[] = {0, 0, 1};
		dJointSetHinge2Axes(vehicle_data->steering_joints[i], temp1, temp2);
		dJointSetHinge2Param(vehicle_data->steering_joints[i], dParamLoStop, 0);
		dJointSetHinge2Param(vehicle_data->steering_joints[i], dParamHiStop, 0);
	}
	for (int i = 0; i < vehicle_data->simple_num; i++) {
		vehicle_data->simple_joints[i] = dJointCreateHinge2(physics_data->world, vehicle_data->jointgroup);
		dJointAttach(vehicle_data->simple_joints[i], dGeomGetBody(vehicle_data->vehicle_geom), dGeomGetBody(vehicle_data->simple_geoms[i]->geom));

		const dReal *geom_pos = dGeomGetPosition(vehicle_data->simple_geoms[i]->geom);
		dJointSetHinge2Anchor(vehicle_data->simple_joints[i], geom_pos[0], geom_pos[1], geom_pos[2]);

		const dReal temp1[] = {0, 0, 0};
		const dReal temp2[] = {0, 0, 1};
		dJointSetHinge2Axes(vehicle_data->simple_joints[i], temp1, temp2);
		dJointSetHinge2Param(vehicle_data->simple_joints[i], dParamLoStop, 0);
		dJointSetHinge2Param(vehicle_data->simple_joints[i], dParamHiStop, 0);
	}
}

void vehicleDeconstruct(struct vehicleData *vehicle_data)
{
	dJointGroupDestroy(vehicle_data->jointgroup);
	free(vehicle_data->steering_joints);
	free(vehicle_data->simple_joints);
}
