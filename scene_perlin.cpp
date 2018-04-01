#include "scene_perlin.h"
#include "commons.h"

float circleRadius = 150;
int spotCount = 4;
spot spots[SPOTS_MAX];
Perlin baseP;
Perlin velP;

void scene_perlin_init() {
	baseP.initPerlin();
	velP.initPerlin();
	set_spots(spots, spotCount, 0, circleRadius);
}

float angleOffset = 0;
void scene_perlin_do(SDL_Surface *surface, int delta, int cnt) {
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
