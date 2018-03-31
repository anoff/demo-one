#include "demo.h"

Perlin baseP;
Perlin velP;

void put_pixel32(SDL_Surface *surface, int x, int y, uint32_t pixel) {
	Uint32 *pixels = reinterpret_cast<uint32_t*>(surface->pixels);
	pixels[(y * surface->w) + x] = pixel;
}

float clamp(float val, float low, float high) {
	const float t = val < low ? low : val;
	return t > high ? high : t;
}
uint32_t color(const int r, const int g, const int b) {
	return 0xff000000 | (r << 16) | (g << 8) | b;
}
uint32_t hot_cold(float f) {
	clamp(f, 0.f, 1.f);
	f = (2.f * f) - 1.f;
	float r = clamp(1.5f - abs(2.f * f - 1.f), 0.f, 1.f);
	float g = clamp(1.5f - abs(2.f * f), 0.f, 1.f);
	float b = clamp(1.5f - abs(2.f * f + 1.f), 0.f, 1.f);
	return color(static_cast<int>(255.f * r), static_cast<int>(255.f * g), static_cast<int>(255.f * b));
}

void demo_init() {
	baseP.initPerlin();
	velP.initPerlin();
}

void addPerlinGradient() {
	for (int y = 0; y < YRES; y++) {
		for (int x = 0; x < XRES; x++) {
			baseP.Gradient[y][x][0] += velP.Gradient[y][x][0];
			baseP.Gradient[y][x][1] += velP.Gradient[y][x][1];
		}
	}
}

int cnt = 0;
void demo_do(SDL_Surface *surface, int delta) {
	cnt++;
	double zoom = 0*sin((float)cnt/10);
	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			float val = baseP.getPerlin((x + XRES/4 + zoom*ASPECT)/(float)(123 + zoom*30), (y + YRES/4 + zoom)/(float)(231 + zoom*30));
			put_pixel32(surface, x, y, hot_cold(val + .5));
		}
	}
	addPerlinGradient();
	if (cnt % 100) {
		velP.initPerlin();
	}
}
