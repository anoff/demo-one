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
  void normalize() {
    float d = sqrt(x*x + y*y + z*z);
    if (d > 0) {
      x /= d;
      y /= d;
      z /= d;
    } else {
      x = y = z = 0;
    }
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
};
struct ray {
  vec3 origin;
  vec3 dir;
};
struct sphere {
  vec3 center;
  float radius;

  // check for intersection with ray, return distance units or -1.f
  //  http://kylehalladay.com/blog/tutorial/math/2013/12/24/Ray-Sphere-Intersection.html
  float intersect(ray r) {
    float t; // response, # units from ray origin
    vec3 L = center - r.origin;
    float tc = L.dot(r.dir);
    if (tc < 0) return -1.0f;
    float d = sqrt(tc*tc - L.dot(L)*L.dot(L));
    if ( d > radius) return -1.0f;
    float t1c = sqrt(radius*radius - d*d);
    float t1 = tc - t1c;
	  float t2 = tc + t1c;

    return t1 < t2 ? t1 : t2;
  }
};
#endif