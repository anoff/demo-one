#ifndef COMMONS_H
#define COMMONS_H

#include "main.h"

// STRCUTS
struct coord {
	int x;
	int y;
};
struct spot {
	coord center;
	int radius;
};


void put_pixel32(SDL_Surface *surface, int x, int y, uint32_t pixel);
float clamp(float val, float low, float high);
uint32_t color(const int r, const int g, const int b);
uint32_t cm_hot_cold(float f);
uint32_t cm_grayscale(float f);
uint32_t cm_terrain(float f);
uint32_t change_lightning(uint32_t c, float scale);
coord calculateXY(float x0, float y0, float radius, float angle);
void set_spots(spot* spots, int spotCount, float angleOffset, float circleRadius);

#endif