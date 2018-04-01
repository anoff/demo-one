#ifndef PERLIN_H
#define PERLIN_H

#include <cmath>
#include <cstdlib>

struct vec3 {
  float x;
  float y;
  float z;

  vec3() {
    x = y = z = 0;
  }
  vec3(float x0, float y0, float z0) {
    x = x0;
    y = y0;
    z = z0;
  }
  float length() {
    return sqrt(x*x + y*y + z*z);
  }
  vec3 normalize() {
    float d = sqrt(x*x + y*y + z*z);
    if (d > 0) {
      x /= d;
      y /= d;
      z /= d;
    } else {
      x = y = z = 0;
    }
    return *this;
  }
  // overload multiplication to change vector length
  vec3 operator*(float mult) {
    vec3 res;
    res.x = x * mult;
    res.y = y * mult;
    res.z = z * mult;
    return res;
  }
  // add two vectors
  vec3 operator+(vec3 v) {
    vec3 res;
    res.x = x + v.x;
    res.y = y + v.y;
    res.z = z + v.z;
    return res;
  }
  vec3 operator+=(vec3 v) {
    x = x+v.x;
    y = y+v.y;
    z = z+v.z;
    return *this;
  }
  // substract two vectors
  vec3 operator-(vec3 v) {
    vec3 res;
    res = *this + v*(-1);
    return res;
  }

  float dot(vec3 v) {
    float res = 0;
    res += x*v.x;
    res += y*v.y;
    res += z*v.z;
    return res;
  }

  vec3 getOrthogonal(vec3 v) {
    vec3 res;
    res.x = y*v.z - z*v.y;
    res.y = z*v.x - x*v.z;
    res.z = x*v.y - y*v.x;
    return res;
  }
};
struct ray {
  vec3 origin;
  vec3 dir;

  ray(vec3 o, vec3 d) {
    origin = o;
    dir = d;
  }
  ray() {
    origin = vec3(0, 0, 0);
    dir = vec3(0, 0, 0);
  }
};
struct sphere {
  vec3 center;
  float radius;

  // check for intersection with ray, return distance units or -1.f
  //  https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#Example
  float intersect(ray r) {
    vec3 L = r.origin - center;
    float tc = L.dot(r.dir);
    if (-tc < 0) return -1.0f;
    float d = L.dot(L) - radius*radius;
    if ( d < 0) return -1.0f;
    float t1c = sqrt(tc*tc - d);
    float t1 = -tc - t1c;
	  float t2 = -tc + t1c;

    return t1 < t2 ? t1 : t2;
  }
};

ray generateViewport(int xP, int yP, ray camera);
#endif