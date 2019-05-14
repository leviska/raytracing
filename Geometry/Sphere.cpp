#include "Sphere.h"

#include <cmath>
#include <algorithm>

Sphere::Sphere() {
	r = 0;
	doubleSided = false;
	material = Material::None;
}

Sphere::Sphere(Vector _center, double _r) {
	center = _center;
	r = _r;
	doubleSided = false;
}

IntersectData Sphere::intersect(const Line & line) {
	Vector d = (line.org - center);
	double dabs = d();
	double a = (line.dir * d);
	double sq = a * a - dabs * dabs + r * r;
	IntersectData data;
	if (sq >= 0) {
		sq = std::sqrt(sq);
		double k1 = -a + sq;
		double k2 = -a - sq;
		if (k1 >= 0 && k2 >= 0) {
			data.koef = std::min(k1, k2);
		}
		else if (k1 >= 0) {
			data.koef = k1;
		}
		else if (k2 >= 0) {
			data.koef = k2;
		}
		data.normal = ((line.org + line.dir * data.koef) - center).makeUnit();
		data.color = Color(255, 255, 255);
	}
	return data;
}

void Sphere::precalc() {
}

void Sphere::initFromNode(const XML::Node* node) {
	GeometryPrimitive::initFromNode(node);
	center.x = node->attrd("centerX");
	center.y = node->attrd("centerY");
	center.z = node->attrd("centerZ");
	r = node->attrd("r");
}
