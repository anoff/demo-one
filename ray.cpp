#include "ray.h"
#include "main.h"

ray generateViewport(int xP, int yP, ray camera) {
  ray res;
  vec3 up(0, -1, 0);
  vec3 p = camera.dir.getOrthogonal(up);
  p.normalize();
  
  float x = (float)(2*xP - XRES)/XRES * ASPECT;
  float y = (float)(2*yP - YRES)/YRES;
  res.dir = p*x + up*y + camera.dir;
  res.dir.normalize();
  res.origin = camera.origin;
  return res;
}