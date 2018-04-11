#ifndef SCENE_SPHERE
#define SCENE_SPHERE

#include <cstdint>
#include <array>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "main.hpp"
#include "ray.hpp"
#include "commons.hpp"
#include "simplex_noise.hpp"

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
struct Ball : Sphere {
  uint32_t color;
  float intensity = 0.f; // light intensity

  Ball() : Sphere() {
    color = 0xFAFAFA;
  }
  uint32_t get_color(float x, float y, float z) {
    return color;
  }
};

struct Planet : Sphere {
  SimplexNoise texture;
  enum ColorMap { grayscale, hotcold, terrain, moon, lava, sun };
  ColorMap cm = ColorMap::moon;

  float getTex(float x, float y, float z) {
    return texture.noise(x, y, z); 
  }
  uint32_t get_color(float x, float y, float z) {
    float colorVal = getTex(x, y, z);
    return colorMap(colorVal/2 + 0.5);
  }
  uint32_t colorMap(float f) {
    switch (cm) {
      case grayscale: return cm_grayscale(f);
      case terrain: return cm_terrain(f);
      case hotcold: return cm_hot_cold(f);
      case lava: return cm_hot_cold(f/2 + 0.5f);
      case moon: return cm_grayscale(f/2 + 0.3f);
      case sun: return cm_hot_cold(f/8 + 0.6f);
      default: return cm_grayscale(f);
    }
  }
  // get UV coordinates
  uv getPolar(vec3 point) {
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
  light(vec3 pos) : vec3(pos) {
    intensity = 1.f;
  }
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
