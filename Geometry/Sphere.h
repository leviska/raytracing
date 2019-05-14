#pragma once
#include "GeometryPrimitive.h"

class Sphere : public GeometryPrimitive {
public:
	Vector center;
	double r;

	Sphere();
	Sphere(Vector center, double r);

	virtual IntersectData intersect(const Line & line) override;

	virtual void precalc() override;

	virtual void initFromNode(const XML::Node* node) override;
};
