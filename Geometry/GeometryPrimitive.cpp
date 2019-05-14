#include "Geometry.h"

#include <cmath>
#include <math.h>
#include <algorithm>


void GeometryPrimitive::initFromNode(const XML::Node* node) {
	doubleSided = node->attr("doubleSided") == "true";

	material.ambient.x = node->attrd("ambientX", Material::None.ambient.x);
	material.ambient.y = node->attrd("ambientY", Material::None.ambient.y);
	material.ambient.z = node->attrd("ambientZ", Material::None.ambient.z);

	material.diffuse.x = node->attrd("diffuseX", Material::None.diffuse.x);
	material.diffuse.y = node->attrd("diffuseY", Material::None.diffuse.y);
	material.diffuse.z = node->attrd("diffuseZ", Material::None.diffuse.z);

	material.specular.x = node->attrd("specularX", Material::None.specular.x);
	material.specular.y = node->attrd("specularY", Material::None.specular.y);
	material.specular.z = node->attrd("specularZ", Material::None.specular.z);

	material.shininess = node->attrd("shininess", Material::None.shininess);

	material.reflection = node->attrd("reflection", Material::None.reflection);
	material.transparency = node->attrd("transparency", Material::None.transparency);
	material.refraction = node->attrd("refraction", Material::None.refraction);

	material.color.r = node->attri("colorR", Material::None.color.r);
	material.color.g = node->attri("colorG", Material::None.color.g);
	material.color.b = node->attri("colorB", Material::None.color.b);
}


Line::Line() {
	doubleSided = false;

	material = Material::None;
}

Line::Line(Vector _org, Vector _dir) {
	org = _org;
	dir = _dir;
	doubleSided = false;
}

IntersectData Line::intersect(const Line & line) {
	IntersectData data;
	double a = dir * (org - line.org);
	double b = (dir / line.dir)();
	if (abs(b) > eps) {
		data.koef = a / b;
	}
	return data;
}

void Line::precalc() {
}

//don't need this
void Line::initFromNode(const XML::Node* node) {
}
