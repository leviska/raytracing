#include "RayTracing.h"

#include <algorithm>
#include <iostream>
#include <thread>
#include <cmath>

IntersectData RayTracing::intersect(const Line & ray) {
	IntersectData res;

	//Going over all objects
	for (size_t i = 0; i < objects.size(); i++) {
		//Just intersect them
		IntersectData temp = objects[i]->intersect(ray);
		//If have intersection and earlier than current, set it as current
		if (temp.koef > eps && (res.id < 0 || temp.koef < res.koef - eps)) {
			res = temp;
			res.id = i;
		}
	}
	return res;
}

Color RayTracing::singleRay(const Line & ray, int iter) {
	//Checking for intersection with objects
	IntersectData res = intersect(ray);

	//If have intersection
	if (res.id >= 0) {
		//Get point of intersection
		Vector point = ray.org + ray.dir * res.koef;

		//Utility
		auto mul = [](Color& c, Vector& v) {
			c.r *= v.x;
			c.g *= v.y;
			c.b *= v.z;
		};
		auto vecmax = [](Vector& c) {
			c.x = std::max(c.x, 0.0);
			c.y = std::max(c.y, 0.0);
			c.z = std::max(c.z, 0.0);
		};
		auto vecmin = [](Vector& c) {
			c.x = std::min(c.x, 1.0);
			c.y = std::min(c.y, 1.0);
			c.z = std::min(c.z, 1.0);
		};

		//Lighting
		Vector lightKoef = objects[res.id]->material.ambient;
		for (int i = 0; i < lights.size(); i++) {
			const Vector& light = lights[i];
			Line lightRay(light, (point - light).makeUnit());

			bool intersects = false;
			double maxKoef = (point - light)();
			for (size_t i = 0; i < objects.size(); i++) {
				IntersectData temp = objects[i]->intersect(lightRay);
				if (temp.koef > eps && temp.koef < maxKoef - eps) {
					intersects = true;
					break;
				}
			}
			if (!intersects) {
				Vector diffuse = ((point - light).makeUnit() * (res.normal * -1)) * objects[res.id]->material.diffuse;
				if (objects[res.id]->doubleSided && diffuse.x < 0) diffuse = -1 * diffuse;

				Vector idealReflection = (lightRay.dir - 2 * (lightRay.dir * res.normal) * res.normal).makeUnit();
				Vector specular = std::pow(std::max(0.0, idealReflection * (ray.dir * -1)), objects[res.id]->material.shininess) * objects[res.id]->material.specular;
				vecmax(diffuse);
				vecmax(specular);

				lightKoef = lightKoef + diffuse + specular;
			}
		}
		vecmin(lightKoef);
		mul(res.color, lightKoef);

		//Reflection
		double reflection = objects[res.id]->material.reflection;
		if (reflection > eps && iter < ITERATION_BREAK) {
			res.color = res.color * (1 - reflection);
			Line reflRay(Vector(point), (ray.dir - 2 * (ray.dir * res.normal) * res.normal).makeUnit());
			reflRay.org = reflRay.org + reflRay.dir * eps;
			Color reflColor = singleRay(reflRay, iter + 1);
			Vector t(reflColor.r, reflColor.g, reflColor.b);
			res.color = res.color + reflColor * reflection;
		}

		//TODO: transparency
		//TODO: refraction

		return res.color;
	}
	return Color();
}

void RayTracing::rayTracingRange(int from, int to, Image * res) {
	int screenSize = camera.resWidth * camera.resHeight / 2;

	//Going over all pixels in range
	for (int t = from; t < to; t++) {
		int v;
		//To move from center to borders
		if (t % 2) v = screenSize - (t + 1) / 2;
		else v = screenSize + t / 2;

		//Get image position
		int i = v / camera.resHeight;
		int j = v % camera.resHeight;

		int r = 0, g = 0, b = 0;
		for (int ti = 0; ti < antialiasing; ti++) {
			for (int tj = 0; tj < antialiasing; tj++) {
				//Get ray
				Vector p(i - camera.resWidth / 2 + double(ti) / antialiasing, 0, j - camera.resHeight / 2 + double(tj) / antialiasing);
				p.x *= camera.width / camera.resWidth;
				p.z *= camera.height / camera.resHeight;
				p = (p + camera.dir).makeUnit();

				//Sending the ray
				Color tmp = singleRay(Line(camera.center, p));
				r += tmp.r;
				g += tmp.g;
				b += tmp.b;
			}
		}
		res->set(i, camera.resHeight - j - 1, Color(r / (antialiasing * antialiasing), g / (antialiasing * antialiasing), b / (antialiasing * antialiasing)));
	}
}

RayTracing::RayTracing() : camera(), objects(), lights() {
	antialiasing = 1;
}

Image RayTracing::rayTracing() {
	int screenSize = camera.resWidth * camera.resHeight;
	antialiasing = std::max(antialiasing, 1);

	//Creating an image
	Image res(camera.resWidth, camera.resHeight);

	//Precalc everything for objects
	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->precalc();
	}

	//Threading and raytracing
	int threadsCount = std::max(1U, std::thread::hardware_concurrency());
	std::cout << "Detected " << threadsCount << " threads\n";

	int dif = PIXELS_PER_THREAD / antialiasing;
	if (dif * threadsCount > screenSize) {
		dif = screenSize / threadsCount + 1;
	}

	for (int t = 0; t < screenSize;) {
		std::vector<std::thread> threads;

		//Creating as many threads as we can
		for (int thr = 0; thr < threadsCount && t < screenSize; thr++) {
			int end = std::min(screenSize, t + dif);
			threads.push_back(std::thread(&RayTracing::rayTracingRange, this, t, end, &res));
			t += dif;
		}

		//Waiting them to finish
		for (int thr = 0; thr < threads.size(); thr++) {
			threads[thr].join();
		}

		//Temp output
		//It is taking time actually, so removing it will increase speed
		std::cout << "Ended " << t << "/" << screenSize << " (" << (t * 100 / screenSize) << "%)\n";
		//res.save("imageTemp.ppm");
	}

	return res;
}
