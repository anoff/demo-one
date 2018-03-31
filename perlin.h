#ifndef PERLIN_H
#define PERLIN_H

#include "demo.h"
#include <cmath>
#include <cstdlib>

float lerp(float a0, float a1, float w);
float dotGridGradient(int ix, int iy, float x, float y);
class Perlin {
  public:
  float Gradient[YRES][XRES][2];
  void initPerlin();
  float getPerlin(float x, float y);
  float dotGridGradient(int ix, int iy, float x, float y);
};
#endif
