#pragma once
#include <vector>

#include "../Image.h"
#include "../Material.h"
#include "../Vector.h"

const double eps = 1e-6;
const int CUBE_DIVISION = 8;

struct IntersectData {
	double koef;
	Vector normal;
	Color color;
	int id;

	IntersectData() : normal(), color() {
		koef = -1;
		id = -1;
	}
};

class Line;

class GeometryPrimitive {
public:
	Material material;
	bool doubleSided;

	virtual IntersectData intersect(const Line& line) = 0;
	virtual void precalc() = 0;
};

class Line : public GeometryPrimitive {
public:
	Vector org;
	Vector dir;

	Line();
	Line(Vector org, Vector dir);

	virtual IntersectData intersect(const Line & line) override;

	virtual void precalc() override;
};
