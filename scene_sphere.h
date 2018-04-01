#ifndef SCENE_SPHERE
#define SCENE_SPHERE

#include "main.h"
#include <cstdint>
#include <array>
#include <cstdlib>
#include <cmath>
using std::uint32_t;

void scene_sphere_init();
void scene_sphere_do(SDL_Surface *surface, int delta, int cnt);
#endif
