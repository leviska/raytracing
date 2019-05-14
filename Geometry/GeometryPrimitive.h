#pragma once
#include <vector>

#include "../Material.h"
#include "../Vector.h"
#include "../Utils/Image.h"
#include "../Utils/XML.h"

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
	virtual void initFromNode(const XML::Node* node);
};

class Line : public GeometryPrimitive {
public:
	Vector org;
	Vector dir;

	Line();
	Line(Vector org, Vector dir);

	virtual IntersectData intersect(const Line & line) override;

	virtual void precalc() override;
	virtual void initFromNode(const XML::Node* node) override;
};
