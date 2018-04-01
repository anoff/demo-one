#include "scene_sphere.h"
#include "ray.h"
#include "commons.h"

#define INF 9999999

std::array<sphere,1> spheres;
void scene_sphere_init() {
	spheres[0].center = vec3(0, 0, 10.f);
	spheres[0].radius = 3;
}

void scene_sphere_do(SDL_Surface *surface, int delta, int cnt) {
	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			ray r = generateViewport(x, y);
			float t = INF;
			for (int s = 0; s < spheres.size(); s++) {
				float t1 = spheres[s].intersect(r);
				if (t1 < 0) continue;
				t = (t1 < t) ? t1 : t;
			}
			put_pixel32(surface, x, y, (t < INF) ? 0xff00ff : 0x0);
		}
	}
}
