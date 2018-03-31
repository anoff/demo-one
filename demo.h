#ifndef DEMO_H
#define DEMO_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdint>
#include <cstdlib>
using std::uint32_t;

void demo_init();
void demo_do(SDL_Surface *surface);

#define XRES 800
#define YRES 600
const float ASPECT = (float)XRES/YRES;

#endif
