#include "demo.h"

// Precomputed (or otherwise) gradient vectors at each grid node
float Gradient[YRES][XRES][2];

void put_pixel32(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
	Uint32 *pixels = reinterpret_cast<uint32_t*>(surface->pixels);
	pixels[(y * surface->w) + x] = pixel;
}

// https://en.wikipedia.org/wiki/Perlin_noise#Pseudocode
// Function to linearly interpolate between a0 and a1
// Weight w should be in the range [0.0, 1.0]
float lerp(float a0, float a1, float w)
{
	return (1.0 - w)*a0 + w*a1;
}
// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y)
{


		// Compute the distance vector
		float dx = x - (float)ix;
		float dy = y - (float)iy;

		// Compute the dot-product
		return (dx*Gradient[iy][ix][0] + dy*Gradient[iy][ix][1]);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y)
{

		// Determine grid cell coordinates
		int x0 = floor(x);
		int x1 = x0 + 1;
		int y0 = floor(y);
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

		return value;
}

float clamp(float val, float low, float high)
{
	const float t = val < low ? low : val;
	return t > high ? high : t;
}
uint32_t color(const int r, const int g, const int b)
{
	return 0xff000000 | (r << 16) | (g << 8) | b;
}
uint32_t hot_cold(float f)
{
	clamp(f, 0.f, 1.f);
	f = (2.f * f) - 1.f;
	float r = clamp(1.5f - abs(2.f * f - 1.f), 0.f, 1.f);
	float g = clamp(1.5f - abs(2.f * f), 0.f, 1.f);
	float b = clamp(1.5f - abs(2.f * f + 1.f), 0.f, 1.f);
	return color(static_cast<int>(255.f * r), static_cast<int>(255.f * g), static_cast<int>(255.f * b));
}

void demo_init()
{
	for (int y = 0; y < YRES; y++) {
		for (int x = 0; x < XRES; x++) {
			float fx = rand() % 1000 - 500;
			float fy = rand() % 1000 - 500;
			float d = sqrt(fx*fx + fy*fy);
			fx /= d;
			fy /= d;
			Gradient[y][x][0] = fx;
			Gradient[y][x][0] = fy;
		}
	}
}

void demo_do(SDL_Surface *surface)
{
	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			float p = perlin(x/(float)33, y/(float)33);
			put_pixel32(surface, x, y, hot_cold(p + .5));
		}
	}
}