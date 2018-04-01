#ifndef SCENE_SPHERE
#define SCENE_SPHERE

#include <cstdint>
#include <array>
#include <cstdlib>
#include <cmath>

#include "main.h"
#include "ray.h"
#include "commons.h"
#include "perlin.h"

using std::uint32_t;

struct ball : sphere {
  uint32_t color;

  ball() : sphere() {
    color = 0xFAFAFA;
  }
};

struct uv {
  float u;
  float v;
  uv() {
    u = 0;
    v = 0;
  }
  uv(float u, float v) {
    this->u = u;
    this->v = v;
  }
};

struct planet : sphere {
  Perlin texture;
  planet init() {
    texture.initPerlin();
    return *this;
  }
  // get UV 
  uv getUV(vec3 point) {
    vec3 d = this->center - point;
    d.normalize();
    float u = 0.5f + atan2(d.z, d.x) / (2 * M_PI);
    float v = 0.5f - asin(d.y) / M_PI;
    return uv(u, v);
  }
  float get_texture(float x, float y) {
    return texture.getPerlin(x, y);
  }
};
struct light : vec3 {
  float intensity;
  light(float x0, float y0, float z0) : vec3(x0, y0, z0) {
    intensity = 1.f;
  }
  light(float x0, float y0, float z0, float intens) : vec3(x0, y0, z0) {
    intensity = intens;
  }
  light(float intens) : vec3(0, 0, 0) {
    intensity = intens;
  }
  light() : vec3() {
    intensity = 1.f;
  }
};

void scene_sphere_init();
void scene_sphere_do(SDL_Surface *surface, int delta, int cnt);
#endif
