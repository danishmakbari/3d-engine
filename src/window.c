#include <engine/window/window.h>

static SDL_Window *window;
static SDL_GLContext glcontext;

void windowInit(const char *windowName, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	glcontext = SDL_GL_CreateContext(window);
	
	SDL_GL_SetSwapInterval(1);
}

bool windowEventsHandle(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			cameraAnglesAdjust(event.motion.xrel, event.motion.yrel);
			break;
		case SDL_QUIT:
			return false;
		}
	}

	return true;
}

void windowCleanup(void)
{
	SDL_GL_DeleteContext(glcontext);
	IMG_Quit();
	SDL_Quit();
}

void windowSizeSet(int width, int height)
{
	SDL_SetWindowSize(window, width, height);
	projMatrixPerspective(projMatrixPerspectiveFOVYGet(), (float) width / height, projMatrixPerspectiveNearGet(), projMatrixPerspectiveFarGet());
}

void windowTitleSet(const char *title)
{
	SDL_SetWindowTitle(window, title);
}

void windowModeSet(int flag)
{
	if (flag == WINDOWMODE_WINDOWED)
		SDL_SetWindowFullscreen(window, 0);
	else if (flag == WINDOWMODE_FULLSCREEN)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else if (flag == WINDOWMODE_WINDOWED_BORDERLESS)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void windowRelativeCursorSet(bool mode)
{
	if (mode)
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

int windowWidthGet(void)
{
	int width;
	SDL_GetWindowSize(window, &width, NULL);

	return width;
}

int windowHeightGet(void)
{
	int height;
	SDL_GetWindowSize(window, NULL, &height);

	return height;
}

void windowSwapFramebuffers(void)
{
	SDL_GL_SwapWindow(window);
}

