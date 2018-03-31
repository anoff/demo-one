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

void addPerlinGradient(Perlin &p1, Perlin &p2) {
	for (int y = 0; y < YRES; y++) {
		for (int x = 0; x < XRES; x++) {
			float v0 = p1.Gradient[y][x][0] + 0.1 * p2.Gradient[y + 13][x + 13][0];
			float v1 = p1.Gradient[y][x][1] + 0.1 * p2.Gradient[y + 13][x + 13][1];
			float d = sqrt(v0*v0 + v1*v1);
			if (d > 0) {
				p1.Gradient[y][x][0] = v0/d;
				p1.Gradient[y][x][1] = v1/d;
			}
		}
	}
}

int cnt = 0;
int spotCenter[] = {0, 0};
int spotRadius = XRES/10;
void demo_do(SDL_Surface *surface, int delta) {
	cnt++;
	spotRadius = sin((float)cnt/10)*70 + 100;
	spotCenter[0] += 50;
	if (spotCenter[0] > XRES) {
		spotCenter[0] = 0;
		spotCenter[1] += spotRadius;
	}
	if (spotCenter[1] > YRES) {
		spotCenter[0] = 0;
		spotCenter[1] = 0;
	}
	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			int radius = sqrt(pow(abs(x - spotCenter[0]), 2) + pow(abs(y - spotCenter[1]), 2));
			if (radius < spotRadius) {
				float val = baseP.getPerlin((x + XRES/4)/(float)(50), (y + YRES/4)/(float)(50));
				put_pixel32(surface, x, y, hot_cold(val + .5));
			} else {
				put_pixel32(surface, x, y, 0x0);
			}
		}
	}
	addPerlinGradient(baseP, velP);
	if (cnt % (int)(200 / delta) == 0) {
		velP.initPerlin();
	}
}
