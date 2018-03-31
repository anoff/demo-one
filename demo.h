#ifndef DEMO_H
#define DEMO_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdint>
using std::uint32_t;

void demo_init();
void demo_do(SDL_Surface *surface);

#endif
