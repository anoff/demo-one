#ifndef MAIN_H
#define MAIN_H
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#define XRES 800
#define YRES 600
const float ASPECT = (float)XRES/YRES;

#endif