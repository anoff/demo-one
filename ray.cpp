#include "ray.h"
#include "main.h"

ray generateViewport(int xP, int yP) {
  ray res;
  float x = (float)(xP - XRES/2)/XRES;
  float y = (float)(yP - YRES/2)/YRES;
  res.dir = vec3(x, y, 1.f);
  res.origin = vec3(0.f, 0.f, 0.f);
  return res;
}