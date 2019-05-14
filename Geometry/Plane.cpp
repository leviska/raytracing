#include "Plane.h"

#include <cmath>
#include <algorithm>

Plane::Plane() {
	doubleSided = false;
	material = Material::None;
}

Plane::Plane(Vector _center, Vector _normal, Vector _edge) {
	center = _center;
	normal = _normal;
	edge = _edge;
	doubleSided = false;
}

IntersectData Plane::intersect(const Line & line) {
	double a = (center - line.org) * normal;
	double b = line.dir * normal;
	IntersectData data;
	if (std::abs(b) > eps) {
		double k = a / b;
		if (k >= 0) {
			if (edge() > 0) {
				Vector point = line.org + line.dir * k;
				Vector prp = edge / normal;
				Vector dif = point - center;
				double a = dif * edge / edge();
				double b = dif * prp / prp();
				if (std::abs(a) <= edge() && std::abs(b) <= edge()) {
					data.koef = k;
					data.normal = normal;
					data.color = Color(250, 250, 250);
				}
			}
			else {
				data.koef = k;
				data.normal = normal;
				data.color = Color(250, 250, 250);
			}
		}
	}
	return data;
}

void Plane::precalc() {
}

void Plane::initFromNode(const XML::Node* node) {
	GeometryPrimitive::initFromNode(node);
	center.x = node->attrd("centerX");
	center.y = node->attrd("centerY");
	center.y = node->attrd("centerZ");
	normal.x = node->attrd("normalX");
	normal.y = node->attrd("normalY");
	normal.z = node->attrd("normalZ");
	edge.x = node->attrd("edgeX");
	edge.y = node->attrd("edgeY");
	edge.z = node->attrd("edgeZ");
}
