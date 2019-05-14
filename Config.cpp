#include "Config.h"

#include <iostream>
#include <chrono>

Config::~Config() {
	for(auto i : tracer.objects) {
		delete(i);
	}
}

void Config::load(std::string fileName) {
	XML::File file;
	if (!file.open(fileName)) {
		return;
	}
	XML::Node* root = file.root;
	XML::Node* cameraTag = nullptr;
	XML::Node* objectsTag = nullptr;
	XML::Node* lightsTag = nullptr;

	//Parcing root
	for (XML::Node* node : root->children) {
		if (!node) {
			continue;
		}
		if (node->name == "cameras") {
			cameraTag = node;
		}
		else if (node->name == "objects") {
			objectsTag = node;
		}
		else if (node->name == "lights") {
			lightsTag = node;
		}
	}
	if (!cameraTag || !objectsTag || !lightsTag) {
		return;
	}

	//Parcing cameras
	for (XML::Node* node : cameraTag->children) {
		if (!node) {
			continue;
		}
		Camera camera;
		camera.center.x = node->attrd("centerX");
		camera.center.y = node->attrd("centerY");
		camera.center.z = node->attrd("centerZ");
		camera.dir.x = node->attrd("dirX");
		camera.dir.y = node->attrd("dirY");
		camera.dir.z = node->attrd("dirZ");
		camera.resWidth = node->attri("resWidth");
		camera.resHeight = node->attri("resHeight");
		camera.width = node->attrd("width");
		camera.height = node->attrd("height");
		camera.antialiasing = node->attri("antialiasing", 1);
		cameras.push_back(camera);
	}

	//Parcing objects
	for (XML::Node* node : objectsTag->children) {
		if (!node) {
			continue;
		}
		GeometryPrimitive* obj = ClassNames.at(node->name)();
		obj->initFromNode(node);
		tracer.objects.push_back(obj);
	}

	//Parcing lights
	for (XML::Node* node : lightsTag->children) {
		if (!node) {
			continue;
		}
		Vector light;
		light.x = node->attrd("lightX");
		light.y = node->attrd("lightY");
		light.z = node->attrd("lightZ");
		tracer.lights.push_back(light);
	}
}

Image Config::render(int i) {
	tracer.camera = cameras[i];
	return tracer.rayTracing();
}

void Config::render() {
	for (int i = 0; i < cameras.size(); i++) {
		std::string fileName = "image" + std::string(i < 10 ? "0" : "") + std::to_string(i) + ".ppm";
		auto started = std::chrono::high_resolution_clock::now();

		Image res = render(i);
		res.save("result/" + fileName);

		auto done = std::chrono::high_resolution_clock::now();
		std::cout << "Ended " << fileName << " in ";
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() / 1000.0;
		std::cout << " seconds" << std::endl;
	}
}
