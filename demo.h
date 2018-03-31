#ifndef DEMO_H
#define DEMO_H


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdint>
#include <cstdlib>
#include <cmath>
using std::uint32_t;

void demo_init();
void demo_do(SDL_Surface *surface, int delta);
float clamp(float val, float low, float high);

#define XRES 800
#define YRES 600
const float ASPECT = (float)XRES/YRES;
#include "perlin.h"
#endif
