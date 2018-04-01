#include "ray.h"
#include "main.h"

ray generateViewport(int xP, int yP) {
  ray res;
  float x = (float)(2*xP - XRES)/XRES * ASPECT;
  float y = (float)(2*yP - YRES)/YRES;
  res.dir = vec3(x, y, 1.f);
  res.dir.normalize();
  res.origin = vec3(0.f, 0.f, 0.f);
  return res;
}