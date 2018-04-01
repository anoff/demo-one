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
			float tMin = INF;
			for (int s = 0; s < spheres.size(); s++) {
				float t1 = spheres[s].intersect(r);
				if (t1 < 0) continue;
				t = (t1 < t) ? t1 : t;
				if (tMin == INF) {
					vec3 L = spheres[s].center - r.origin;
					tMin = L.length() - spheres[s].radius;
				}
			}
			if (t < INF) {
				int i = 3;
			}
			put_pixel32(surface, x, y, (t < INF) ? change_lightning(0xff00ff, 1.f - (t - tMin)*(t - tMin)) : 0x0);
		}
	}
}
