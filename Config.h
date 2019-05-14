#include "RayTracing.h"
#include "Utils/XML.h"

#include <map>
#include <vector>

//I want a map of <Class Name, Class Type> to create instances of given type
typedef GeometryPrimitive* (*Creator)();
template <typename T>
GeometryPrimitive* CreateInstance() {
	return new T{};
}

const std::map<std::string, Creator> ClassNames = {
	{ "Sphere", CreateInstance<Sphere> },
	{ "Plane", CreateInstance<Plane> },
	{ "Mesh", CreateInstance<Mesh> },
};

class Config {

public:
	std::vector<Camera> cameras;
	RayTracing tracer;

	~Config();

	void load(std::string fileName);

	Image render(int i);

	void render();
};
