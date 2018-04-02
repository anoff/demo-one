#ifndef SCENE_SPHERE
#define SCENE_SPHERE

#include <cstdint>
#include <array>
#include <cstdlib>
#include <cmath>

#include "main.h"
#include "ray.h"
#include "commons.h"
#include "simplex_noise.h"

using std::uint32_t;

#define PERL_TEX_SIZE 1000

struct uv {
  float u;
  float v;
  uv() {
    u = 0;
    v = 0;
  }
};
struct ball : sphere {
  uint32_t color;

  ball() : sphere() {
    color = 0xFAFAFA;
  }
};

struct planet : sphere {
  SimplexNoise* texture = nullptr;
  ~planet() {
   if (texture != nullptr) {
     delete texture;
     texture = nullptr; 
   }
  }
  void init() {
    texture = new SimplexNoise();
  }
  float getTex(float x, float y, float z) {
    return texture->noise(x, y, z); 
  }
  // get UV 
  uv getUV(vec3 point) {
    uv res;
    vec3 d = this->center - point;
    d.normalize();
    float u = 0.5f + atan2(d.z, d.x) / (2 * M_PI);
    float v = 0.5f - asin(d.y) / M_PI;
    res.u = u;
    res.v = v;
    return res;
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
