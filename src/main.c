#include <engine/main/main.h>

int main(int argc, char **argv)
{
	luaInit();
	
	luaFileExecute("main.lua");
	
	luaCleanup();

	return 0;

/*
	windowInit("My super engine =)", 1920, 1080);
	windowModeSet(WINDOWMODE_FULLSCREEN);
	windowRelativeCursorSet(true);
	physicsEngineInit();
	graphicsInit(45, 0.1, 10000);

	struct skyboxData *skybox = skyboxCreate("assets/right.png", "assets/left.png", "assets/top.png", "assets/bottom.png", "assets/front.png", "assets/back.png", glm::vec3(2));
	struct fontData *fdata = fontRender("assets/OpenSans-Regular.ttf", 12);
	struct drawList *drawlist1 = drawListHeadCreate();
	struct lightList *lightlist1 = lightListHeadCreate();
	lightListElementAdd(lightlist1, glm::vec4(-1, 1, 0, 0), glm::vec4(0.01, 0.01, 0.01, 0), glm::vec4(1, 1, 1, 0), glm::vec4(1, 1, 1, 0), true, 4096, 0.1, 1000);
	//lightListElementAdd(lightlist1, glm::vec4(-500, 5, 0, 1), glm::vec4(0.01, 0.01, 0.01, 0), glm::vec4(1, 1, 1, 0), glm::vec4(1, 1, 1, 0), true, 4096, 0.1, 100);
	//lightListElementAdd(lightlist1, glm::vec4(500, 50, 0, 1), glm::vec4(0.01, 0.01, 0.01, 0), glm::vec4(1, 1, 1, 0), glm::vec4(1, 1, 1, 0), true, 4096, 0.1, 100);
	
	struct materialList *material1 = materialListHeadCreate();
	materialList2DElementAdd(material1, glm::vec4(1), glm::vec4(0), 256, glm::vec4(0), "assets/ground.jpg", NULL, NULL, NULL);
	
	terrainCreate(material1, lightlist1, "assets/terrain.jpg", glm::vec3(100, 1, 100), true, false);
	struct materialList *material2 = materialListHeadCreate();
	materialListCube2DElementAdd(material2, glm::vec4(1), glm::vec4(0), 256, glm::vec4(0), "assets/brick.jpg", NULL, NULL, "assets/normal.jpg");
	
	sphereCreate(material2, lightlist1, 5, 50, glm::vec3(0, 500, -100), true, false);
	boxCreate(material2, lightlist1, 1, glm::vec3(5, 5, 5), glm::vec3(0, 500, -200), true, false);
	for (int i = 0; i < 150; i++) {
		cylinderCreate(material1, lightlist1, 4, 5, 5, glm::vec3(i * 15, 500 + i * 10, -300), true, false);
	}
	struct primitiveData *tower = boxCreate(material2, lightlist1, 0, glm::vec3(20, 20, 20), glm::vec3(0, 475, 0), true, false);
	dBodySetKinematic(tower->body);
	struct primitiveData *carbox = boxCreate(material2, lightlist1, 100, glm::vec3(5, 1, 2), glm::vec3(0, 500, -4), true, false);

	struct primitiveData *steering_geoms[2], *simple_geoms[2];
	steering_geoms[0] = cylinderCreate(material1, lightlist1, 10, 0.8, 0.55, glm::vec3(-2, 500, -2.5), true, false);
	steering_geoms[1] = cylinderCreate(material1, lightlist1, 10, 0.8, 0.55, glm::vec3(-2, 500, -5.5), true, false);
	simple_geoms[0] = cylinderCreate(material1, lightlist1, 10, 1, 0.85, glm::vec3(2, 500, -2.5), true, false);
	simple_geoms[1] = cylinderCreate(material1, lightlist1, 10, 1, 0.85, glm::vec3(2, 500, -5.5), true, false);

	struct vehicleData vehicle_data;
	vehicle_data.vehicle_geom = carbox->geom;
	vehicle_data.jointgroup = 0;
	vehicle_data.steering_joints = NULL;
	vehicle_data.steering_geoms = steering_geoms;
	vehicle_data.steering_num = 2;
	vehicle_data.fmax1 = 1000;
	vehicle_data.steer_delta = 5;
	vehicle_data.steer = 0;
	vehicle_data.lostop = glm::radians(-12.0);
	vehicle_data.histop = glm::radians(12.0);
	vehicle_data.simple_joints = NULL;
	vehicle_data.simple_geoms = simple_geoms;
	vehicle_data.simple_num = 2;
	vehicle_data.fmax2 = 500;
	vehicle_data.w_delta = 5;
	vehicle_data.s_delta = 1;
	vehicle_data.speed = 0;
	vehicle_data.w_lim = 25;
	vehicle_data.s_lim = 5;

	vehicleConstruct(&vehicle_data);

	while (windowEventsHandle()) {

		physicsEngineMakeStep();

		//moveVehicleThirdPersonCamera(glm::vec3(0, 2, 15), &vehicle_data);
		freeroamCameraMove(5);

		//cameraCenterSet(glm::vec3(0, 1000, 0));

		drawShadows(lightlist1, 0.85);
		drawScene(skybox);

		glm::vec3 cameraPos = viewMatrixCameraPosGet();
		glm::vec4 color = glm::vec4(0, 1, 0, 1);
		glm::vec2 textPos = glm::vec2(0, 0);
		textPos = gprintf(fdata, color, textPos, "vendor: %s\n", gpu_vendor());
		textPos = gprintf(fdata, color, textPos, "gpu: %s\n", gpu_model());
		textPos = gprintf(fdata, color, textPos, "api: %s\n", opengl_version());
		textPos = gprintf(fdata, color, textPos, "shaders: %s\n", glsl_version());
		textPos = gprintf(fdata, color, textPos, "memory usage: %.2f/%.2f mb\n", nv_gpu_memusage_mb(), nv_gpu_memmax_mb());
		textPos = gprintf(fdata, color, textPos, "fps: %.2f\n", fps());
		textPos = gprintf(fdata, color, textPos, "frametime: %u ms\n", frametime());
		textPos = gprintf(fdata, color, textPos, "position: x = %.2f y = %.2f z = %.2f\n", cameraPos.x, cameraPos.y, cameraPos.z);

		windowSwapFramebuffers();

		openglErrorsHandle();
	}

	skyboxDestroy(skybox);
	
	fontDestroy(fdata);

	vehicleDeconstruct(&vehicle_data);

	lightListHeadDestroy(lightlist1);

	openglErrorsHandle();

	physicsEngineCleanup();
	windowCleanup();
	graphicsCleanup();

	return 0;
*/
}

