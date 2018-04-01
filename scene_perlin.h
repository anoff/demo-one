#ifndef SCENE_PERLIN
#define SCENE_PERLIN

#include "main.h"
#include <cstdint>
#include <array>
#include <cstdlib>
#include <cmath>
using std::uint32_t;

void scene_perlin_init();
void scene_perlin_do(SDL_Surface *surface, int delta);

#define SPOTS_MAX 5
#define GLOW_RADIUS 30
#include "perlin.h"
#endif
