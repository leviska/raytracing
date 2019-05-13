#include "Image.h"
#include "Geometry.h"
#include "RayTracing.h"
#include "PLY.h"

#include <iostream>
#include <vector>
#include <chrono>

const int W = 800;
const int H = 600;

void generateScene1(RayTracing& tracer) {
	GeometryPrimitive* obj;
	Mesh* mesh;

	//Background cube
	mesh = new Mesh();
	mesh->loadFromFile("./ply/scene.ply");
	mesh->doubleSided = true;
	mesh->scale(0.7);
	//mesh->rotate(Vector(0, 0, 3.1415));
	tracer.objects.push_back(mesh);

	//Sphere mesh
	mesh = new Mesh();
	mesh->createSphere(20, 20, 1);
	mesh->move(Vector(-1.5, 0, 0));
	mesh->material.reflection = 0;
	tracer.objects.push_back(mesh);

	//Sphere object
	Sphere *sph = new Sphere(Vector(1.5, 0, 0), 1);
	sph->material.reflection = 0.8;
	tracer.objects.push_back(sph);

	//Cuboid mesh
	mesh = new Mesh();
	mesh->createCuboid(1, 1, 1);
	mesh->move(Vector(-0.5, -0.5, -0.5));
	mesh->rotate(Vector(0.4, 0, 0.78));
	mesh->move(Vector(0, 2, 0));
	tracer.objects.push_back(mesh);
}

void generateScene2(RayTracing& tracer) {
	GeometryPrimitive* obj;
	Mesh* mesh;

	//Background cube
	mesh = new Mesh();
	mesh->loadFromFile("./ply/scene.ply");
	mesh->doubleSided = true;
	mesh->scale(0.7);
	//mesh->rotate(Vector(0, 0, 3.1415));
	tracer.objects.push_back(mesh);

	obj = new Sphere(Vector(6, 3, 0), 1);
	obj->material = Material::Chrome;
	tracer.objects.push_back(obj);
	obj = new Sphere(Vector(3, 3, 0), 1);
	obj->material = Material::Gold;
	tracer.objects.push_back(obj);
	obj = new Sphere(Vector(0, 3, 0), 1);
	obj->material = Material::None;
	tracer.objects.push_back(obj);
	obj = new Sphere(Vector(-3, 3, 0), 1);
	obj->material = Material::Rubber;
	tracer.objects.push_back(obj);
	obj = new Sphere(Vector(-6, 3, 0), 1);
	obj->material = Material::Plastic;
	tracer.objects.push_back(obj);
}

int main() {
	//Just in case to generate random scenes
	srand(time_t(0));

	RayTracing tracer;

	generateScene1(tracer);

	//Light
	tracer.lights.push_back(Vector(0, -5, 2));

	//Camera
	tracer.camera.center = Vector(0, -5, 0);
	tracer.camera.dir = Vector(0, 3, 0);
	tracer.camera.width = 8;
	tracer.camera.height = 4.5;
	tracer.camera.resWidth = W;
	tracer.camera.resHeight = H;

	tracer.antialiasing = 4;

	for (int i = 0; i < 1; i++) {
		std::string fileName = "image" + std::string(i < 10 ? "0" : "") + std::to_string(i) + ".ppm";
		auto started = std::chrono::high_resolution_clock::now();

		Image res = tracer.rayTracing();
		res.save("result/" + fileName);

		auto done = std::chrono::high_resolution_clock::now();
		std::cout << "Ended " << fileName << " in ";
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() / 1000.0;
		std::cout << " seconds" << std::endl;
	}

	return 0;
}
