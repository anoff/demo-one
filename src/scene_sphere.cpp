#include "scene_sphere.h"

#define INF 9999999
#define MIN_ILLUMINATION 0.2f

std::array<planet,5> spheres;
std::array<light,1> lights;
ray camera;
SimplexNoise background;

void scene_sphere_init() {
	camera.origin = vec3(0, 60, 0.1f);
	camera.dir = camera.origin * -1.f;
	camera.dir.normalize();

	lights[0] = light(0, 0, 0, 1.f);
	//lights[1] = light(camera.origin);

	spheres[0].init();
	spheres[0].radius = 10;
	spheres[1].init();
	spheres[1].radius = 2;
	spheres[2].init();
	spheres[2].radius = 3;
	spheres[3].init();
	spheres[3].radius = 1;
	spheres[4].init();
	spheres[4].radius = 1;
}

bool check_collision(ray r, float& t, planet** obj) {
	t = INF;
	float tMin = INF;
	for (int s = 0; s < spheres.size(); s++) {
		float t1 = spheres[s].intersect(r);
		if (t1 < 0) continue;
		// new shortest distance found
		if (t1 < t) {
			t = t1;
			 *obj = &spheres[s];
		}
	}
	if (t < INF) {
		return true;
	}
	return false;
}

float calc_intensity(vec3 point, vec3 normal) {
	float lightIntensity = 0.f;
	for (int lix = 0; lix < lights.size(); lix++) {
		light lightSource = lights[lix];
		// do this stuff for each light if there are multiple ones
		vec3 surf2Light = lightSource - point;
		ray l = ray(point, surf2Light);
		l.dir.normalize();
		float t;
		planet* obj = nullptr;
		bool lightSourceHidden = check_collision(l, t, &obj); // check if there is an object intersection on the light ray
		lightSourceHidden = lightSourceHidden && t < surf2Light.length(); // and object is closer than the light source
		if (!lightSourceHidden) {
			float lightCosine = normal.dot(l.dir) * lightSource.intensity;
			lightIntensity = lightCosine > lightIntensity ? clamp(lightCosine, 0, lightSource.intensity) : lightIntensity;
		}
	}
	return clamp(lightIntensity, MIN_ILLUMINATION, 1);
}

void scene_sphere_do(SDL_Surface *surface, int delta, int cnt) {
	spheres[0].center = vec3(60*sin(cnt/73.f), 0, 40*cos(cnt/73.f));
	spheres[1].center = vec3(20*sin(cnt/13.f), 0, 20*cos(cnt/13.f)) + spheres[0].center;
	spheres[2].center = vec3(9*sin(cnt/33.f), 0, -9*cos(cnt/33.f));
	spheres[3].center = vec3(4*sin(cnt/8.f), 0, 4*cos(cnt/8.f)) + spheres[2].center;
	spheres[4].center = vec3(4*sin(cnt/8.f + M_PI/2.f), 0, 4*cos(cnt/8.f + M_PI/2.f)) + spheres[2].center;

	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			ray r = generateViewport(x, y, camera); // generate a ray from the camera position through the current pixel position
			float t;
			planet* obj = nullptr;
			bool hasObject = check_collision(r, t, &obj); // check if there are any objects in view
			if (hasObject) {
				vec3 surfacePoint = r.origin + r.dir*t;
				vec3 normal = (surfacePoint - obj->center).normalize();
				surfacePoint += normal*1e-3;
				float lightIntensity = calc_intensity(surfacePoint, normal);
				vec3 texCoords = (surfacePoint - obj->center).normalize();
				float colorVal = obj->getTex(texCoords.x, texCoords.y, texCoords.z);
				uint32_t color = cm_terrain(colorVal/2 + 0.5);
				color = change_lightning(color, lightIntensity);
				put_pixel32(surface, x, y, color);
			} else {
				// add "stars" in background
				float value = background.noise(x/30.f, y/30.f)/2 + 0.5f; // normalize 0..1
				if (value > 0.9999f) {
					put_pixel32(surface, x, y, 0xffffff);
				} else {
					put_pixel32(surface, x, y, 0x0);
				}
			}
		}
	}
}
