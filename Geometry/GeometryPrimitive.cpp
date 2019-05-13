#include "Geometry.h"

#include <cmath>
#include <math.h>
#include <algorithm>

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
