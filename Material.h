#pragma once
#include "Vector.h"
#include "Image.h"

struct Material {
	Vector ambient;
	Vector diffuse;
	Vector specular;
	double shininess;

	double reflection;
	//TODO:
	double transparency;
	//TODO:
	double refraction;

	Color color;

	static const Material None;
	static const Material Ruby;
	static const Material Chrome;
	static const Material Silver;
	static const Material Gold;
	static const Material Plastic;
	static const Material Rubber;
	static const Material Glass;
};