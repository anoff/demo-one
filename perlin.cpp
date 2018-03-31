#include "perlin.h"

// https://en.wikipedia.org/wiki/Perlin_noise#Pseudocode
// Function to linearly interpolate between a0 and a1
// Weight w should be in the range [0.0, 1.0]

float lerp(float a0, float a1, float w) {
  return (1.f - w)*a0 + w*a1;
}
// Computes the dot product of the distance and gradient vectors.
float Perlin::dotGridGradient(int ix, int iy, float x, float y) {
    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx*Gradient[iy][ix][0] + dy*Gradient[iy][ix][1]);
}

void Perlin::initPerlin() {
  for (int y = 0; y < YRES; y++) {
      for (int x = 0; x < XRES; x++) {
        float fx = rand() % 1000 - 500.f;
        float fy = rand() % 1000 - 500.f;
        float d = sqrt(fx*fx + fy*fy);
        fx /= d;
        fy /= d;
        Gradient[y][x][0] = fx;
        Gradient[y][x][1] = fy;
      }
    }
}

// Compute Perlin noise at coordinates x, y
float Perlin::getPerlin(float x, float y) {
    x = clamp(x, 0, XRES);
    y = clamp(y, 0, YRES);
    // Determine grid cell coordinates
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;
    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = lerp(n0, n1, sx);
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = lerp(n0, n1, sx);
    value = lerp(ix0, ix1, sy);

    /*if (x == round(x) || y == round(y)) {
      return 1;
    }*/
    return value;
}
