#include "scene_sphere.hpp"

#define INF 9999999
#define MIN_ILLUMINATION 0.2f

std::vector<Planet> planets;
std::vector<Ball> stars;
std::array<light,1> lights;
Ray camera;
SimplexNoise background;

void scene_sphere_init() {
	camera.origin = vec3(0, 60, 0.1f);
	camera.dir = camera.origin * -1.f;
	camera.dir.normalize();

	lights[0] = light(0, 4, 0, 1);
	//lights[1] = light(camera.origin);
	Ball sun;
	sun.center = vec3(0, 0, 0);
	sun.intensity = 1.0f;
	sun.radius = 5;
	sun.color = 0xFFFF00;
	stars.push_back(sun);

	for (int i = 0; i < 4; i++) {
		Planet p;
		planets.push_back(p);
	}
	planets[0].radius = 10;
	planets[0].cm = Planet::ColorMap::terrain;
	planets[1].radius = 2;
	planets[2].radius = 3;
	planets[2].cm = Planet::ColorMap::lava;
	planets[3].radius = 2;
	planets[3].cm = Planet::ColorMap::sun;

	// add stars
	/*
	for (float u = 0; u < M_PI*2; u=u+0.3f) {
		for (float v = 0; v < M_PI*2; v=v+0.3f) {
			Ray r;
			r.dir = vec3(sin(u)*cos(v), sin(u)*sin(v), cos(u));
			vec3 starPos = r.origin + r.dir*1000;
			float f = background.noise(starPos.x, starPos.y, starPos.z)/2 + 0.5;
			if (f > 0.8f) {
				Ball s;
				s.radius = 5;
				s.center = starPos;
				s.color = 0xffffd0;
				stars.push_back(s);
			}
		}
	}*/
}

std::vector<Object*> get_all_objects() {
	std::vector<Object*> objects;
	for (int i = 0; i < planets.size(); i++) {
		objects.push_back(&planets[i]);
	}
	for (int i = 0; i < stars.size(); i++) {
		objects.push_back(&stars[i]);
	}
	return objects;
}

bool check_collision(Ray r, float& t, Planet** obj) {
	t = INF;
	float tMin = INF;
	for (int s = 0; s < planets.size(); s++) {
		float t1 = planets[s].intersect(r);
		if (t1 < 0) continue;
		// new shortest distance found
		if (t1 < t) {
			t = t1;
			 *obj = &planets[s];
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
		Ray l = Ray(point, surf2Light);
		l.dir.normalize();
		float t;
		Planet* obj = nullptr;
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
	planets[0].center = vec3(60*sin(cnt/73.f), 0, 40*cos(cnt/73.f));
	planets[1].center = vec3(20*sin(cnt/13.f), 0, 20*cos(cnt/13.f)) + planets[0].center;
	planets[2].center = vec3(13*sin(cnt/33.f), 0, -13*cos(cnt/33.f));

	//camera.dir = (planets[0].center - camera.origin).normalize();
	for (int y = 0; y<surface->h; y++) {
		for (int x = 0; x<surface->w; x++) {
			Ray r = generateViewport(x, y, camera); // generate a ray from the camera position through the current pixel position
			float t;
			Planet* obj = nullptr;
			bool hasObject = check_collision(r, t, &obj); // check if there are any objects in view
			if (hasObject) {
				vec3 surfacePoint = r.origin + r.dir*t;
				vec3 normal = (surfacePoint - obj->center).normalize();
				surfacePoint += normal*1e-3;
				float lightIntensity = calc_intensity(surfacePoint, normal);
				vec3 texCoords = (surfacePoint - obj->center).normalize();
				uint32_t color = obj->get_color(texCoords.x, texCoords.y, texCoords.z);
				color = change_lightning(color, lightIntensity);
				put_pixel32(surface, x, y, color);
			} else {
					put_pixel32(surface, x, y, 0x0);
			}
		}
	}
}
