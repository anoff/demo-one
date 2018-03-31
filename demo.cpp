#include "demo.h"

#define SPOT_COUNT 5

Perlin baseP;
Perlin velP;
struct spot {
	int x;
	int y;
	int radius;
} spots[SPOT_COUNT];

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

std::array<float,2> calculateXY(float x0, float y0, float radius, float angle) {
	float x = x0 + sin(angle)*radius;
	float y = y0 + cos(angle)*radius;
	return {x, y};
}
void set_spots(spot* spots, int spotCount, float angleOffset, float circleRadius) {
	int spotRadius = 100;

	for (int i = 0; i < spotCount; i++) {
		std::array<float,2> pos = calculateXY(XRES/2, YRES/2, circleRadius, i * 2* 3.1415 /spotCount + angleOffset);
		spots[i].x = pos[0];
		spots[i].y = pos[1];
		spots[i].radius = spotRadius;
	}
}
float circleRadius = 230;
void demo_init() {
	baseP.initPerlin();
	velP.initPerlin();
	set_spots(spots, SPOT_COUNT, 0, circleRadius);
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
float angleOffset = 0;
void demo_do(SDL_Surface *surface, int delta) {
	cnt++;
	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			put_pixel32(surface, x, y, 0x0);
			for (int s = 0; s < SPOT_COUNT; s++) {
				int radius = sqrt(pow(abs(x - spots[s].x), 2) + pow(abs(y - spots[s].y), 2));
				if (radius < spots[s].radius) {
					float val = baseP.getPerlin((x + XRES/4)/(float)(50), (y + YRES/4)/(float)(50));
					put_pixel32(surface, x, y, hot_cold(val + .5));
				}
			}
		}
	}
	addPerlinGradient(baseP, velP);
	if (cnt % (int)(200 / delta) == 0) {
		velP.initPerlin();
	}
	set_spots(spots, SPOT_COUNT, angleOffset+=abs(sin((float)cnt/10)/10), circleRadius += 10*sin((float)cnt/3));
}
