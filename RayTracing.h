#pragma once
#include "Image.h"
#include "Geometry.h"

#include <vector>

const int PIXELS_PER_THREAD = 20000;
const int ITERATION_BREAK = 5;

struct Camera {
	Vector center;
	Vector dir;

	//Image resolution
	int resWidth;
	int resHeight;

	//In space camera size
	//Aka FOV
	double width;
	double height;

	Camera() : center(), dir() {
		resWidth = 0;
		resHeight = 0;
		width = 0;
		height = 0;
	}
};

class RayTracing {
	IntersectData intersect(const Line& ray);
	Color singleRay(const Line& ray, int iter = 0);
	void rayTracingRange(int from, int to, Image* res);

public:
	std::vector<GeometryPrimitive*> objects;
	std::vector<Vector> lights;
	Camera camera;
	int antialiasing;

	RayTracing();
	Image rayTracing();
};