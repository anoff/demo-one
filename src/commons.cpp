#include "commons.hpp"
#include <cmath>

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
uint32_t cm_grayscale(float f) {
	f = clamp(f, 0.f, 1.f);
	f = 2*f*f;
	f = clamp(f, 0.f, 1.f);
	return color(static_cast<int>(255.f * f), static_cast<int>(255.f * f), static_cast<int>(255.f * f));
}
uint32_t cm_hot_cold(float f) {
	clamp(f, 0.f, 1.f);
	f = (2.f * f) - 1.f;
	float r = clamp(1.5f - abs(2.f * f - 1.f), 0.f, 1.f);
	float g = clamp(1.5f - abs(2.f * f), 0.f, 1.f);
	float b = clamp(1.5f - abs(2.f * f + 1.f), 0.f, 1.f);
	return color(static_cast<int>(255.f * r), static_cast<int>(255.f * g), static_cast<int>(255.f * b));
}
uint32_t cm_terrain(float f) {
	clamp(f, 0.f, 1.f);
	// color scale: blue -> green -> white
	//
	// stop 		r 			g				b
	// 0.0			0				0				0.3
	// 0.13			0 			0				1.0
	// 0.5			0				0.5			0
	// 0.85			0.75		0.75		0.75
	// 1.0 			1				1				1
	// set according to sections
	float r, g, b;
	if (f > 0.85f) {
		// 0..1 how finished this section is (color = t*gradient + offset)
		float t = clamp(f - 0.85f, 0, 1)/0.15f;
		r = 0.25f*t + 0.75f;
		g = 0.25f*t + 0.75f;
		b = 0.25f*t + 0.75f;
	} else if (f > 0.5f) {
		float t = clamp(f - 0.5f, 0, 1)/0.35f;
		r = 0.75f*t;
		g = 0.25f*t + 0.5f;
		b = 0.75f*t;
	} else if (f > 0.13f) {
		float t = clamp(f - 0.13f, 0, 1)/0.37f;
		r = 0;
		g = 0.5f*t;
		b = -t + 1.f;
	} else { // f > 0
		float t = f/0.13f;
		r = 0;
		g = 0;
		b = 0.7f*t + 0.3f;
	}
	r = clamp(r, 0.f, 1.f);
	g = clamp(g, 0.f, 1.f);
	b = clamp(b, 0.f, 1.f);
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