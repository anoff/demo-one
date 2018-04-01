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

bool checkCollision(ray r, float& t) {
	t = INF;
	float tMin = INF;
	for (int s = 0; s < spheres.size(); s++) {
		float t1 = spheres[s].intersect(r);
		if (t1 < 0) continue;
		t = (t1 < t) ? t1 : t;
	}
	if (t < INF) {
		return true;
	}
	return false;
}

float calcIntensity(vec3 point, ray incomingRay) {
	float lightIntensity = 0;
	vec3 lightSource = lights[0];
	// do this stuff for each light if there are multiple ones
	vec3 surf2Light = lightSource - point;
	ray l = ray(point, surf2Light);
	l.dir.normalize();
	float t;
	bool lightSourceHidden = checkCollision(l, t); // check if there is an object intersection on the light ray
	lightSourceHidden = lightSourceHidden && t < surf2Light.length(); // and object is closer than the light source
	if (!lightSourceHidden) {
		float lightAngle = incomingRay.dir.dot(l.dir);
		float cosin = cos(lightAngle);
		lightIntensity = clamp((cosin + 1) / 2, 0, 1);
	}
	return lightIntensity;
}

void scene_sphere_do(SDL_Surface *surface, int delta, int cnt) {
	for (int y = 300; y<surface->h; y++) {
		for (int x = 400; x<surface->w; x++) {
			ray r = generateViewport(x, y); // generate a ray from the camera position through the current pixel position
			float t;
			bool hasObject = checkCollision(r, t); // check if there are any objects in view
			if (hasObject) {
				vec3 surfacePoint = r.origin + r.dir*(t - 1e-6);
				float lightIntensity = calcIntensity(surfacePoint, r);
				float color = change_lightning(0xff00ff, lightIntensity);
				put_pixel32(surface, x, y, color);
			} else {
				put_pixel32(surface, x, y, 0x0);
			}
		}
	}
}
