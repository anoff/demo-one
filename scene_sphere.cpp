#include "scene_sphere.h"
#include "ray.h"
#include "commons.h"

#define INF 9999999

std::array<sphere,1> spheres;
std::array<vec3,1> lights;
void scene_sphere_init() {
	spheres[0].center = vec3(0, 0, 10.f);
	spheres[0].radius = 3;

	lights[0] = vec3(5, 5, 0);
}

float checkCollision(ray r) {
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
	return t;
}

float calcIntensity(vec3 point, ray incomingRay) {
	float lightIntensity = 0;
	ray l = ray(point, lights[0] - point);
	l.dir.normalize();
	float t = checkCollision(l);
	float distL = (point - lights[0]).length();
	if ( t > 0 && (t == INF /* no collission between light source and point */
		|| (t < INF && distL < t))) { // collision object is behind lightsource
			float lightAngle = incomingRay.dir.dot(l.dir);
			lightIntensity = clamp(cos(lightAngle), 0, 1);
		}
	return lightIntensity;
}

void scene_sphere_do(SDL_Surface *surface, int delta, int cnt) {
	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			ray r = generateViewport(x, y);
			float t = checkCollision(r);
			if (t < INF && t > 0) {
				vec3 hit = r.origin + r.dir*(t-1e-9);
				float lightIntensity = calcIntensity(hit, r);
				put_pixel32(surface, x, y, change_lightning(0xff00ff, lightIntensity));
			} else {
				put_pixel32(surface, x, y, 0x0);
			}
		}
	}
}
