#ifndef WINDOW_H
#define WINDOW_H

#define WINDOWMODE_WINDOWED 0
#define WINDOWMODE_FULLSCREEN 1
#define WINDOWMODE_WINDOWED_BORDERLESS 2

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <engine/camera/camera.h>
#include <engine/mvp/mvp.h>

void windowInit(const char *windowName, int width, int height);
bool windowEventsHandle(void);
void windowCleanup(void);

void windowRelativeCursorSet(bool mode);

void windowModeSet(int flag);

void windowTitleSet(const char *title);

void windowSizeSet(int width, int height);

int windowWidthGet(void);

int windowHeightGet(void);

void windowSwapFramebuffers(void);

#endif

