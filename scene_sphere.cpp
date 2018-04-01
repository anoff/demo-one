#include "scene_sphere.h"

#define INF 9999999
#define MIN_ILLUMINATION 0.2

std::array<ball,1> spheres;
std::array<light,1> lights;
ray camera;

void scene_sphere_init() {
	camera.origin = vec3(0, 60, 0.1f);
	camera.dir = camera.origin * -1.f;
	camera.dir.normalize();

	lights[0] = light(0, 0, 0, 1.f);
	//lights[1] = light(50, 0, 0, 0.6f);

	spheres[0] = ball();
}

bool check_collision(ray r, float& t, ball& obj) {
	t = INF;
	float tMin = INF;
	for (int s = 0; s < spheres.size(); s++) {
		float t1 = spheres[s].intersect(r);
		if (t1 < 0) continue;
		// new shortest distance found
		if (t1 < t) {
			t = t1;
			obj = spheres[s];
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
		ball obj;
		bool lightSourceHidden = check_collision(l, t, obj); // check if there is an object intersection on the light ray
		lightSourceHidden = lightSourceHidden && t < surf2Light.length(); // and object is closer than the light source
		if (!lightSourceHidden) {
			float lightCosine = normal.dot(l.dir) * lightSource.intensity;
			lightIntensity = lightCosine > lightIntensity ? clamp(lightCosine, 0, lightSource.intensity) : lightIntensity;
		}
	}
	return clamp(lightIntensity, MIN_ILLUMINATION, 1);
}

void scene_sphere_do(SDL_Surface *surface, int delta, int cnt) {
	spheres[0].center = vec3(40*sin(cnt/30.f), 0, 40*cos(cnt/30.f));
	spheres[0].radius = 3;

	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			ray r = generateViewport(x, y, camera); // generate a ray from the camera position through the current pixel position
			float t;
			ball obj;
			bool hasObject = check_collision(r, t, obj); // check if there are any objects in view
			if (hasObject) {
				vec3 surfacePoint = r.origin + r.dir*t;
				vec3 normal = (surfacePoint - obj.center).normalize();
				surfacePoint += normal*1e-3;
				float lightIntensity = calc_intensity(surfacePoint, normal);
				/*
				uv texCoords = obj.getUV(surfacePoint);
				float colorVal = obj.get_texture(texCoords.u/50, texCoords.v/50);
				uint32_t color = hot_cold(colorVal + .5);
				*/
				uint32_t color = 0xff0000;
				color = change_lightning(color, lightIntensity);
				put_pixel32(surface, x, y, color);
			} else {
				put_pixel32(surface, x, y, 0x0);
			}
			if (x == XRES/2 || y == YRES/2) {
				put_pixel32(surface, x, y, 0xFFFFFF);
			}
		}
	}
}
