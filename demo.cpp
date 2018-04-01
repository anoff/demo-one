#include "demo.h"
#define SPOTS_MAX 5
#define GLOW_RADIUS 90

struct coord {
	int x;
	int y;
};
struct spot {
	coord center;
	int radius;
} spots[SPOTS_MAX];

float circleRadius = 150;
int spotCount = 4;
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
uint32_t change_lightning(uint32_t c, float scale) {
	float r = (float)((c & 0xff0000) >> 16);
	float g = (float)((c & 0x00ff00) >> 8);
	float b = (float)(c & 0x0000ff);

	r = clamp(r*scale, 0.f, 255.f);
	g = clamp(g*scale, 0.f, 255.f);
	b = clamp(b*scale, 0.f, 255.f);
	return color(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
}
coord calculateXY(float x0, float y0, float radius, float angle) {
	coord response;
	response.x = x0 + sin(angle)*radius;
	response.y = y0 + cos(angle)*radius;
	return response;
}
void set_spots(spot* spots, int spotCount, float angleOffset, float circleRadius) {
	int spotRadius = 100 - clamp(spotCount - 3, 0, 10) * 10;

	for (int i = 0; i < spotCount; i++) {
		coord pos = calculateXY(XRES/2, YRES/2, circleRadius, i * 2* 3.1415 /spotCount + angleOffset);
		spots[i].center = pos;
		spots[i].radius = spotRadius;
	}
}

void demo_init() {
	baseP.initPerlin();
	velP.initPerlin();
	set_spots(spots, spotCount, 0, circleRadius);
}

// add the gradients of p2 to p1 using a factor
// 	the factor determines how fast p1 converges against p2
void addPerlinGradient(Perlin &p1, Perlin &p2, float factor) {
	for (int y = 0; y < YRES; y++) {
		for (int x = 0; x < XRES; x++) {
			float v0 = p1.Gradient[y][x][0] + factor * p2.Gradient[y][x][0];
			float v1 = p1.Gradient[y][x][1] + factor * p2.Gradient[y][x][1];
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
			float lightning = 0;
			for (int s = 0; s < spotCount; s++) {
				int radius = sqrt(pow(abs(x - spots[s].center.x), 2) + pow(abs(y - spots[s].center.y), 2));
				if (spots[s].center.x == 0 && spots[s].center.y == 0) lightning = lightning > 0 ? lightning : 0;
				else if (radius < spots[s].radius) {
					lightning = 1;
				} else if (radius < spots[s].radius + GLOW_RADIUS) {
					float glowDistance = radius - spots[s].radius;
					lightning = lightning > (GLOW_RADIUS - glowDistance)/GLOW_RADIUS ? lightning : (GLOW_RADIUS - glowDistance)/GLOW_RADIUS;
				}
				if (lightning > 0) {
					float val = baseP.getPerlin((x + XRES/4)/(float)(50), (y + YRES/4)/(float)(50));
					put_pixel32(surface, x, y, change_lightning(hot_cold(val + .5), lightning));
				} else {
					put_pixel32(surface, x, y, 0x00);
				}
			}
		}
	}
	addPerlinGradient(baseP, velP, 0.1);
	if (cnt % (int)(200 / delta) == 0) {
		velP.initPerlin();
	}
	if (cnt % (int)(500 / delta) == 0) {
		spotCount = rand() % SPOTS_MAX + 1;
		for (int s = 0; s < SPOTS_MAX; s++) {
			spots[s].center.x = 0;
			spots[s].center.y = 0;
		}
		set_spots(spots, spotCount, angleOffset, circleRadius);
	}
	set_spots(spots, spotCount, angleOffset+=abs(sin((float)cnt/10)/10), circleRadius += 10*sin((float)cnt/3));
}
