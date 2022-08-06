
function printInfo(font, color)
	local cameraPos = cameraPosGet()
	local textPos = {0, 0}
	textPos = gprintf(font, color, textPos, "vendor: %s\n", gpu_vendor())
	textPos = gprintf(font, color, textPos, "gpu: %s\n", gpu_model())
	textPos = gprintf(font, color, textPos, "api: %s\n", opengl_version())
	textPos = gprintf(font, color, textPos, "shaders: %s\n", glsl_version())
	textPos = gprintf(font, color, textPos, "memory usage: %.2f/%.2f mb\n", nv_gpu_memusage_mb(), nv_gpu_memmax_mb())
	textPos = gprintf(font, color, textPos, "fps: %.2f\n", fps())
	textPos = gprintf(font, color, textPos, "frametime: %d ms\n", frametime())
	textPos = gprintf(font, color, textPos, "position: x = %.2f y = %.2f z = %.2f\n", cameraPos[1], cameraPos[2], cameraPos[3])
end

function main()
	windowInit("My super engine =)", 1920, 1080)
	windowModeSet("windowmode_fullscreen")
	windowRelativeCursorSet(true)
	physicsEngineInit()
	graphicsInit(45, 0.1, 100000)

	local skybox = skyboxCreate("assets/right.png", "assets/left.png", "assets/top.png", "assets/bottom.png", "assets/front.png", "assets/back.png", {2, 2, 2})
	local font = fontRender("assets/OpenSans-Regular.ttf", 12)

	directionShadowLightCreate({1, 1, 0}, {0.1, 0.1, 0.1, 0}, {1, 1, 1, 0}, {1, 1, 1, 0}, 4096, 0.1, 5000, 0.6)
	
	--pointShadowLightCreate({0, 30, 0}, {0.1, 0.1, 0.1, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, 4096, 0.1, 200)
	--pointShadowLightCreate({100, 30, 0}, {0.1, 0.1, 0.1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, 4096, 0.1, 200)

	--spotShadowLightCreate({0, 10, 0}, {0, -10, -0.1}, 20, {0.1, 0.1, 0.1, 0}, {1, 1, 1, 0}, {1, 1, 1, 0}, 4096, 0.1, 100)

	local material1 = materialListHeadCreate()
	materialList2DElementAdd(material1, {1, 1, 1, 1}, {0, 0, 0, 0}, 256, {0, 0, 0, 0}, "assets/grounddiff.jpg", nil, nil, "assets/groundnorm.jpg")
	materialList2DElementAdd(material1, {1, 1, 1, 1}, {0, 0, 0, 0}, 256, {0, 0, 0, 0}, "assets/ground.jpg", nil, nil, nil)
	
	local material2 = materialListHeadCreate()
	materialListCube2DElementAdd(material2, {1, 1, 1, 1}, {0, 0, 0, 0}, 256, {0, 0, 0, 0}, "assets/grounddiff.jpg", nil, nil, "assets/groundnorm.jpg")

	local earth_imgs = {"assets/px.png", "assets/nx.png", "assets/py.png", "assets/ny.png", "assets/pz.png", "assets/nz.png"}
	local earth_material = materialListHeadCreate()
	materialListCubeElementAdd(earth_material, {1, 1, 1, 1}, {1, 1, 1, 1}, 128, {0, 0, 0, 0}, earth_imgs, nil, nil, nil)

	local material3 = materialListHeadCreate()
	materialListCube2DElementAdd(material3, {1, 1, 1, 1}, {1, 1, 1, 1}, 128, {0, 0, 0, 0}, "assets/boxdiff.png", "assets/boxspec.png", nil, nil)

	terrainCreate(material1, "assets/hm.png", {100, 1, 100}, true, false)

	for i = 0, 100 do
		sphereCreate(material2, 5, 10, {0, 0 + i * 10, 0}, true, false)
		boxCreate(material2, 10, {20, 10, 20}, {0, 0 + i * 30, 0}, true, false)
		--cylinderCreate(material1, 4, 5, 5, {0, 0 + i * 10, 0}, true, false)
	end

	cameraEyeSet({0, 0, 0})
	cameraCenterSet({0, 100, 0})
	
	while windowEventsHandle() do
		physicsEngineMakeStep()

		freeroamCameraMove(3)

		drawShadows()
		drawScene(skybox)

		printInfo(font, {0, 1, 0, 1})

		windowSwapFramebuffers()

		openglErrorsHandle()
	end

	skyboxDestroy(skybox)
	fontDestroy(font);
	
	materialListHeadDestroy(material1)
	materialListHeadDestroy(material2)

	windowCleanup()
	physicsEngineCleanup()

	openglErrorsHandle()

	print("\nHello world!\n");

end

main()

